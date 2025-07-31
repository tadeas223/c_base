#ifndef STRINGS_H
#define STRINGS_H

#include "c_base/ds/array.h"
#include <c_base/base/errors/errors.h>
#include <c_base/base/memory/temp.h>
#include <c_base/base/strings/string_view.h>
#include <c_base/base/types.h>

#define S(cstr) C_String_new((ascii*)cstr, sizeof(cstr) - 1)
#define PS(cstr) Pass(S(cstr))

GenericVal_ErrorCode(EG_Strings)

// implements: IFormattable, IHashable
typedef struct C_String C_String;

typedef struct {
  Interface interface;

  C_String* (*to_str_R)(void* self);
  C_String* (*to_str_format_R)(void* self, C_String* format);
} IFormattable;
Id(IFormattable)

/******************************
 * IFormattable
 ******************************/
IFormattable IFormattable_construct_format(
    C_String* (*to_str_R)(void* self),
    C_String* (*to_str_format_R)(void* self, C_String* format));
IFormattable IFormattable_construct(C_String* (*to_str_R)(void* self));

C_String* IFormattable_to_str_PR(void* self);
C_String* IFormattable_to_str_format_PR(void* self, C_String* format);

/******************************
 * new/dest
 ******************************/
C_String* C_String_new(ascii* chars, u32 len);
C_String* C_String_new_view(StringView view);
C_String* C_String_new_empty(u32 len);

void C_String_destroy(void* self);

/******************************
 * logic
 ******************************/
C_String* C_String_to_str_R(void* self);
ascii C_String_at(C_String* self, u32 index);
void C_String_put(C_String* self, u32 index, ascii character);

C_String* C_String_substr_R(C_String* original, u32 index, u32 len);
C_String* C_String_concat_PR(C_String* string, ...);

C_Array* /* C_String* */ C_String_split_R(C_String* string, ascii splitter);
C_String* C_String_join_PR(C_Array* /* C_String* */ strings);

/******************************
 * get/set
 ******************************/
ascii* C_String_get_chars(C_String* self);
u32 C_String_get_len(C_String* self);

#endif
