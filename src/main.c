#include "c_base/base/errors/results.h"
#include "c_base/base/memory/handles.h"
#include "c_base/base/memory/objects.h"
#include "c_base/base/strings/string_convert.h"
#include "c_base/base/strings/strings.h"
#include "c_base/base/varargs.h"
#include "c_base/ds/C_Array.h"
#include "c_base/os/os_io.h"

int main(void) {
  console_set_raw_node(true);

  while (true) {
    C_String* input = console_read_until_R('b');
    console_write_single_ln_P(PS("b"));
    Unref(input);
  }

  console_set_raw_node(false);
  return 0;
}
