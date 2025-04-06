#include "base/strings.h"
#include "base/arena.h"
#include "base/ds/list.h"
#include "base/errors.h"
#include "base/mem.h"
#include "base/strdef.h"
#include "base/types.h"
#include "base/varargs.h"
#include "base/arithmetics.h"

Define_ErrorGroup(EG_Strings);

GenericImpl_List(String);
GenericImpl_List(StringAlloc);


StringAlloc str_allocate(Arena *arena, u32 len) {
    u8* chars = arena_alloc(arena, len);
    return (StringAlloc) {.string = (String) {.chars = chars, .len = len} };
}

StringStack str_stack(u32 len, u8 *buffer) {
    return (StringStack) {.string = {.chars = buffer, .len = len}};
}

u8 str_at(String string, u32 index) {
    return string.chars[index];
}

void str_set(String string ,u32 index, u8 c) {
    string.chars[index] = c;
}

StringAlloc str_copy(Arena *arena, String original) {
    StringAlloc a_str = str_allocate(arena, original.len);
    mem_copy(a_str.string.chars, original.chars, original.len);
    return a_str;
}

void str_copy2(String dest, String src) {
    if(src.len >= dest.len) {
        mem_copy(dest.chars, src.chars, dest.len);  
    } else {
        mem_copy(dest.chars, src.chars, src.len);  
    }
}

bool str_equals(String a, String b) {
    if(a.len != b.len) return false;

    for(u32 i = 0; i < a.len; i++) {
        if(str_at(a, i) != str_at(b, i)) {
            return false;  
        }
    }

    return true;
}

Result(u32) str_find_char(String string, u8 c) {
    for(u32 i = 0; i < string.len; i++) {
        if(str_at(string, i) == c) {
            return ResultOk(u32, i); 
        }
    }
    return ResultErr(u32, E(EG_Strings, E_OutOfBounds, S("Character not found")));
}

char* str_to_cstr(Arena *arena, String string) {
    char* cstr = arena_alloc(arena, string.len + 1);
    cstr[string.len] = '\0';

    mem_copy(cstr, string.chars, string.len);
    return cstr;
}

String str_from_cstr(char* cstr) {
    u32 len = 0;
    while(cstr[len] != '\0') {
        len++; 
    }

    return (String) {.chars = (u8*)cstr, .len = len};
}

String str_substr(String original, u32 index, u32 len) {
    return (String) {.chars = original.chars + index, .len = len}; 
}

List(String) str_split(Arena *arena, String original, u8 splitter) {
    List(String) list;
    ListInit(String, &list);
    
    u32 splitStart = 0;

    for(u32 i = 0; i < original.len; i++) {
        if(str_at(original, i) == splitter) {
            String substr = str_substr(original, splitStart, i - splitStart);
            ListPush(String, arena, &list, substr);
            splitStart = i + 1;
        }
    }

    // insert the last substring
    String substr = str_substr(original, splitStart, original.len - splitStart);
    ListPush(String, arena, &list, substr);

    return list;
}

StringAlloc str_join(Arena *arena, List(String) list) {
    u32 len = 0;
    ListForeach(String, &list, {
        len += value.len;
    });
    
    StringAlloc a_result = str_allocate(arena, len);

    u32 pos = 0;
    ListForeach(String, &list, {
        mem_copy(a_result.string.chars + pos, value.chars, value.len);
        pos += value.len;
    });

    return a_result;
}

StringAlloc str_glue(Arena *arena, /* String */ Varargs) {
    List(String) list;
    ListInit(String, &list);
    
    VarargsBlock({
        VarargsForeach(String, {
            ListPush(String, arena, &list, value); 
        });
    });
    
    return str_join(arena, list);
}

StringAlloc str_glue_varlist(Arena *arena, /* String */ Varlist) {
    List(String) list;
    ListInit(String, &list);
    
    VarlistForeach(String, {
        ListPush(String, arena, &list, value); 
    });
    
    return str_join(arena, list);
}

bool is_number(u8 c) {
    return (c >= '0' || c <= '9');
}

Result(u8) char_to_num(u8 c) {
    if(c >= '0' && c <= '9') {
        return ResultOk(u8, c - '0');
    }
    return ResultErr(u8, E(EG_Strings, E_InvalidArgument, S("character does not represent a number")));
}

Result(u8) num_to_char(u8 num) {
    if(num >= 0 && num <= 9) {
        return ResultOk(u8, num + '0');
    }
    return ResultErr(u8, E(EG_Strings, E_InvalidArgument, S("number is not single digit")));
}

