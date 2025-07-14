#ifndef STRING_VIEW_H
#define STRING_VIEW_H

#include "base/types.h"

#define SV(cstr) StringView_construct((u8*)(cstr), sizeof(cstr) - 1)
#define StringViewLit(cstr) {(u8*)(cstr), sizeof(cstr) - 1}

typedef struct {
  u8* chars;
  u32 len;
} StringView;

/******************************
 * const
 ******************************/
StringView StringView_construct(u8* chars, u32 len);

#endif
