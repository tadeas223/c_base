#include "c_base/base/memory/objects.h"
#include "c_base/base/strings/string_convert.h"
#include "c_base/base/strings/strings.h"
#include "c_base/base/varargs.h"
#include "c_base/ds/C_List.h"
#include "c_base/os/os_io.h"

int main(void) {
  C_List* list = C_List_new();

  bool end = false;
  while (!end) {
    C_String* input = console_read_ln_R();

    if (C_String_equals(input, C_StringEmpty)) {
      end = true;
      goto loop_end;
    } else {
      C_List_push_P(list, input);
    }

  loop_end:
    Unref(input);
  }

  C_String* join = C_String_join_PR(Pass(C_List_to_array_PR(list)));

  console_write_single_ln_P(Pass(join));

  Unref(list);

  console_write_ln_P(PS("refs: "), Pass(u32_to_str_R(refs)), ArgsEnd);
}
