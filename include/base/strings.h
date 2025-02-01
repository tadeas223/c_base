#ifndef STRINGS_H
#define STRINGS_H

#include "base/mem.h"
#include "base/types.h"
#include "base/defines.h"

#define Str8Lit(c_str) str8_cstr_count(c_str, sizeof(c_str) - 1)

typedef struct {
    u64 count;
    u8 *str;
} String8;

typedef struct {
    u64 count;
    u8 *str;
} String8View;

typedef struct String8ViewNode {
    struct String8ViewNode *next;
    struct String8ViewNode *prev;
    String8View str;
} String8ViewNode;

typedef struct {
    String8ViewNode *head;
    String8ViewNode *tail;
    u64 node_count;
    u64 str_count;
} String8ViewList;

String8 str8(u8* str, u64 count);
String8 str8_range(u8* start, u8* end);
String8 str8_alloc(m_Arena *arena, u64 count);

String8View str8_view(String8 str);
String8View str8_cstr(const char* cstr);
String8View str8_cstr_count(const char* cstr, u64 count);

void str8_print(String8 str);
void str8_view_print(String8View view);

u8 str8_at(String8View view, u64 pos);

void str8_list_init(String8ViewList *list);
void str8_list_push(m_Arena *arena, String8ViewList *list, String8View view);
String8View str8_list_pop(String8ViewList *list);

String8View str8_substr(String8View view, u64 start, u64 length);
String8ViewList* str8_split(m_Arena *arena, String8View view, u8 splitter);
String8 str8_join(m_Arena *arena, String8ViewList *list);

bool is_number(u8 c);
bool is_white_space(u8 c);
bool is_alpha(u8 c);
bool is_alhpanum(u8 c);
bool is_upper(u8 c);
bool is_lower(u8 c);

#define IsNumber(c) (c >= 48 && c <= 57)
#define IsSpace(c) (c == 32)
#define IsUpper(c) (c >= 65 && c <= 90)
#define IsLower(c) (c >= 97 && c <= 122)

#if defined(PLATFORM_WINDOWS)
    #define IsNewLine(c) (c == 10 && c == 13)
#else
    #define IsNewLine(c) (c == 10)
#endif

#define IsAlpha(c) (IsUpper(c) || IsLower(c)))
#define IsAphaNum(c) (IsAlpha(c) || IsNumber(c))

#endif
