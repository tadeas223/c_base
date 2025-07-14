#include "base/strings/string_convert.h"
#include "base/errors/errors.h"
#include "base/errors/results.h"
#include "base/math.h"
#include "base/memory/objects.h"
#include "base/strings/strings.h"
#include "system.h"

C_String* u32_to_str_R(u32 x) {
  u32 len = math_simple_log(x, 10) + ((x == 0)? 1 : 0);
  
  C_String* result = C_String_new_empty(len);

  for(s32 i = len-1; i >= 0; i--) {
    u8 num = x % 10;
    C_String_put(result, i, num + '0');
    x /= 10;
  }

  return result;
}

C_String* s32_to_str_R(s32 x) {
  bool sign = x < 0;

  if(sign) {
    x *= -1; 
  }

  u32 len = math_simple_log(x, 10) + sign + ((x == 0)? 1 : 0);
  C_String* result = C_String_new_empty(len);

  if(sign) {
    C_String_put(result, 0, '-'); 
  }

  for(s32 i = len-1; i >= sign; i--) {
    u8 num = x % 10;
    C_String_put(result, i, num + '0');
    x /= 10;
  }

  return result;
}

C_Result* /* u32 */ u32_parse_R(C_String* string) {
  C_Result* result; 
  u32 result_value = 0;
  u32 start_index = 0;

  if(C_String_at(string, 0) == '+') {
    start_index++; 
  }

  for(u32 i = start_index; i < C_String_get_len(string); i++) {
    u32 c = C_String_at(string, i);

    if(c < '0' || c > '9') {
      crash(E(EG_Strings, E_InvalidArgument, SV("u32_parse -> string cannot be parsed to u32"))); 
    }

    C_Result* math_result = u32_mult_safe_R(result_value, 10);
    if(!C_Result_get_ok(math_result)) {
      result = math_result;
      goto ret;
    }
    
    result_value = Unwrap(u32, C_Result_force_B(math_result));
    unref(math_result);
    
    math_result = u32_add_safe_R(result_value, c - '0');
    if(!C_Result_get_ok(math_result)) {
      result = math_result;
      goto ret;
    }
    
    result_value = Unwrap(u32, C_Result_force_B(math_result));
    unref(math_result);

  }
  
  result = C_Result_new_ok(Wrap(u32, result_value));

ret:
  return result;
}

C_Result* /* s32 */ s32_parse_R(C_String* string) {
  C_Result* result; 
  s32 result_value = 0;
  u32 start_index = 0;
  
  bool sign;
  if(C_String_at(string, 0) == '+') {
    sign = false;
    start_index = 1;
  } else if(C_String_at(string, 0) == '-') {
    sign = true;
    start_index = 1;
  }

  for(u32 i = start_index; i < C_String_get_len(string); i++) {
    u32 c = C_String_at(string, i);

    if(c < '0' || c > '9') {
      crash(E(EG_Strings, E_InvalidArgument, SV("u32_parse -> string cannot be parsed to u32"))); 
    }

    C_Result* math_result = s32_mult_safe_R(result_value, 10);
    if(!C_Result_get_ok(math_result)) {
      result = math_result; 
      goto ret; 
    }
    
    result_value = Unwrap(s32, C_Result_force_B(math_result));
    unref(math_result);
    
    math_result = s32_add_safe_R(result_value, c - '0');
    if(!C_Result_get_ok(math_result)) {
      result = math_result; 
    }
    
    result_value = Unwrap(s32, C_Result_force_B(math_result));
    unref(math_result);

  }
  
  if(sign) {
    C_Result* sign_result = s32_mult_safe_R(result_value, -1);
    if(!C_Result_get_ok(sign_result)) {
      return sign_result; 
    }
    
    result_value = Unwrap(s32, C_Result_force_B(sign_result));
    unref(sign_result);
  }

  result = C_Result_new_ok(Wrap(s32, result_value));
  
ret:
  return result;
}
