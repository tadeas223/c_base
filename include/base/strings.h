#ifndef STRINGS_H
#define STRINGS_H

#include "base/mem.h"
#include "base/errors.h"
#include "base/types.h"

#define Str8Lit(c_str) str8_cstr_count(c_str, sizeof(c_str) - 1)

typedef struct {
    u64 count;
    u8 *str;
} String8;

typedef struct {
    String8 string;
} String8Alloc;

typedef struct String8Node {
    struct String8Node *next;
    struct String8Node *prev;
    String8 string;
} String8Node;

typedef struct {
    String8Node *head;
    String8Node *tail;
    u64 node_count;
    u64 str_count;
} String8List;

typedef struct {
    ResultVars(String8);
} String8Result;

typedef struct {
    ResultVars(String8Alloc);
} String8AllocResult;

typedef struct {
    ResultVars(String8List);
} String8ListResult;

String8 str8(u8* str, u64 count);
String8 str8_range(u8* start, u8* end);

String8Alloc str8_alloc(m_Arena *arena, u64 count);

String8 str8_cstr(const char* cstr);
String8 str8_cstr_count(const char* cstr, u64 count);
char* str8_to_cstr(m_Arena *arena, String8 str);

String8 substr(String8 string, u64 start, u64 count);

void str8_list_begin(String8List *list);
void str8_list_push(m_Arena *arena, String8List *list, String8 string);
String8 str8_list_pop(String8List *list);

String8List str8_split(m_Arena *arena, String8 string, u8 splitter);
String8Alloc str8_join(m_Arena *arena, String8List *list);

u8 str8_at(String8 view, u64 pos);

bool is_number(u8 c);
bool is_white_space(u8 c);
bool is_alpha(u8 c);
bool is_alhpanum(u8 c);
bool is_upper(u8 c);
bool is_lower(u8 c);

#define IsNumber(c) (c >= '0' && c <= '9')
#define IsSpace(c) (c == ' ')
#define IsUpper(c) (c >= 'A' && c <= 'Z')
#define IsLower(c) (c >= 'a' && c <= 'z')

#define IsNewLine(c) (c == '\n')

#define IsAlpha(c) (IsUpper(c) || IsLower(c)))
#define IsAphaNum(c) (IsAlpha(c) || IsNumber(c))

#endif
