#ifndef ERRORS_H
#define ERRORS_H

#include "base/types.h"
#include "base/strdef.h"
#include "base/macros.h"

typedef u64 ErrorCode;
typedef u64 ErrorGroup;

TypeStruct(Error, {
    u64 code;
    ErrorGroup grp;
    String msg;
});

#define Define_Error(name)                               \
    String Concat(__##name, _str) = S(#name);            \
    ErrorCode name = (ErrorCode)&Concat(__##name, _str) 

#define Define_ErrorGroup(name)                          \
    String Concat(__##name, _str) = S(#name);            \
    ErrorGroup name = (ErrorGroup)&Concat(__##name, _str) 

#define Extern_Error(name)      \
    extern ErrorCode name

#define Extern_ErrorGroup(name) \
    extern ErrorGroup name

#define E(errgrp, errcode, errmsg) (Error) {.grp = (errgrp), .code = (errcode), .msg = (errmsg)}

#define ErrorName(err) (String)(*(String*)err.code)
#define ErrorGroupName(err) (String)(*(String*)err.grp)

Extern_ErrorGroup(EG_Unspecified);

Extern_Error(E_OutOfBounds);
Extern_Error(E_Uninitialzied);
Extern_Error(E_Unimplemented);
Extern_Error(E_InvalidArgument);
Extern_Error(E_NullArgument);
Extern_Error(E_Unspecified);
Extern_Error(E_OutOfMemory);

#define GenericType_Result(type)                    \
    TypeStruct(Concat(type, _Result), {             \
        union {                                     \
            type value;                             \
            Error err;                              \
        };                                          \
        bool ok;                                    \
    });                                             \

#define GenericTypeAll_Result(type)                  \
    GenericType_Result(type);                        \
    GenericType_Result(Concat(type, _p));            \
    GenericType_Result(Concat(type, _pp));           \
    GenericType_Result(Concat(type, _ppp));          \

#define Result(type) Concat(type, _Result)
#define Result_p(type) Concat(type, _Result_p)
#define Result_pp(type) Concat(type, _Result_pp)
#define Result_ppp(type) Concat(type, _Result_ppp)

#define ResultOk(type, val) (Result(type)) {.ok = true, .value = (val)}
#define ResultErr(type, error) (Result(type)) {.ok = false, .err = (error)}

#define TryReturn(current_func_ret, expr_ret, expr) ({\
        Highlight(expr_ret);\
        Result(expr_ret) result = expr;\
        if(!result.ok) {\
            return ResultErr(current_func_ret, result.err);\
        }\
        result.value;\
        })

typedef struct {
    bool ok;
    Error err;
} Result;

//#define EmptyResultOk() (Result) {.ok = true}
//#define EmptyResultErr(error) (Result) {.err = (error)} 

GenericType_Result(none);
GenericType_Result(void_p);
GenericType_Result(void_pp);
GenericType_Result(void_ppp);

GenericTypeAll_Result(u8);
GenericTypeAll_Result(u16);
GenericTypeAll_Result(u32);
GenericTypeAll_Result(u64);

GenericTypeAll_Result(s8);
GenericTypeAll_Result(s16);
GenericTypeAll_Result(s32);
GenericTypeAll_Result(s64);

GenericTypeAll_Result(b8);
GenericTypeAll_Result(b16);
GenericTypeAll_Result(b32);
GenericTypeAll_Result(b64);

typedef Result(b8) Result(bool);

#endif
