#include "base/errors/errors.h"
#include "base/errors/results.h"
#include "base/memory/objects.h"
#include "os/os_io.h"

#include <fcntl.h>
#include <unistd.h>

C_Result* /* u32 */ console_read_chars_R(void* chars, u32 len) {
  int read_result = read(0, chars, len);
  if(read_result < 0) {
    return C_Result_new_err(E(EG_OS_IO, E_Unspecified, SV("console_read_chars -> failed to read from console")));
  } else {
    return C_Result_new_ok(Wrap(u32, read_result));
  }
}

C_Result* /* u32 */ console_write_chars_R(void* chars, u32 len) {
  int write_result = write(1, chars, len);
  if(write_result < 0) {
    return C_Result_new_err(E(EG_OS_IO, E_Unspecified, SV("console_write_chars -> failed to write to console")));
  } else {
    return C_Result_new_ok(Wrap(u32, write_result));
  }
}
