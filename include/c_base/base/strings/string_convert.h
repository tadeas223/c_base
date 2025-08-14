#ifndef STRING_CONVERT_H
#define STRING_CONVERT_H

#include <c_base/base/errors/errors.h>
#include <c_base/base/strings/strings.h>

ascii digit_to_hex(u8 digit);

C_String* u8_to_str_R(u8 x);
C_String* u16_to_str_R(u16 x);
C_String* u32_to_str_R(u32 x);
C_String* u64_to_str_R(u64 x);

C_String* s8_to_str_R(s8 x);
C_String* s16_to_str_R(s16 x);
C_String* s32_to_str_R(s32 x);
C_String* s64_to_str_R(s64 x);

C_String* b8_to_str_R(b8 x);
C_String* b16_to_str_R(b16 x);
C_String* b32_to_str_R(b32 x);
C_String* b64_to_str_R(b64 x);

C_String* ptr_to_str_R(void* ptr);

C_String* bool_to_str_R(bool x);

/* these are just wrappers, they dont format shit!!! */
C_String* u8_to_str_format_R(u8 x, C_String* format);
C_String* u16_to_str_format_R(u16 x, C_String* format);
C_String* u32_to_str_format_R(u32 x, C_String* format);
C_String* u64_to_str_format_R(u64 x, C_String* format);

C_String* s8_to_str_format_R(s8 x, C_String* format);
C_String* s16_to_str_format_R(s16 x, C_String* format);
C_String* s32_to_str_format_R(s32 x, C_String* format);
C_String* s64_to_str_format_R(s64 x, C_String* format);

C_String* b8_to_str_format_R(b8 x, C_String* format);
C_String* b16_to_str_format_R(b16 x, C_String* format);
C_String* b32_to_str_format_R(b32 x, C_String* format);
C_String* b64_to_str_format_R(b64 x, C_String* format);

C_String* ptr_to_str_format_R(void* ptr, C_String* format);

C_String* bool_to_str_format_R(bool x, C_String* format);

C_Result* /* u32 */ u32_parse_PR(C_String* string);
C_Result* /* s32 */ s32_parse_PR(C_String* string);

#endif
