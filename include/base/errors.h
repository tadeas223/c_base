#ifndef ERRORS_H
#define ERRORS_H

#include "base/types.h"

#define ResultVars(type) type value; const u32 err; const bool ok

#define ResultOK(val) {.ok = true, .value = val}
#define ResultERR(err_code) {.ok = false, .err = err_code}

extern enum {
    ERR_UNSPECIFIED = U32_MAX,
} ErrorCodes;

/* unsigned results */
typedef struct {
    ResultVars(u8);
} U8Result;

typedef struct {
    ResultVars(u16);
} U16Result;

typedef struct {
    ResultVars(u32);
} U32Result;

typedef struct {
    ResultVars(u64);
} U64Result;

/* signed results */
typedef struct {
    ResultVars(s8);
} S8Result;

typedef struct {
    ResultVars(s16);
} S16Result;

typedef struct {
    ResultVars(s32);
} S32Result;

typedef struct {
    ResultVars(s64);
} S64Result;

/* bool results */
typedef struct {
    ResultVars(b8);
} B8Result;

typedef struct {
    ResultVars(b16);
} B16Result;

typedef struct {
    ResultVars(b32);
} B32Result;

typedef struct {
    ResultVars(b64);
} B64Result;

#define BoolResult B8Result

#endif
