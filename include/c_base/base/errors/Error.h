#ifndef ERROR_H
#define ERROR_H

#include "c_base/base/strings/string_view.h"
#define E(grp_code, code, msg) Error_construct(grp_code, code, msg)

typedef StringView* ErrorCode;

typedef struct {
  ErrorCode grp_code;
  ErrorCode code;
  StringView msg;
} Error;

Error Error_construct(ErrorCode grp_code, ErrorCode code, StringView msg);

#endif
