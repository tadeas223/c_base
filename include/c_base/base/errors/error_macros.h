#ifndef ERROR_MACROS_H
#define ERROR_MACROS_H

#include "c_base/base/errors/Error.h"
#include "c_base/base/strings/string_view.h"

#define GenericVal_ErrorCode(name) extern ErrorCode name;
#define GenericValImpl_ErrorCode(name)                                         \
  StringView __str_##name = StringViewLit(#name);                              \
  ErrorCode name = &__str_##name;

// {{{ Generic_Val_ErrorCode
GenericVal_ErrorCode(EG_Unspecified)

GenericVal_ErrorCode(E_Unspecified)
GenericVal_ErrorCode(E_InvalidPointer)
GenericVal_ErrorCode(E_InvalidArgument)
GenericVal_ErrorCode(E_OutOfBounds)
// }}}

#endif
