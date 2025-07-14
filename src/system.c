#include <c_base/system.h>
#include <c_base/os/os_io.h>

void crash(Error err) {
  volatile u8* ptr = null;

  console_write_ln(C_String_new_view(err.msg));

  *ptr = null;
}
