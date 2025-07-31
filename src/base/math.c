#include <c_base/base/errors/errors.h>
#include <c_base/base/errors/results.h>
#include <c_base/base/macros.h>
#include <c_base/base/math.h>
#include <c_base/base/memory/handles.h>
#include <c_base/base/memory/objects.h>
#include <c_base/system.h>

GenericValImpl_ErrorCode(EG_Math)

f64 math_abs(f64 val) {
  if (val < 0) {
    val *= -1;
  }

  return val;
}

f64 math_pow(f64 num, f64 power) {
  f64 result = num;

  while (--power) {
    result *= num;
  }

  return result;
}

u32 math_simple_log(u32 x, u32 base) {
  u32 result = 0;

  while (x >= 1) {
    x /= base;
    result++;
  }

  return result;
}

f64 math_ln(f64 x) {
#define TAYLOR_ITERATIONS 20
  s32 power_adjust = 0;
  u32 k;
  f64 result = 0.0;
  f64 y;
  f64 term;

  if (x <= 0) {
    crash(E(EG_Math, E_InvalidArgument,
            SV("logarithm is defined only for numbers grater than 0")));
  }

  while (x > 1.0) {
    x /= MATH_E;
    power_adjust++;
  }

  while (x < 0.25) {
    x *= MATH_E;
    power_adjust--;
  }

  y = x - 1.0;
  term = y;

  for (k = 1; k <= TAYLOR_ITERATIONS; k++) {
    result += term / k;
    term *= -y;
  }

  return result + power_adjust;

#undef TAYLOR_ITERATIONS
}

f64 math_log(f64 x, f64 base) { return math_ln(x) / math_ln(base); }

#define AddSafeS(T)                                                            \
  C_Result* Concat(T, _add_safe_R)(T a, T b) {                                 \
    C_Result* result;                                                          \
    if ((b > 0 && a > Concat(T, _MAX) - b) ||                                  \
        (b < 0 && a < Concat(T, _MIN) - b)) {                                  \
      result = C_Result_new_err(                                               \
          E(EG_Math, E_OutOfBounds, SV("T_add_safe -> would overflow")));      \
      goto ret;                                                                \
    }                                                                          \
                                                                               \
    C_Handle_##T* handle = Concat(C_Handle_##T, _new)(a + b);                  \
    result = C_Result_new_ok_P(handle);                                        \
    Unref(handle);                                                             \
                                                                               \
    goto ret;                                                                  \
  ret:                                                                         \
    return result;                                                             \
  }

#define AddSafeU(T)                                                            \
  C_Result* Concat(T, _add_safe_R)(T a, T b) {                                 \
    C_Result* result;                                                          \
    if (a > Concat(T, _MAX) - b) {                                             \
      result = C_Result_new_err(                                               \
          E(EG_Math, E_OutOfBounds, SV("T_add_safe -> would overflow")));      \
      goto ret;                                                                \
    }                                                                          \
                                                                               \
    C_Handle_##T* handle = Concat(C_Handle_##T, _new)(a + b);                  \
    result = C_Result_new_ok_P(handle);                                        \
    Unref(handle);                                                             \
                                                                               \
  ret:                                                                         \
    return result;                                                             \
  }

