#ifndef MATH_H
#define MATH_H

#include "base/errors/results.h"
#include "base/types.h"
#include "base/errors/errors.h"

#define MATH_E  2.718281828459045235
#define MATH_PI 3.14159265358979323846

GenericVal_ErrorCode(EG_Math)

/* this guy is a fucking legend!!! ---> https://www.youtube.com/watch?v=bIDOTWy7gZk */

f64 math_abs(f64 val);
u32 math_simple_log(u32 x, u32 base);
f64 math_log(f64 x, f64 base);
f64 math_ln(f64 x);
f64 math_pow(f64 x, f64 power);

C_Result* /* u32 */ u32_add_safe_R(u32 a, u32 b);
C_Result* /* u64 */ u64_add_safe_R(u64 a, u64 b);
C_Result* /* s32 */ s32_add_safe_R(s32 a, s32 b);
C_Result* /* s64 */ s64_add_safe_R(s64 a, s64 b);

u32 u32_add(u32 a, u32 b);
u64 u64_add(u64 a, u64 b);
s32 s32_add(s32 a, s32 b);
s64 s64_add(s64 a, s64 b);

C_Result* /* u32 */ u32_sub_safe_R(u32 a, u32 b);
C_Result* /* u64 */ u64_sub_safe_R(u64 a, u64 b);
C_Result* /* s32 */ s32_sub_safe_R(s32 a, s32 b);
C_Result* /* s64 */ s64_sub_safe_R(s64 a, s64 b);

u32 u32_sub(u32 a, u32 b);
u64 u64_sub(u64 a, u64 b);
s32 s32_sub(s32 a, s32 b);
s64 s64_sub(s64 a, s64 b);

C_Result* /* u32 */ u32_mult_safe_R(u32 a, u32 b);
C_Result* /* u64 */ u64_mult_safe_R(u64 a, u64 b);
C_Result* /* s32 */ s32_mult_safe_R(s32 a, s32 b);
C_Result* /* s64 */ s64_mult_safe_R(s64 a, s64 b);

u32 u32_mult(u32 a, u32 b);
u64 u64_mult(u64 a, u64 b);
s32 s32_mult(s32 a, s32 b);
s64 s64_mult(s64 a, s64 b);

C_Result* /* u32 */ u32_div_safe_R(u32 a, u32 b);
C_Result* /* u64 */ u64_div_safe_R(u64 a, u64 b);
C_Result* /* s32 */ s32_div_safe_R(s32 a, s32 b);
C_Result* /* s64 */ s64_div_safe_R(s64 a, s64 b);

u32 u32_div(u32 a, u32 b);
u64 u64_div(u64 a, u64 b);
s32 s32_div(s32 a, s32 b);
s64 s64_div(s64 a, s64 b);

#endif
