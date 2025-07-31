#ifndef STRING_VIEW_H
#define STRING_VIEW_H

#include <c_base/base/types.h>

#define SV(cstr) StringView_construct((ascii*)(cstr), sizeof(cstr) - 1)
#define StringViewLit(cstr) {(ascii*)(cstr), sizeof(cstr) - 1}

typedef struct {
  ascii* chars;
  u32 len;
} StringView;

/******************************
 * const
 ******************************/
StringView StringView_construct(ascii* chars, u32 len);

#endif
