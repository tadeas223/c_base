#include "c_base/base/errors/C_EmptyResult.h"
#include "c_base/base/memory/objects.h"
#include "c_base/base/strings/string_convert.h"
#include "c_base/base/types.h"
#include "c_base/os/os_io.h"
#include "c_base/os/os_threads.h"

u64 counter = 0;
Mutex m = MutexConstructStatic;

void thread(C_Thread* self) {
  Mutex_lock(&m);
  (void)self;

  for (u32 i = 0; i < 100000; i++) {
    counter++;
  }
  Mutex_unlock(&m);
}

int main(void) {
  C_Thread* t1 = C_Thread_new(thread, null);
  C_Thread* t2 = C_Thread_new(thread, null);

  C_EmptyResult* result = C_Thread_run(t1);
  C_EmptyResult_force(result);

  C_EmptyResult* result2 = C_Thread_run(t2);
  C_EmptyResult_force(result2);

  C_Thread_join(t1);
  C_Thread_join(t2);

  console_write_single_ln_P(Pass(u64_to_str_R(counter)));

  Unref(t1);
  Unref(t2);
  Unref(result);
  Unref(result2);
  return 0;
}
