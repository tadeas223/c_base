#include "c_base/base/errors/C_EmptyResult.h"
#include "c_base/base/errors/C_Result.h"
#include "c_base/base/memory/allocator.h"
#include "c_base/base/memory/objects.h"
#include "c_base/base/strings/format.h"
#include "c_base/base/strings/string_convert.h"
#include "c_base/base/strings/strings.h"
#include "c_base/base/varargs.h"
#include "c_base/ds/C_Array.h"
#include "c_base/ds/C_List.h"
#include "c_base/os/os_io.h"
#include "c_base/os/os_threads.h"
#include <stdio.h>

void thread_func(C_Thread* self) {
  (void)self;

  printf("aa");
}

int main(void) {
  C_Thread* thread = C_Thread_new(thread_func, null);

  C_EmptyResult* result = C_Thread_run(thread);
  (void)result;
  return 0;
}
