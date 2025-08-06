#ifndef ERRORS_H
#define ERRORS_H

#include <c_base/base/strings/string_view.h>

#define GenericVal_ErrorCode(name) extern ErrorCode name;
#define GenericValImpl_ErrorCode(name)                                         \
  StringView __str_##name = StringViewLit(#name);                              \
  ErrorCode name = &__str_##name;

#define E(grp_code, code, msg) Error_construct(grp_code, code, msg)

typedef StringView* ErrorCode;

typedef struct {
  ErrorCode grp_code;
  ErrorCode code;
  StringView msg;
} Error;

Error Error_construct(ErrorCode grp_code, ErrorCode code, StringView msg);

// {{{ Generic_Val_ErrorCode
GenericVal_ErrorCode(EG_Unspecified)

GenericVal_ErrorCode(E_Unspecified)
GenericVal_ErrorCode(E_InvalidPointer)
GenericVal_ErrorCode(E_InvalidArgument)
GenericVal_ErrorCode(E_OutOfBounds)
// }}}

#endif
