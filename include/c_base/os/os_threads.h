#ifndef OS_THREADS_H
#define OS_THREADS_H

#include "c_base/base/errors/C_EmptyResult.h"
#include "c_base/base/types.h"

typedef struct C_Thread C_Thread;
typedef struct C_Mutex C_Mutex;

C_Thread* C_Thread_new(void);
C_Thread* C_Thread_new_stack_size(u32 stack_size);

void C_Thread_join(C_Thread* self);

C_EmptyResult* C_Thread_close(C_Thread* self);
void C_Thread_destroy(void* self);

#endif
