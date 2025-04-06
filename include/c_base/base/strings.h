#ifndef STRINGS_H
#define STRINGS_H

#include "base/ds/list.h"
#include "base/errors.h"
#include "base/varargs.h"

Extern_ErrorGroup(EG_Strings);

GenericType_Result(String);
GenericType_Result(StringAlloc);

GenericType_List(String);
GenericType_List(StringAlloc);

ExternGenericImpl_List(String);
ExternGenericImpl_List(StringAlloc);

#define Stack_String(name, len) u8 __##name[len]; StringStack name = str_stack((len), (u8*)&__##name)
#define Stack_StringCopy(name, original) u8 __##name[(original).len]; StringStack name = str_stack((original).len, (u8*)&__##name); str_copy2(name.string, (original))

StringAlloc str_allocate(Arena *arena, u32 len);

StringStack str_stack(u32 len, u8* buffer);

u8 str_at(String string, u32 index);

void str_set(String string ,u32 index, u8 c);

StringAlloc str_copy(Arena *arena, String original);
void str_copy2(String dest, String src);

bool str_equals(String a, String b);

Result(u32) str_find_char(String string, u8 c);

char* str_to_cstr(Arena *arena, String string);
String str_from_cstr(char* cstr);

String str_substr(String original, u32 index, u32 len);

List(String) str_split(Arena *arena, String original, u8 splitter);
StringAlloc str_join(Arena *arena, List(String) list);
StringAlloc str_glue(Arena *arena, /* String */ Varargs);
StringAlloc str_glue_varlist(Arena *arena, /* String */ Varlist);


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

#define GenericFunc_ToStr(type, arena, value) StringAlloc Concat(type, _to_str)(Arena *arena, type value)
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
