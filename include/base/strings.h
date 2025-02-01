#ifndef STRINGS_H
#define STRINGS_H

#include "base/mem.h"
#include "base/types.h"

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

#endif
