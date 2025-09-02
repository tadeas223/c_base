#include "c_base/base/errors/C_EmptyResult.h"
#include "c_base/base/errors/C_Result.h"
#include "c_base/base/errors/errors_base.h"
#include "c_base/base/memory/allocator.h"
#include "c_base/base/memory/memory.h"
#include "c_base/base/memory/memory_base.h"
#include "c_base/base/memory/objects.h"
#include "c_base/base/strings/string_convert.h"
#include "c_base/base/types.h"
#include "c_base/os/os_io.h"
#include "c_base/system.h"
#include <c_base/os/os_atomic.h>
#include <c_base/os/os_threads.h>

#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <linux/futex.h>
#include <sched.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <unistd.h>

GenericValImpl_ErrorCode(EG_OS_THREADS)

struct C_Thread {
  ClassObject base;

  // atomic
  u32 done;

  u64 stack_size;
  u8* stack;
  C_Array* args;
  void (*thread_func)(C_Thread* self);
  pid_t tid;
};

static s32 futex(u32* uaddr, int futex_op, u32 val,
  const struct timespec* timeout, u32* uaddr2, u32 val3) {
  return syscall(SYS_futex, uaddr, futex_op, val, timeout, uaddr2, val3);
}

Mutex Mutex_construct(void) { return (Mutex){0}; }

void Mutex_lock(Mutex* self) {
  u32 unlocked = 0;
  while (!os_atomic_u32_compare_exchange(&self->ftx, &unlocked, 1)) {
    unlocked = 0;

    s32 s = futex(&self->ftx, FUTEX_WAIT, 1, null, null, 0);

    if (s < -1 && errno != EAGAIN) {
      crash(E(EG_OS_THREADS, E_Unspecified, SV("Mutex_lock -> failed")));
    }
  }
}

void Mutex_unlock(Mutex* self) {
  u32 locked = 1;
  if (os_atomic_u32_compare_exchange(&self->ftx, &locked, 0)) {
    futex(&self->ftx, FUTEX_WAKE, 1, null, null, 0);
  }
}

static int C_Thread_call_func(void* self) {
  C_Thread* self_cast = self;

  os_atomic_u32_store(&self_cast->done, 0);

  self_cast->tid = gettid();
  self_cast->thread_func(self);

  os_atomic_u32_store(&self_cast->done, 1);
  futex(&self_cast->done, FUTEX_WAKE, 1, null, null, 0);

  return 0;
}

C_Thread* C_Thread_new(void (*thread_func)(C_Thread* self), C_Array* args) {
  C_Thread* thread =
    C_Thread_new_stack_size(thread_func, args, OSThreadStackSize);

  return thread;
}

C_Thread* C_Thread_new_stack_size(
  void (*thread_func)(C_Thread* self), C_Array* args, u32 stack_size) {

  C_Thread* self = allocate(sizeof(C_Thread));
  self->base = ClassObject_construct(C_Thread_destroy, null);
  self->args = args;
  self->stack_size = stack_size;
  self->thread_func = thread_func;

  return self;
}

C_EmptyResult* C_Thread_run(C_Thread* self) {
  C_EmptyResult* result;

  MemoryResult stack =
    global_memory_base->reserve(global_memory_base, self->stack_size);
  if (!stack.ok) {
    result = C_EmptyResult_new_err(E(EG_OS_THREADS, E_Unspecified,
      SV("C_Thread_run -> failed to allocate thread stack")));
    goto ret;
  }

  /* commit memory */
  /* linux will still allocate only the pages that are used */
  MemoryResult stack_commit =
    global_memory_base->commit(global_memory_base, stack.ptr, stack.size);
  if (!stack_commit.ok) {
    result = C_EmptyResult_new_err(E(EG_OS_THREADS, E_Unspecified,
      SV("C_Thread_run -> failed to commit allocated thread stack")));
    goto ret;
  }

  self->stack = stack.ptr;
  self->stack_size = stack.size;

  pid_t tid = clone(C_Thread_call_func,
    (void*)mem_align_forward((u64)((u8*)stack.ptr + stack.size), 16),
    CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD |
      CLONE_SYSVSEM,
    self);

  if (tid == -1) {
    result = C_EmptyResult_new_err(E(EG_OS_THREADS, E_Unspecified,
      SV("C_Thread_run -> failed to run new thread")));
    goto ret;
  }

  result = C_EmptyResult_new_ok();

ret:
  return result;
}

void C_Thread_join(C_Thread* self) {
  while (os_atomic_u32_load(&self->done) == 0) {
    futex(&self->done, FUTEX_WAIT, 0, null, null, null);
  }
}

void C_Thread_destroy(void* self) {
  C_Thread* self_cast = self;
  MemoryResult release = global_memory_base->release(
    global_memory_base, self_cast->stack, self_cast->stack_size);
  if (!release.ok) {
    crash(E(EG_OS_THREADS, E_Unspecified,
      SV("C_Thread_destroy -> failed to release threads stack")));
  }
}