#define Add(T)                                                                 \
  T Concat(T, _add)(T a, T b) {                                                \
    C_Result* result = Concat(T, _add_safe_R)(a, b);                           \
                                                                               \
    T value = Concat(C_Handle_##T, _get_value)(C_Result_force_B(result));      \
                                                                               \
    Unref(result);                                                             \
    return value;                                                              \
  }

#define SubSafeS(T)                                                            \
  C_Result* Concat(T, _sub_safe_R)(T a, T b) {                                 \
    C_Result* result;                                                          \
    if ((b > 0 && a < Concat(T, _MIN) + b) ||                                  \
        (b < 0 && a > Concat(T, _MAX) + b)) {                                  \
      result = C_Result_new_err(                                               \
          E(EG_Math, E_OutOfBounds, SV("T_sub_safe -> would overflow")));      \
      goto ret;                                                                \
    }                                                                          \
                                                                               \
    C_Handle_##T* handle = Concat(C_Handle_##T, _new)(a - b);                  \
    result = C_Result_new_ok_P(handle);                                        \
    Unref(handle);                                                             \
                                                                               \
  ret:                                                                         \
    return result;                                                             \
  }

#define SubSafeU(T)                                                            \
  C_Result* Concat(T, _sub_safe_R)(T a, T b) {                                 \
    C_Result* result;                                                          \
    if (a > b) {                                                               \
      result = C_Result_new_err(                                               \
          E(EG_Math, E_OutOfBounds, SV("T_sub_safe -> would overflow")));      \
      goto ret;                                                                \
    }                                                                          \
                                                                               \
    C_Handle_##T* handle = Concat(C_Handle_##T, _new)(a - b);                  \
    result = C_Result_new_ok_P(handle);                                        \
    Unref(handle);                                                             \
  ret:                                                                         \
    return result;                                                             \
  }

#define Sub(T)                                                                 \
  T Concat(T, _sub)(T a, T b) {                                                \
    C_Result* result = Concat(T, _sub_safe_R)(a, b);                           \
                                                                               \
    T value = Concat(C_Handle_##T, _get_value)(C_Result_force_B(result));      \
                                                                               \
    Unref(result);                                                             \
    return value;                                                              \
  }

#define MultSafeS(T)                                                           \
  C_Result* Concat(T, _mult_safe_R)(T a, T b) {                                \
    C_Result* result;                                                          \
    if (a > 0) {                                                               \
      if (b > 0) {                                                             \
        if (a > Concat(T, _MAX) / b) {                                         \
          result = C_Result_new_err(                                           \
              E(EG_Math, E_OutOfBounds, SV("T_mult_safe -> would overflow"))); \
          goto ret;                                                            \
        }                                                                      \
      } else {                                                                 \
        if (b < Concat(T, _MIN) / a) {                                         \
          result = C_Result_new_err(                                           \
              E(EG_Math, E_OutOfBounds, SV("T_mult_safe -> would overflow"))); \
          goto ret;                                                            \
        }                                                                      \
      }                                                                        \
    } else {                                                                   \
      if (b > 0) {                                                             \
        if (a < Concat(T, _MIN) / b) {                                         \
          result = C_Result_new_err(                                           \
              E(EG_Math, E_OutOfBounds, SV("T_mult_safe -> would overflow"))); \
          goto ret;                                                            \
        } else {                                                               \
          if (a != 0 && b < Concat(T, _MAX) / a) {                             \
            result = C_Result_new_err(E(EG_Math, E_OutOfBounds,                \
                                        SV("T_mult_safe -> would overflow"))); \
            goto ret;                                                          \
          }                                                                    \
        }                                                                      \
      }                                                                        \
    }                                                                          \
                                                                               \
    C_Handle_##T* handle = Concat(C_Handle_##T, _new)(a * b);                  \
    result = C_Result_new_ok_P(handle);                                        \
    Unref(handle);                                                             \
                                                                               \
  ret:                                                                         \
    return result;                                                             \
  }

#define MultSafeU(T)                                                           \
  C_Result* Concat(T, _mult_safe_R)(T a, T b) {                                \
    C_Result* result;                                                          \
    if (b != 0 && a > Concat(T, _MAX) / b) {                                   \
      result = C_Result_new_err(                                               \
          E(EG_Math, E_OutOfBounds, SV("T_mult_safe -> would overflow")));     \
      goto ret;                                                                \
    }                                                                          \
                                                                               \
    C_Handle_##T* handle = Concat(C_Handle_##T, _new)(a * b);                  \
    result = C_Result_new_ok_P(handle);                                        \
    Unref(handle);                                                             \
                                                                               \
  ret:                                                                         \
    return result;                                                             \
  }

#define Mult(T)                                                                \
  T Concat(T, _mult)(T a, T b) {                                               \
    C_Result* result = Concat(T, _mult_safe_R)(a, b);                          \
                                                                               \
    T value = Concat(C_Handle_##T, _get_value)(C_Result_force_B(result));      \
                                                                               \
    Unref(result);                                                             \
    return value;                                                              \
  }

#define DivSafeS(T)                                                            \
  C_Result* Concat(T, _div_safe_R)(T a, T b) {                                 \
    C_Result* result;                                                          \
    if (b == 0) {                                                              \
      result = C_Result_new_err(                                               \
          E(EG_Math, E_InvalidArgument, SV("_safe_div -> division by zero"))); \
      goto ret;                                                                \
    }                                                                          \
    if (a == Concat(T, _MIN) && b == -1) {                                     \
      result = C_Result_new_err(                                               \
          E(EG_Math, E_InvalidArgument, SV("_safe_div -> would overflow")));   \
      goto ret;                                                                \
    }                                                                          \
                                                                               \
    C_Handle_##T* handle = Concat(C_Handle_##T, _new)(a / b);                  \
    result = C_Result_new_ok_P(handle);                                        \
    Unref(handle);                                                             \
                                                                               \
  ret:                                                                         \
    return result;                                                             \
  }

#define DivSafeU(T)                                                            \
  C_Result* Concat(T, _div_safe_R)(T a, T b) {                                 \
    C_Result* result;                                                          \
    if (b == 0) {                                                              \
      result = C_Result_new_err(                                               \
          E(EG_Math, E_InvalidArgument, SV("_safe_div -> division by zero"))); \
      goto ret;                                                                \
    }                                                                          \
                                                                               \
    C_Handle_##T* handle = Concat(C_Handle_##T, _new)(a / b);                  \
    result = C_Result_new_ok_P(handle);                                        \
    Unref(handle);                                                             \
                                                                               \
  ret:                                                                         \
    return result;                                                             \
  }

#define Div(T)                                                                 \
  T Concat(T, _div)(T a, T b) {                                                \
    C_Result* result = Concat(T, _div_safe_R)(a, b);                           \
                                                                               \
    T value = Concat(C_Handle_##T, _get_value)(C_Result_force_B(result));      \
                                                                               \
    Unref(result);                                                             \
    return value;                                                              \
  }

AddSafeS(s32)
AddSafeS(s64)

AddSafeU(u32)
AddSafeU(u64)

Add(u32)
Add(u64)
Add(s32)
Add(s64)

SubSafeS(s32)
SubSafeS(s64)

SubSafeU(u32)
SubSafeU(u64)

Sub(u32)
Sub(u64)
Sub(s32)
Sub(s64)

MultSafeS(s32)
MultSafeS(s64)

MultSafeU(u32)
MultSafeU(u64)

Mult(u32)
Mult(u64)
Mult(s32)
Mult(s64)

DivSafeS(s32)
DivSafeS(s64)

DivSafeU(u32)
DivSafeU(u64)

Div(u32)
Div(u64)
Div(s32)
Div(s64)
