#ifndef STRDEF_H
#define STRDEF_H

#include "base/types.h"


#define S(cstr) ((String) {.len = sizeof((cstr))-1, .chars = (u8*) (cstr)})

TypeStruct(String, {
    u32 len;
    u8* chars;
});

TypeStruct(StringAlloc, {
    String string;
});

TypeStruct(StringStack, {
    String string;
});

#endif
