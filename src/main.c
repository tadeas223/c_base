#include "c_base/base/errors/results.h"
#include "c_base/base/memory/handles.h"
#include "c_base/base/memory/objects.h"
#include "c_base/base/strings/string_convert.h"
#include "c_base/base/strings/strings.h"
#include "c_base/base/varargs.h"
#include "c_base/ds/C_Array.h"
#include "c_base/os/os_io.h"

int main(int argc, char* argv[]) {
  C_Array* args;
  ArgsLoad(args, argc, argv);

  u32 sum = 0;
  C_ArrayForeach(args, {
    if (iter == 0)
      continue;
    C_Result* r_parse = u32_parse_PR(value);
    sum += C_Handle_u32_get_value(C_Result_force_B(r_parse));
    Unref(r_parse);
  });

  C_List* list = C_List_new();

  console_write_ln_P(
      PS("args: "),
      Pass(IFormattable_to_str_format_PR(list, PS("start={;end=};sep=, "))),
      ArgsEnd);

  console_write_ln_P(PS("sum: "), Pass(u32_to_str_R(sum)), ArgsEnd);

  Unref(args);
  Unref(list);
  return 0;
}
