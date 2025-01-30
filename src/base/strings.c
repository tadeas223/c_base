#include "base/strings.h"

void
string_init_length(String *str, u8 *c_str, u64 length) {
    str->length = length;
    str->c_str = c_str;
}

void
string_init(String *str, u8 *c_str) {
    u64 len = 0;
    while(c_str[len] != null) {
        len++;
    }

    str->c_str = c_str;
    str->length = len;
}

String
substring(String *str, u64 start, u64 length) {
    String substr;
    substr.c_str = str->c_str;
    substr.c_str += start;
    substr.length = length;
    return substr;
}


