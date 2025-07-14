#include <c_base/base/strings/string_view.h>

/******************************
 * const
 ******************************/
StringView StringView_construct(u8 *chars, u32 len) {
  StringView self;
  self.chars = chars;
  self.len = len;
  return self;
}
