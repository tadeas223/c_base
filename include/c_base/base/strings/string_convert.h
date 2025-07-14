#ifndef STRING_CONVERT_H
#define STRING_CONVERT_H

#include "base/errors/results.h"
#include "base/strings/strings.h"

C_String* u32_to_str_R(u32 x);
C_String* s32_to_str_R(s32 x);

C_Result* /* u32 */ u32_parse_R(C_String* string);
C_Result* /* s32 */ s32_parse_R(C_String* string);

#endif
