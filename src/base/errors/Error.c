#include "c_base/base/errors/Error.h"

Error Error_construct(ErrorCode grp_code, ErrorCode code, StringView msg) {
  Error self;
  self.grp_code = grp_code;
  self.code = code;
  self.msg = msg;
  return self;
}
