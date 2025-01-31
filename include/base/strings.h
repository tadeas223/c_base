#ifndef STRINGS_H
#define STRINGS_H

#include "base/mem.h"
#include "base/types.h"

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

String8 str8_alloc(m_Arena *arena, u64 count);
void str8_init(String8 *string, u8* str, u64 count);
String8 str8(u8* str, u64 count);
String8 str8_cstr(u8* cstr);
String8 str8_range(u8* start, u8* end);

void str8_print(String8 *str);
void str8_print_view(String8View *str);

String8View str8_view(String8* str);

u8 str8_at(String8View* str);
String8View str8_substr(String8View* str, u64 start, u64 length);

String8ViewList* str8_split(m_Arena *arena, String8View *view, u8 splitter);

void str8_list_init(String8ViewList *list);
void str8_list_push(m_Arena *arena, String8ViewList *list, String8View *view);
String8View str8_list_pop(String8ViewList *list);
#endif









