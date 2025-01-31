#include "base/strings.h"
#include "base/defines.h"
#include "base/mem.h"
#include "base/ds.h"

String8
str8_alloc(m_Arena *arena, u64 count) {
    u8* str = m_arena_push(arena, sizeof(*str) * count);
    String8 result = (String8){.str = str, .count = count};
    return result;
}

String8
str8(u8 *str, u64 count) {
    return (String8){.str = str, .count = count};
}

String8
str8_cstr(u8 *cstr) {
    u64 count = 0;
    String8 str;
    str.str = cstr;
    
    while(cstr[count]) {
        count++;
    }

    return str;
}

String8
str8_range(u8 *start, u8 *end) {
    String8 str;
    str.count = end - start;
    str.str = start;
    return str;
}

String8View
str8_view(String8 *str) {
    return (String8View){.str = str->str, .count = str->count};
}

String8View
str8_substr(String8View *str, u64 start, u64 count) {
    return (String8View){.str = str->str + start, .count = count};
}

String8ViewArray
str8_split(String8View *view, u8 splitter) {
    struct SplitPos {
        int start;
        int end;
    } splitPos;


    String8ViewArray array;
    DArrayInit(&array);
    

    struct splitPos = (SplitPos){.start = 0, .end = 0};
    
    struct {
        m_Arena arena;
        u64 count;

    }


    for(i = 0; i < view->count; i++) {
        if(view->str[i] == splitter) {
        }
    }

    return array;
}
