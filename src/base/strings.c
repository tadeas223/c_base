#include "base/strings.h"
#include "base/errors.h"
#include "base/mem.h"
#include "base/types.h"
#include <cctype>
#include <cinttypes>
#include <threads.h>

/****************************************
 * String creation
****************************************/

String8
str8(u8 *str, u64 count) {
    return (String8){.str = str, .count = count};
}

String8
str8_range(u8 *start, u8 *end) {
    String8 string;
    string.count = end - start;
    string.str = start;
    return string;
}

String8Alloc
str8_alloc(m_Arena *arena, u64 count) {
    u8* str = m_arena_alloc(arena, sizeof(u8) * count);
    String8Alloc result = (String8Alloc){.string = {.str = str, .count = count}};
    return result;
}

String8
str8_cstr_count(const char *cstr, u64 count) {
    String8 string;
    string.str = (u8*)cstr;
    string.count = count;
    return string;
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

/****************************************
 * String manipulation
****************************************/

char*
str8_to_cstr(m_Arena *arena, String8 string) {
    char* cstr = m_arena_alloc(arena, sizeof(char) * string.count + 1);
    
    m_copy(cstr, string.str, string.count);
    cstr[string.count] = '\0';

    return cstr;
}

String8
str8_substr(String8 string, u64 start, u64 count) {
    return (String8){.str = string.str + start, .count = count};
}

String8List
str8_split(m_Arena *arena, String8 string, u8 splitter) {
    u64 splitStart = 0;
    u64 i = 0;
    String8List list;
    str8_list_begin(&list);

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

String8Alloc
str8_join(m_Arena *arena, String8List *list) {
    String8Alloc str_alloc = str8_alloc(arena, list->str_count);

    u64 counter = 0;
    String8Node *node = list->head;
    for(node = list->head; node != null; node = node->next) {
        m_copy(str_alloc.string.str + counter, node->string.str, node->string.count); 
        counter += node->string.count;
    }

    return str_alloc;
}

/****************************************
 * String list
****************************************/

/* same as list = {0} */
void
str8_list_begin(String8List *list) {
    list->head = null;
    list->tail = null;
    list->node_count = 0;
    list->str_count = 0;
}

void
str8_list_push(m_Arena *arena, String8List *list, String8 string) {
    String8Node *node = m_arena_alloc(arena, sizeof(*node));
    node->string = string;
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
    String8 string = list->tail->string;
    list->tail = list->tail->prev;
    list->node_count--;
    list->str_count -= string.count;
    return string;
}

u8
str8_at(String8 string, u64 pos) {
    if(pos >= string.count) return null;
    return string.str[pos];
}


/****************************************
 * Char checking
****************************************/

bool
is_number(u8 c) { return IsNumber(c); }

bool
is_space(u8 c) { return IsSpace(c); }

bool
is_alpha(u8 c) { return IsAlpha(c); }

bool
is_alhpanum(u8 c) { return IsAlphaNum(c); }

bool
is_upper(u8 c) { return IsUpper(c); }

bool
is_lower(u8 c) { return IsLower(c); }

bool
is_new_line(u8 c) { return IsNewLine(c); }


/**************************************** Integer and Float parsing
****************************************/
U8Result str8_parse_u8(String8 str) {
    u8 num;
    u8 i;
    for(i = 0; i < str.count; i++) {
        u8 c = str8_at(str, i);
        if(!IsNumber(c)) {
            return (U8Result) ResultERR(ERR_UNSPECIFIED); 
        }
        
        u8 cur_num = NumFromAscii(c);
        
        if(num + cur_num < num) {
             return (U8Result) ResultERR(ERR_UNSPECIFIED);
        }

        num += cur_num;

    }

    return (U8Result) ResultOK(num);
}

U16Result str8_parse_u16(String8 str) {
    u16 num;
    u8 i;
    for(i = 0; i < str.count; i++) {
        u8 c = str8_at(str, i);
        if(!IsNumber(c)) {
            return (U16Result) ResultERR(ERR_UNSPECIFIED); 
        }
        
        u8 cur_num = NumFromAscii(c);
        
        if(num + cur_num < num) {
             return (U16Result) ResultERR(ERR_UNSPECIFIED);
        }

        num += cur_num;

    }

    return (U16Result) ResultOK(num);
}

U32Result str8_parse_u32(String8 str) {
    u32 num;
    u8 i;
    for(i = 0; i < str.count; i++) {
        u8 c = str8_at(str, i);
        if(!IsNumber(c)) {
            return (U32Result) ResultERR(ERR_UNSPECIFIED); 
        }
        
        u8 cur_num = NumFromAscii(c);
        
        if(num + cur_num < num) {
             return (U32Result) ResultERR(ERR_UNSPECIFIED);
        }

        num += cur_num;

    }

    return (U32Result) ResultOK(num);
}
U64Result str8_parse_u64(String8 str) {
    u64 num;
    u8 i;
    for(i = 0; i < str.count; i++) {
        u8 c = str8_at(str, i);
        if(!IsNumber(c)) {
            return (U64Result) ResultERR(ERR_UNSPECIFIED); 
        }
        
        u8 cur_num = NumFromAscii(c);
        
        if(num + cur_num < num) {
             return (U64Result) ResultERR(ERR_UNSPECIFIED);
        }

        num += cur_num;

    }

    return (U64Result) ResultOK(num);
}

S8Result str8_parse_s8(String8 str) {
    s8 num;
    u8 i = 0;
    bool is_positive;
    
    u8 c = str8_at(str, i);
    if(c == '-') {
         is_positive = false;
         i++;
    } else if(c == '+') {
        is_positive = true;
         i++;
    }

    
    for(; i < str.count; i++) {
        u8 c = str8_at(str, i);
        if(!IsNumber(c)) {
            return (S8Result) ResultERR(ERR_UNSPECIFIED); 
        }
        
        u8 cur_num = NumFromAscii(c);
        
        if((is_positive && num + cur_num < num) || (!is_positive && num + cur_num > num)) {
             return (S8Result) ResultERR(ERR_UNSPECIFIED);
        }
        
        if(is_positive) {
            num += cur_num;
        } else {
            num -= cur_num;
        }
    }

    return (S8Result) ResultOK(num);
}

S16Result str8_parse_s16(String8 str) {
    s16 num;
    u8 i = 0;
    bool is_positive;
    
    u8 c = str8_at(str, i);
    if(c == '-') {
         is_positive = false;
         i++;
    } else if(c == '+') {
        is_positive = true;
         i++;
    }

    
    for(; i < str.count; i++) {
        u8 c = str8_at(str, i);
        if(!IsNumber(c)) {
            return (S16Result) ResultERR(ERR_UNSPECIFIED); 
        }
        
        u8 cur_num = NumFromAscii(c);
        
        if((is_positive && num + cur_num < num) || (!is_positive && num + cur_num > num)) {
             return (S16Result) ResultERR(ERR_UNSPECIFIED);
        }
        
        if(is_positive) {
            num += cur_num;
        } else {
            num -= cur_num;
        }
    }

    return (S16Result) ResultOK(num);
}
S32Result str8_parse_s32(String8 str) {
    s32 num;
    u8 i = 0;
    bool is_positive;
    
    u8 c = str8_at(str, i);
    if(c == '-') {
         is_positive = false;
         i++;
    } else if(c == '+') {
        is_positive = true;
         i++;
    }

    
    for(; i < str.count; i++) {
        u8 c = str8_at(str, i);
        if(!IsNumber(c)) {
            return (S32Result) ResultERR(ERR_UNSPECIFIED); 
        }
        
        u8 cur_num = NumFromAscii(c);
        
        if((is_positive && num + cur_num < num) || (!is_positive && num + cur_num > num)) {
             return (S32Result) ResultERR(ERR_UNSPECIFIED);
        }
        
        if(is_positive) {
            num += cur_num;
        } else {
            num -= cur_num;
        }
    }

    return (S32Result) ResultOK(num);
}
S64Result str8_parse_s64(String8 str) {
    s64 num;
    u8 i = 0;
    bool is_positive;
    
    u8 c = str8_at(str, i);
    if(c == '-') {
         is_positive = false;
         i++;
    } else if(c == '+') {
        is_positive = true;
         i++;
    }

    
    for(; i < str.count; i++) {
        u8 c = str8_at(str, i);
        if(!IsNumber(c)) {
            return (S64Result) ResultERR(ERR_UNSPECIFIED); 
        }
        
        u8 cur_num = NumFromAscii(c);
        
        if((is_positive && num + cur_num < num) || (!is_positive && num + cur_num > num)) {
             return (S64Result) ResultERR(ERR_UNSPECIFIED);
        }
        
        if(is_positive) {
            num += cur_num;
        } else {
            num -= cur_num;
        }
    }

    return (S64Result) ResultOK(num);
}
