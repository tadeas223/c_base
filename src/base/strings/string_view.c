#include <c_base/base/strings/string_view.h>

/******************************
 * const
 ******************************/
StringView StringView_construct(ascii* chars, u32 len) {
  StringView self;
  self.chars = chars;
  self.len = len;
  return self;
}
