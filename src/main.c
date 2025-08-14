#include "c_base/base/memory/handles.h"
#include "c_base/base/memory/objects.h"
#include "c_base/base/strings/strings.h"
#include "c_base/base/varargs.h"
#include "c_base/ds/C_Array.h"
#include "c_base/os/os_io.h"

u32 sum_P(C_Handle_u32* x, ...) {
  C_Array* args;
  VarargsLoad(args, x);

  u32 sum = 0;
  C_ArrayForeach(args, { sum += C_Handle_s32_get_value(value); });

  Unref(args);
  return sum;
}

int main(void) {
  u32 x =
    sum_P(Pass(C_Handle_u32_new(10)), Pass(C_Handle_u32_new(20)), ArgsEnd);

  console_write_ln_P(
    PS("x = "), Pass(u32_to_str_R(x)), PS("\nnull: "), null, ArgsEnd);

  return 0;
}
