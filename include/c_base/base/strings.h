#ifndef STRINGS_H
#define STRINGS_H

#include "ds/list.h"
#include "base/errors.h"
#include "base/varargs.h"

Extern_ErrorGroup(EG_Strings);

GenericType_Result(String);
GenericType_Result(StringMut);

GenericType_List(String);
GenericType_List(StringMut);

ExternGenericImpl_List(String);
ExternGenericImpl_List(StringMut);
#define Stack_String(name, len) u8 __##name[len]; StringMut name = str_mut_create((len), (u8*)&__##name)
#define Stack_StringCopy(name, original) Stack_String(name, (original).len); str_copy_to(name, (original))

StringMut str_allocate(Arena *arena, u32 len);

StringMut str_mut_create(u32 len, u8* chars);

/* tested */ u8 str_at(String str, u32 index);

/* tested */ void str_set(StringMut mut_str, u32 index, u8 c);

/* tested */ StringMut str_copy(Arena *arena, String original);
/* tested */ void str_copy_to(StringMut dest, String src);

/* tested */ bool str_equals(String a, String b);

/* tested */ Result(u32) str_find_char(String str, u8 c);

/* tested */ char* str_to_cstr(Arena *arena, String string);
/* tested */ String str_from_cstr(char* cstr);

/* tested */ String str_substr(String original, u32 index, u32 len);

List(String) str_split(Arena *arena, String original, u8 splitter);
String str_join(Arena *arena, List(String) list);
String str_glue(Arena *arena, /* String */ Varargs);
String str_glue_varlist(Arena *arena, /* String */ Varlist);

bool is_number(u8 c);

Result(u8) char_to_num(u8 c);

#define GenericFunc_Parse(type, str) Result(type) Concat(type, _parse)(String str)
#define Parse(type, string) Concat(type, _parse)((string))
#define TryParse(type, string) TryCrash(type, Concat(type, _parse)((string)))

GenericFunc_Parse(u8,  str);
GenericFunc_Parse(u16, str);
GenericFunc_Parse(u32, str);
GenericFunc_Parse(u64, str);

GenericFunc_Parse(s8,  str);
GenericFunc_Parse(s16, str);
GenericFunc_Parse(s32, str);
GenericFunc_Parse(s64, str);

GenericFunc_Parse(bool, str);

#define GenericFunc_ToStr(type, arena, value) String Concat(type, _to_str)(Arena *arena, type value)
#define ToStr(type, arena, value) Concat(type, _to_str)((arena), (value))

GenericFunc_ToStr(u8,  arena, value);
GenericFunc_ToStr(u16, arena, value);
GenericFunc_ToStr(u32, arena, value);
GenericFunc_ToStr(u64, arena, value);

GenericFunc_ToStr(s8,  arena, value);
GenericFunc_ToStr(s16, arena, value);
GenericFunc_ToStr(s32, arena, value);
GenericFunc_ToStr(s64, arena, value);

GenericFunc_ToStr(bool, arena, value);

#endif
