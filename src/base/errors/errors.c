#include <c_base/base/errors/errors.h>

// {{{ GenericBalImpl_ErrorCode
GenericValImpl_ErrorCode(EG_Unspecified)

  GenericValImpl_ErrorCode(E_Unspecified)
  GenericValImpl_ErrorCode(E_InvalidPointer)
  GenericValImpl_ErrorCode(E_InvalidArgument)
GenericValImpl_ErrorCode(E_OutOfBounds)
  // }}}

Error Error_construct(ErrorCode grp_code, ErrorCode code, StringView msg) {
  Error self;
  self.grp_code = grp_code;
  self.code = code;
  self.msg = msg;
  return self;
}

