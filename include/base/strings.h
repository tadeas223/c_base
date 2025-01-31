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

typedef struct {
    m_Arena arena;
    u64 count;
    String8View *items;
} String8ViewArray;

String8 str8_alloc(m_Arena *arena, u64 count);
void str8_init(String8 *string, u8* str, u64 count);
String8 str8(u8* str, u64 count);
String8 str8_cstr(u8* cstr);
String8 str8_range(u8* start, u8* end);

String8View str8_view(String8* str);

String8View str8_substr(String8View* str, u64 start, u64 length);
u8 str8_at(String8View* str);

#endif









