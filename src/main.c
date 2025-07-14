#include "base/errors/results.h"
#include "base/math.h"
#include "base/memory/objects.h"
#include "base/strings/string_convert.h"
#include "os/os_io.h"

int main(void) {
  Using(C_String*, S("num1: "), {
    console_write(value);
  });
  
  C_String* s = console_read_ln_R();
  C_Result* r = s32_parse_R(s);
  u32 num1 = Unwrap(s32, C_Result_force_B(r));
  unref(s);
  unref(r);
  
  Using(C_String*, S("num2: "), {
    console_write(value);
  });

  s = console_read_ln_R();
  r = s32_parse_R(s);
  u32 num2 = Unwrap(s32, C_Result_force_B(r));
  unref(s);
  unref(r);
  
  Using(C_String*, S("result: "), {
    console_write(value);
  });
  
  C_Result* r_overflow = s32_add_safe_R(num1, num2);
  C_String* r_text = s32_to_str_R(Unwrap(s32, C_Result_force_B(r_overflow)));
  console_write_ln(r_text);
  unref(r_text);
  unref(r_overflow);

  return 0;
}
