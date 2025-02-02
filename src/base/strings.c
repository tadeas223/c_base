#include "base/strings.h"
#include "base/mem.h"
#include "base/types.h"
#include <stdio.h>

String8
str8_alloc(m_Arena *arena, u64 count) {
    u8* str = m_arena_push(arena, sizeof(u8) * count);
    String8 result = (String8){.str = str, .count = count};
    return result;
}

String8
str8(u8 *str, u64 count) {
    return (String8){.str = str, .count = count};
}

String8
str8_cstr(const char *cstr) {
    u64 count = 0;
    String8 str;
    str.str = (u8*)cstr;
    
    while(cstr[count] != null) {
        count++;
    }

    str.count = count;

    return str;
}

String8
str8_cstr_count(const char *cstr, u64 count) {
    String8 string;
    string.str = (u8*)cstr;
    string.count = count;
    return string;
}

char*
str8_to_cstr(m_Arena *arena, String8 string) {
    char* cstr = m_arena_push(arena, sizeof(char) * string.count + 1);
    
    m_copy(cstr, string.str, string.count);
    cstr[string.count] = '\0';

    return cstr;
}

String8
str8_range(u8 *start, u8 *end) {
    String8 string;
    string.count = end - start;
    string.str = start;
    return string;
}

void
str8_print(String8 string) {
    u64 i;
    for(i = 0; i < string.count; i++) {
        printf("%c", string.str[i]);
    }
}

String8
str8_substr(String8 string, u64 start, u64 count) {
    return (String8){.str = string.str + start, .count = count};
}


/* same as list = {0} */
void
str8_list_init(String8List *list) {
    list->head = null;
    list->tail = null;
    list->node_count = 0;
    list->str_count = 0;
}

void
str8_list_push(m_Arena *arena, String8List *list, String8 string) {
    String8Node *node = m_arena_push(arena, sizeof(*node));
    node->str = string;
    node->next = null;
    node->prev = null;

    if(list->head == null) {
        list->head = node;
    } else if(list->tail == null) {
        list->tail = node;
        list->head->next = list->tail; 
        list->tail->prev = list->head;
    } else {
        node->prev = list->tail;
        list->tail->next = node;
        list->tail = node;
    }
    list->node_count++;
    list->str_count += string.count;
}

String8
str8_list_pop(String8List *list) {
    String8 string = list->tail->str;
    list->tail = list->tail->prev;
    list->node_count--;
    list->str_count -= string.count;
    return string;
}



String8List
str8_split(m_Arena *arena, String8 string, u8 splitter) {
    u64 splitStart = 0;
    u64 i = 0;
    String8List list;
    str8_list_init(&list);

    for(i = 0; i < string.count; i++) {
        if(string.str[i] == splitter) {
            String8 splitString = str8_substr(string, splitStart, i - splitStart);
            str8_list_push(arena, &list, splitString);
            splitStart = i + 1; /* skip the splitter */
        }
    }
   
    String8 splitString = str8_substr(string, splitStart, i - splitStart);
    str8_list_push(arena, &list, splitString);

    return list;
}

String8
str8_join(m_Arena *arena, String8List *list) {
    String8 str = str8_alloc(arena, list->str_count);

    u64 counter = 0;
    String8Node *node = list->head;
    for(node = list->head; node != null; node = node->next) {
        m_copy(str.str + counter, node->str.str, node->str.count); 
        counter += node->str.count;
    }

    return str;
}

u8
str8_at(String8 string, u64 pos) {
    if(pos >= string.count) return null;
    return string.str[pos];
}
