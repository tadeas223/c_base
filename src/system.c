#include "c_base/base/varargs.h"
#include <c_base/os/os_io.h>
#include <c_base/system.h>

void crash(Error err) {
  volatile u8* ptr = null;

  console_write_single_ln_P(PS("=====ERROR====="));
  console_write_ln_P(PS("group code: "),
                     Pass(C_String_new_view(*(StringView*)err.grp_code)),
                     ArgsEnd);
  console_write_ln_P(PS("code:       "),
                     Pass(C_String_new_view(*(StringView*)err.code)), ArgsEnd);
  console_write_ln_P(PS("message:    "), Pass(C_String_new_view(err.msg)),
                     ArgsEnd);

  *ptr = null;
}