#define __TemplateStrParseUnsigned(type)                                                                                 \
GenericFunc_Parse(type, string) {                                                                                        \
    type num = 0;                                                                                                        \
    u64 i = 0;                                                                                                           \
                                                                                                                         \
    if(str_at(string, 0) == '+') {                                                                                       \
        i++;                                                                                                             \
    }                                                                                                                    \
                                                                                                                         \
    for(; i < string.len; i++) {                                                                                         \
        u8 c = str_at(string, i);                                                                                        \
        if(!is_number(c)) {                                                                                              \
            return ResultErr(type, E(EG_Strings, E_InvalidArgument, S("string cannot be parsed to number"))); \
        }                                                                                                                \
        u8 c_num = TryReturn(type, u8, char_to_num(c));                                                                  \
        num = TryReturn(type, type, MultSafe(type, num, 10));                                                            \
        num = TryReturn(type, type, AddSafe(type, num, c_num));                                                          \
    }                                                                                                                    \
    return ResultOk(type, num);                                                                                          \
}

#define __TemplateStrParseSigned(type)                                                                                   \
GenericFunc_Parse(type, string) {                                                                                        \
    bool negative = false;                                                                                               \
    type num = 0;                                                                                                        \
    u64 i = 0;                                                                                                           \
                                                                                                                         \
    if(str_at(string, 0) == '+') {                                                                                       \
        i++;                                                                                                             \
    } else if(str_at(string, 0) == '-') {                                                                                \
        negative = true;                                                                                                 \
        i++;                                                                                                             \
    }                                                                                                                    \
                                                                                                                         \
    for(; i < string.len; i++) {                                                                                         \
        u8 c = str_at(string, i);                                                                                        \
        if(!is_number(c)) {                                                                                              \
            return ResultErr(type, E(EG_Strings, E_InvalidArgument, S("string cannot be parsed to number"))); \
        }                                                                                                                \
                                                                                                                         \
        u8 c_num = TryReturn(type, u8, char_to_num(c));                                                                  \
        num = TryReturn(type, type, MultSafe(type, num, 10));                                                            \
        num = TryReturn(type, type, AddSafe(type, num, c_num));                                                          \
    }                                                                                                                    \
                                                                                                                         \
    if(negative) {                                                                                                       \
        num = TryReturn(type, type, MultSafe(type, num, -1));                                                            \
    }                                                                                                                    \
                                                                                                                         \
    return ResultOk(type, num);                                                                                          \
}

__TemplateStrParseUnsigned(u8);
__TemplateStrParseUnsigned(u16);
__TemplateStrParseUnsigned(u32);
__TemplateStrParseUnsigned(u64);

__TemplateStrParseSigned(s8);
__TemplateStrParseSigned(s16);
__TemplateStrParseSigned(s32);
__TemplateStrParseSigned(s64);

#define Template_ToStrSigned(type)                                                          \
GenericFunc_ToStr(type, arena, num) {                                                       \
    bool negative = (num < 0);                                                              \
    u64 digits;                                                                             \
                                                                                            \
    if(negative) {                                                                          \
        num *= -1;                                                                          \
    }                                                                                       \
                                                                                            \
    if(num == 0) {                                                                          \
        digits = 1;                                                                         \
    } else {                                                                                \
        digits = f64_floor(f64_log10(num)) + 1;                                             \
    }                                                                                       \
                                                                                            \
    u64 str_len = digits + (negative ? 1 : 0);                                              \
    u8* buffer = arena_alloc(arena, str_len);                                               \
    StringAlloc a_string = {.string = {.chars = buffer, .len = str_len}};                   \
                                                                                            \
    if(negative) {                                                                          \
        str_set(a_string.string, 0, '-');                                                   \
    }                                                                                       \
                                                                                            \
    s32 index = a_string.string.len - 1;                                                    \
    while(num > 0) {                                                                        \
        u8 digit = num % 10;                                                                \
        u8 ch = TryCrash(u8, num_to_char(digit));                                           \
        str_set(a_string.string, index, ch);                                                \
        num /= 10;                                                                          \
        index--;                                                                            \
    }                                                                                       \
                                                                                            \
    return a_string;                                                                        \
}

#define Template_ToStrUnsigned(type)                                     \
GenericFunc_ToStr(type, arena, num) {                                    \
    u64 digits;                                                          \
                                                                         \
    if(num == 0) {                                                       \
        digits = 1;                                                      \
    } else {                                                             \
        digits = f64_floor(f64_log10(num)) + 1;                          \
    }                                                                    \
                                                                         \
    u8* buffer = arena_alloc(arena, digits);                             \
    StringAlloc a_string = {.string = {.chars = buffer, .len = digits}}; \
                                                                         \
    for(s64 i = digits - 1; i >= 0; i--) {                               \
        u8 digit = num % 10;                                             \
        u8 ch = TryCrash(u8, num_to_char(digit));                        \
        str_set(a_string.string, i, ch);                                 \
        num /= 10;                                                       \
    }                                                                    \
                                                                         \
    return a_string;                                                     \
}

Template_ToStrSigned(s8);
Template_ToStrSigned(s16);
Template_ToStrSigned(s32);
Template_ToStrSigned(s64);

Template_ToStrUnsigned(u8);
Template_ToStrUnsigned(u16);
Template_ToStrUnsigned(u32);
Template_ToStrUnsigned(u64);
