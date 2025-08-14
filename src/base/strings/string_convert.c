#include <c_base/base/errors/errors.h>
#include <c_base/base/math.h>
#include <c_base/base/memory/allocator.h>
#include <c_base/base/memory/handles.h>
#include <c_base/base/memory/objects.h>
#include <c_base/base/strings/string_convert.h>
#include <c_base/base/strings/strings.h>
#include <c_base/system.h>

ascii digit_to_hex(u8 d) { return (d < 10) ? ('0' + d) : ('a' + (d - 10)); }

#define UToStr(T)                                                              \
  C_String* Concat(T, _to_str_R)(T x) {                                        \
    s32 len = math_simple_log(x, 10) + ((x == 0) ? 1 : 0);                     \
                                                                               \
    C_String* result = C_String_new_empty(len);                                \
                                                                               \
    for (s32 i = len - 1; i >= 0; i--) {                                       \
      u8 num = x % 10;                                                         \
      C_String_put(result, i, num + '0');                                      \
      x /= 10;                                                                 \
    }                                                                          \
                                                                               \
    return result;                                                             \
  }

UToStr(u8)
UToStr(u16)
UToStr(u32)
UToStr(u64)

#define SToStr(T)                                                              \
  C_String* Concat(T, _to_str_R)(T x) {                                        \
    bool sign = x < 0;                                                         \
                                                                               \
    if (sign) {                                                                \
      x *= -1;                                                                 \
    }                                                                          \
                                                                               \
    s32 len = math_simple_log(x, 10) + sign + ((x == 0) ? 1 : 0);              \
    C_String* result = C_String_new_empty(len);                                \
                                                                               \
    if (sign) {                                                                \
      C_String_put(result, 0, '-');                                            \
    }                                                                          \
                                                                               \
    for (s32 i = len - 1; i >= sign; i--) {                                    \
      u8 num = x % 10;                                                         \
      C_String_put(result, i, num + '0');                                      \
      x /= 10;                                                                 \
    }                                                                          \
                                                                               \
    return result;                                                             \
  }

SToStr(s8)
SToStr(s16)
SToStr(s32)
SToStr(s64)

/* AI generated :(*/
C_String* ptr_to_str_R(void* ptr) {
  if (ptr == null) {
    return S("null");
  }

  u64 value = (u64)ptr;

  u32 max_digits = sizeof(void*) * 2;
  u32 buf_len = 2 + max_digits;

  ascii* chars = allocate(buf_len);
  chars[0] = '0';
  chars[1] = 'x';

  u32 i = 0;
  for (; value != 0; ++i) {
    u8 digit = value % 16;
    chars[2 + i] = digit_to_hex(digit);
    value /= 16;
  }

  for (u32 j = 0; j < i / 2; ++j) {
    ascii temp = chars[2 + j];
    chars[2 + j] = chars[2 + i - 1 - j];
    chars[2 + i - 1 - j] = temp;
  }

  return C_String_new(chars, 2 + i);
}
C_Result* /* u32 */ u32_parse_PR(C_String* string) {
  Ref(string);
  C_Result* result;
  u32 result_value = 0;
  u32 start_index = 0;

  if (C_String_at(string, 0) == '+') {
    start_index++;
  }

  for (u32 i = start_index; i < C_String_get_len(string); i++) {
    u32 c = C_String_at(string, i);

    if (c < '0' || c > '9') {
      result = C_Result_new_err(E(EG_Strings, E_InvalidArgument,
        SV("u32_parse -> string cannot be parsed to u32")));
      goto ret;
    }

    C_Result* math_result = u32_mult_safe_R(result_value, 10);
    if (!C_Result_get_ok(math_result)) {
      result = math_result;
      goto ret;
    }

    result_value = C_Handle_u32_get_value(C_Result_force_B(math_result));
    Unref(math_result);

    math_result = u32_add_safe_R(result_value, c - '0');
    if (!C_Result_get_ok(math_result)) {
      result = math_result;
      goto ret;
    }

    result_value = C_Handle_u32_get_value(C_Result_force_B(math_result));
    Unref(math_result);
  }

  C_Handle_u32* handle = C_Handle_u32_new(result_value);
  result = C_Result_new_ok_P(handle);
  Unref(handle);

ret:
  Unref(string);
  return result;
}

C_Result* /* s32 */ s32_parse_R(C_String* string) {
  Ref(string);
  C_Result* result;
  s32 result_value = 0;
  u32 start_index = 0;

  bool sign;
  if (C_String_at(string, 0) == '+') {
    sign = false;
    start_index = 1;
  } else if (C_String_at(string, 0) == '-') {
    sign = true;
    start_index = 1;
  }

  for (u32 i = start_index; i < C_String_get_len(string); i++) {
    u32 c = C_String_at(string, i);

    if (c < '0' || c > '9') {
      result = C_Result_new_err(E(EG_Strings, E_InvalidArgument,
        SV("u32_parse -> string cannot be parsed to u32")));
      goto ret;
    }

    C_Result* math_result = s32_mult_safe_R(result_value, 10);
    if (!C_Result_get_ok(math_result)) {
      result = math_result;
      goto ret;
    }

    result_value = C_Handle_s32_get_value(C_Result_force_B(math_result));
    Unref(math_result);

    math_result = s32_add_safe_R(result_value, c - '0');
    if (!C_Result_get_ok(math_result)) {
      result = math_result;
    }

    result_value = C_Handle_s32_get_value(C_Result_force_B(math_result));
    Unref(math_result);
  }

  if (sign) {
    C_Result* sign_result = s32_mult_safe_R(result_value, -1);
    if (!C_Result_get_ok(sign_result)) {
      return sign_result;
    }

    result_value = C_Handle_s32_get_value(C_Result_force_B(sign_result));
    Unref(sign_result);
  }

  C_Handle_s32* handle = C_Handle_s32_new(result_value);
  result = C_Result_new_ok_P(handle);
  Unref(handle);

ret:
  Unref(string);
  return result;
}

#define BToStr(T, bits)                                                        \
  C_String* Concat(T, _to_str_R)(T x) {                                        \
    C_String* result = C_String_new_empty(bits);                               \
                                                                               \
    for (u32 i = 0; i < bits; i++) {                                           \
      T bit = (x >> i) & 1;                                                    \
      if (bit) {                                                               \
        C_String_put(result, i, '1');                                          \
      } else {                                                                 \
        C_String_put(result, i, '0');                                          \
      }                                                                        \
    }                                                                          \
                                                                               \
    return result;                                                             \
  }

BToStr(b8, 8)
BToStr(b16, 16)
BToStr(b32, 32)
BToStr(b64, 64)

C_String* bool_to_str_R(bool x) {
  if (x) {
    return S("true");
  }
  return S("false");
}

#define ToStrFormatWrap(T)                                                     \
  C_String* Concat(T, _to_str_format_R)(T x, C_String * format) {              \
    (void)format;                                                              \
    return Concat(T, _to_str_R)(x);                                            \
  }

ToStrFormatWrap(u8)
ToStrFormatWrap(u16)
ToStrFormatWrap(u32)
ToStrFormatWrap(u64)

ToStrFormatWrap(s8)
ToStrFormatWrap(s16)
ToStrFormatWrap(s32)
ToStrFormatWrap(s64)

ToStrFormatWrap(b8)
ToStrFormatWrap(b16)
ToStrFormatWrap(b32)
ToStrFormatWrap(b64)

ToStrFormatWrap(ptr)

ToStrFormatWrap(bool)
