#include "c_base/base/errors/C_EmptyResult.h"
#include "c_base/base/errors/C_Result.h"
#include "c_base/base/errors/errors_base.h"
#include "c_base/base/memory/allocator.h"
#include "c_base/base/memory/memory.h"
#include "c_base/base/memory/memory_base.h"
#include "c_base/base/memory/objects.h"
#include "c_base/base/types.h"
#include <c_base/os/os_threads.h>

#define _GNU_SOURCE
#include <fcntl.h>
#include <sched.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <unistd.h>

GenericValImpl_ErrorCode(EG_OS_THREADS)

struct C_Thread {
  ClassObject base;

  u64 stack_size;
  C_Array* args;
  void (*thread_func)(C_Thread* self);
  bool running;
  pid_t tid;
};

static int C_Thread_call_func(void* self) {
  C_Thread* self_cast = self;

  self_cast->tid = gettid();

  self_cast->thread_func(self);
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
  self->running = false;

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

void C_Thread_destroy(void* self) { (void)self; }
