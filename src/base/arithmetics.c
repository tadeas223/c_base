#include "base/arithmetics.h"
#include "base/errors.h"
#include "system.h"

Define_ErrorGroup(EG_Arithmetics);
Define_Error(E_Overflow);
Define_Error(E_DivisionByZero);

#define Template_ArithmeticsAddSafe(type)\
GenericFunc_AddSafe(type, a, b) {\
    if((a > 0 && b > Max(type) - a) || (a < 0 && b < Max(type) - a)) {\
        return ResultErr(type, E(EG_Arithmetics, E_Overflow,          \
                    S("overflow while adding two numbers")));         \
    }                                                                 \
                                                                      \
    return ResultOk(type, a + b);                                     \
}

#define Template_ArithmeticsSubSafe(type)                              \
GenericFunc_SubSafe(type, a, b) {                                      \
    if((a < 0 && b < Max(type) + a) || (a > 0 && b > Min(type) + a)) { \
        return ResultErr(type, E(EG_Arithmetics, E_Overflow,           \
                    S("overflow while subtracting two numbers")));     \
    }                                                                  \
                                                                       \
    return ResultOk(type, a - b);                                      \
}

#define Template_ArithmeticsMultSafe(type)                             \
GenericFunc_MultSafe(type, a, b) {                                     \
    if((a > 0 && b > Max(type) / a) || (a < 0 && b < Min(type) / a)) { \
        return ResultErr(type, E(EG_Arithmetics, E_Overflow,           \
                    S("overflow while multiplying two numbers")));     \
    }                                                                  \
                                                                       \
    return ResultOk(type, a * b);                                      \
}

#define Template_ArithmeticsUnsignedDivSafe(type)     \
GenericFunc_DivSafe(type, a, b) {                     \
    if(b == 0) {                                      \
        return ResultErr(type, E(EG_Arithmetics,      \
                    E_DivisionByZero,                 \
                    S("division by zero :(")));       \
    }                                                 \
    return ResultOk(type, a / b);                     \
}

#define Template_ArithmeticsSignedDivSafe(type)                  \
GenericFunc_DivSafe(type, a, b) {                               \
    if(b == 0) {                                                \
        return ResultErr(type, E(EG_Arithmetics,                \
                    E_DivisionByZero,                           \
                    S("division by zero :(")));                 \
    }                                                           \
    if(a == Min(type) && b == -1) {                             \
        return ResultErr(type, E(EG_Arithmetics, E_Overflow,    \
                    S("overflow while dividing two numbers"))); \
                                                                \
    }                                                           \
    return ResultOk(type, a / b);                               \
}

f32 f32_log(u64 exp, f32 num) {
    if(num <= 0) {
        crash(E(EG_Arithmetics, E_InvalidArgument, S("logarithm works only on numbers grater than 0")));
    }

    u64 log_value = 0;
    while(num >= exp) {
        num /= exp;
        log_value++;
    }
    return log_value;
}

f64 f64_log(u64 exp, f64 num) {
    if(num <= 0) {
        crash(E(EG_Arithmetics, E_InvalidArgument, S("logarithm works only on numbers grater than 0")));
    }

    u64 log_value = 0;
    while(num >= exp) {
        num /= exp;
        log_value++;
    }
    return log_value;
}

f64 f64_log10(f64 num) {
    return f64_log(10, num);
}

f32 f32_log10(f32 num) {
    return f32_log(10, num);
}

f32 f32_floor(f32 num) {
    s32 i = (s32) num;

    if(num < 0 && i != num) {
        return i-1;
    }

    return i;
}

f64 f64_floor(f64 num) {
    s64 i = (s64) num;

    if(num < 0 && i != num) {
        return i-1;
    }

    return i;
} 

Template_ArithmeticsAddSafe(u8);
Template_ArithmeticsAddSafe(u16);
Template_ArithmeticsAddSafe(u32);
Template_ArithmeticsAddSafe(u64);

Template_ArithmeticsAddSafe(s8);
Template_ArithmeticsAddSafe(s16);
Template_ArithmeticsAddSafe(s32);
Template_ArithmeticsAddSafe(s64);

Template_ArithmeticsSubSafe(u8);
Template_ArithmeticsSubSafe(u16);
Template_ArithmeticsSubSafe(u32);
Template_ArithmeticsSubSafe(u64);

Template_ArithmeticsSubSafe(s8);
Template_ArithmeticsSubSafe(s16);
Template_ArithmeticsSubSafe(s32);
Template_ArithmeticsSubSafe(s64);

Template_ArithmeticsMultSafe(u8);
Template_ArithmeticsMultSafe(u16);
Template_ArithmeticsMultSafe(u32);
Template_ArithmeticsMultSafe(u64);

Template_ArithmeticsMultSafe(s8);
Template_ArithmeticsMultSafe(s16);
Template_ArithmeticsMultSafe(s32);
Template_ArithmeticsMultSafe(s64);

Template_ArithmeticsUnsignedDivSafe(u8);
Template_ArithmeticsUnsignedDivSafe(u16);
Template_ArithmeticsUnsignedDivSafe(u32);
Template_ArithmeticsUnsignedDivSafe(u64);

Template_ArithmeticsSignedDivSafe(s8);
Template_ArithmeticsSignedDivSafe(s16);
Template_ArithmeticsSignedDivSafe(s32);
Template_ArithmeticsSignedDivSafe(s64);
