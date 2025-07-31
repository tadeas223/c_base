#include <c_base/os/os_io.h>
#include <c_base/system.h>

void crash(Error err) {
  volatile u8* ptr = null;

  console_write_single_ln_P(Pass(C_String_new_view(err.msg)));

  *ptr = null;
}
