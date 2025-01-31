#ifndef STRINGS_H
#define STRINGS_H

#include "base/types.h"

typedef struct {
    u64 length;
    u8 *c_str;
} String;

void string_init(String* str, u8 *c_str);
void string_init_length(String* str, u8 *c_str, u64 length);

String substring(String* str, u64 start, u64 length);

#endif
