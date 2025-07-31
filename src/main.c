#include "c_base/base/memory/handles.h"
#include "c_base/base/memory/objects.h"
#include "c_base/base/strings/strings.h"
#include "c_base/base/varargs.h"
#include "c_base/ds/darray.h"
#include "c_base/os/os_io.h"

int main(void) {
  C_String* text = S("hello, this, will, be, split");

  C_Array* split = C_String_split_R(text, ',');
  C_String* join = C_String_join_PR(Pass(split));

  console_write_ln_P(PS("text = "), Pass(text), ArgsEnd);
  console_write_ln_P(PS("join = "), Pass(join), ArgsEnd);
}
