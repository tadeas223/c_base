#ifndef STRINGS_H
#define STRINGS_H

#include "base/strings/string_view.h"
#include "base/memory/temp.h"
#include "base/errors/errors.h"

#define S(cstr) C_String_new((u8*)cstr, sizeof(cstr) - 1)
#define TS(cstr) temp(S(cstr))

GenericVal_ErrorCode(EG_Strings)

typedef struct C_String C_String;

/******************************
 * new/dest
 ******************************/
C_String* C_String_new(u8* chars, u32 len);
C_String* C_String_new_view(StringView view);
C_String* C_String_new_empty(u32 len);

void C_String_destroy(void* self);

/******************************
 * logic
 ******************************/
u8 C_String_at(C_String* self, u32 index);
void C_String_put(C_String* self, u32 index, u8 character);

C_String* C_String_substr_R(C_String* original, u32 index, u32 len);
C_String* C_String_concat_R(C_String* a, C_String* b);

/******************************
 * get/set
 ******************************/
u8* C_String_get_chars(C_String* self);
u32 C_String_get_len(C_String* self);

#endif
