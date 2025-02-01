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

String8View
str8_cstr(const char *cstr) {
    u64 count = 0;
    String8View str;
    str.str = (u8*)cstr;
    
    while(cstr[count] != null) {
        count++;
    }

    str.count = count;

    return str;
}

String8View
str8_cstr_count(const char *cstr, u64 count) {
    String8View str;
    str.str = (u8*)cstr;
    str.count = count;
    return str;
}

String8
str8_range(u8 *start, u8 *end) {
    String8 str;
    str.count = end - start;
    str.str = start;
    return str;
}

void
str8_print(String8 str) {
    u64 i;
    for(i = 0; i < str.count; i++) {
        printf("%c", str.str[i]);
    }
}

void
str8_view_print(String8View view) {
    u64 i;
    for(i = 0; i < view.count; i++) {
        printf("%c", view.str[i]);
    }
}

String8View
str8_view(String8 str) {
    return (String8View){.str = str.str, .count = str.count};
}

String8View
str8_substr(String8View view, u64 start, u64 count) {
    return (String8View){.str = view.str + start, .count = count};
}

void
str8_list_init(String8ViewList *list) {
    list->head = null;
    list->tail = null;
    list->node_count = 0;
    list->str_count = 0;
}

void
str8_list_push(m_Arena *arena, String8ViewList *list, String8View view) {
    String8ViewNode *node = m_arena_push(arena, sizeof(String8ViewNode));
    node->str = view;
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
    list->str_count += view.count;
}

String8View
str8_list_pop(String8ViewList *list) {
    String8View view = list->tail->str;
    list->tail = list->tail->prev;
    list->node_count--;
    list->str_count -= view.count;
    return view;
}



String8ViewList*
str8_split(m_Arena *arena, String8View view, u8 splitter) {
    u64 splitStart = 0;
    u64 i = 0;
    String8ViewList *list = m_arena_push(arena, sizeof(*list));
    str8_list_init(list);

    for(i = 0; i < view.count; i++) {
        if(view.str[i] == splitter) {
            String8View splitView = str8_substr(view, splitStart, i - splitStart);
            str8_list_push(arena, list, splitView);
            splitStart = i + 1; /* skip the splitter */
        }
    }
   
    String8View splitView = str8_substr(view, splitStart, i - splitStart);
    str8_list_push(arena, list, splitView);

    return list;
}

String8
str8_join(m_Arena *arena, String8ViewList *list) {
    String8 str = str8_alloc(arena, list->str_count);

    u64 counter = 0;
    String8ViewNode *node = list->head;
    for(node = list->head; node != null; node = node->next) {
        m_copy(str.str + counter, node->str.str, node->str.count); 
        counter += node->str.count;
    }

    return str;
}

u8
str8_at(String8View view, u64 pos) {
    if(pos >= view.count) return null;
    return view.str[pos];
}
