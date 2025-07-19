#include <c_base/base/errors/errors.h>
#include <c_base/base/errors/results.h>
#include <c_base/base/memory/objects.h>
#include <c_base/os/os_io.h>

#include <fcntl.h>
#include <unistd.h>

C_Result* /* u32 */ console_read_chars_R(void* chars, u32 len) {
  C_Result* result; 
  int read_result = read(0, chars, len);
  if(read_result < 0) {
    result = C_Result_new_err(E(EG_OS_IO, E_Unspecified, SV("console_read_chars -> failed to read from console")));
    goto ret; 
  } else {
    C_Handle_u32* handle = C_Handle_u32_new(read_result);
    result = C_Result_new_ok_P(handle);
    Unref(handle);

    goto ret; 
  }

ret:
  return result;
}

C_Result* /* u32 */ console_write_chars_R(void* chars, u32 len) {
  int write_result = write(1, chars, len);
  C_Result* result; 
  if(write_result < 0) {
    result = C_Result_new_err(E(EG_OS_IO, E_Unspecified, SV("console_write_chars -> failed to write to console")));
    goto ret;
  } else {
    C_Handle_u32* handle = C_Handle_u32_new(write_result);
    result = C_Result_new_ok_P(handle);
    Unref(handle);
    
    goto ret;
  }

ret:
  return result;
}
