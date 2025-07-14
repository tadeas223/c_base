#include "system.h"
#include "os/os_io.h"

void crash(Error err) {
  volatile u8* ptr = null;

  console_write_ln(C_String_new_view(err.msg));

  *ptr = null;
}
