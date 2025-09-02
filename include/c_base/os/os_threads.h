#ifndef OS_THREADS_H
#define OS_THREADS_H

#include "c_base/base/errors/C_EmptyResult.h"
#include "c_base/base/macros.h"
#include "c_base/base/types.h"
#include "c_base/ds/C_Array.h"

GenericVal_ErrorCode(EG_OS_THREADS)

typedef struct C_Thread C_Thread;

typedef struct {
  u32 ftx;
} Mutex;

#define OSThreadStackSize Megabytes(8)

Mutex Mutex_construct(void);
void Mutex_lock(Mutex* self);
void Mutex_unlock(Mutex* self);

C_Thread* C_Thread_new(void (*thread_func)(C_Thread* self), C_Array* args);

C_Thread* C_Thread_new_stack_size(
  void (*thread_fun)(C_Thread* self), C_Array* args, u32 stack_size);

C_EmptyResult* C_Thread_run(C_Thread* self);
void C_Thread_join(C_Thread* self);

void C_Thread_join(C_Thread* self);

C_EmptyResult* C_Thread_close(C_Thread* self);
void C_Thread_destroy(void* self);

#endif
