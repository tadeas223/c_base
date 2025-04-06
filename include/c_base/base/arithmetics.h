#ifndef ARITHMETICS_H
#define ARITHMETICS_H

#include "base/types.h"
#include "base/errors.h"

Extern_ErrorGroup(EG_Arithmetics);
Extern_Error(E_Overflow);
Extern_Error(E_DivisionByZero);


f32 f32_log(u64 exp, f32 num);
f64 f64_log(u64 exp, f64 num);
f64 f64_log10(f64 num);
f32 f32_log10(f32 num);
f32 f32_floor(f32 num);
f64 f64_floor(f64 num);

#define GenericFunc_Add(type, a, b) type Concat(type, _add)(type a, type b)
#define GenericFunc_Sub(type, a, b) type Concat(type, _sub)(type a, type b)
#define GenericFunc_Div(type, a, b) type Concat(type, _div)(type a, type b)
#define GenericFunc_Mult(type, a, b) type Concat(type, _mult)(type a, type b)

#define Add(type, a, b) Concat(type, _add)(a, b)
#define Sub(type, a, b) Concat(type, _sub)(a, b)
#define Div(type, a, b) Concat(type, _div)(a, b)
#define Mult(type, a, b) Concat(type, _mult)(a, b)

#define GenericFunc_AddSafe(type, a, b) Result(type) Concat(type, _add_safe)(type a, type b)
#define GenericFunc_SubSafe(type, a, b) Result(type) Concat(type, _sub_safe)(type a, type b)
#define GenericFunc_DivSafe(type, a, b) Result(type) Concat(type, _div_safe)(type a, type b)
#define GenericFunc_MultSafe(type, a, b) Result(type) Concat(type, _mult_safe)(type a, type b)

#define AddSafe(type, a, b) Concat(type, _add_safe)(a, b)
#define SubSafe(type, a, b) Concat(type, _sub_safe)(a, b)
#define DivSafe(type, a, b) Concat(type, _div_safe)(a, b)
#define MultSafe(type, a, b) Concat(type, _mult_safe)(a, b)

static inline GenericFunc_Add(u8, a, b)  {return a + b;}
static inline GenericFunc_Add(u16, a, b) {return a + b;}
static inline GenericFunc_Add(u32, a, b) {return a + b;}
static inline GenericFunc_Add(u64, a, b) {return a + b;}

static inline GenericFunc_Sub(u8, a, b)  {return a - b;}
static inline GenericFunc_Sub(u16, a, b) {return a - b;}
static inline GenericFunc_Sub(u32, a, b) {return a - b;}
static inline GenericFunc_Sub(u64, a, b) {return a - b;}

static inline GenericFunc_Div(u8, a, b)  {return a / b;}
static inline GenericFunc_Div(u16, a, b) {return a / b;}
static inline GenericFunc_Div(u32, a, b) {return a / b;}
static inline GenericFunc_Div(u64, a, b) {return a / b;}

static inline GenericFunc_Mult(u8, a, b)  {return a * b;}
static inline GenericFunc_Mult(u16, a, b) {return a * b;}
static inline GenericFunc_Mult(u32, a, b) {return a * b;}
static inline GenericFunc_Mult(u64, a, b) {return a * b;}

static inline GenericFunc_Add(s8, a, b)  {return a + b;}
static inline GenericFunc_Add(s16, a, b) {return a + b;}
static inline GenericFunc_Add(s32, a, b) {return a + b;}
static inline GenericFunc_Add(s64, a, b) {return a + b;}

static inline GenericFunc_Sub(s8, a, b)  {return a - b;}
static inline GenericFunc_Sub(s16, a, b) {return a - b;}
static inline GenericFunc_Sub(s32, a, b) {return a - b;}
static inline GenericFunc_Sub(s64, a, b) {return a - b;}

static inline GenericFunc_Div(s8, a, b)  {return a / b;}
static inline GenericFunc_Div(s16, a, b) {return a / b;}
static inline GenericFunc_Div(s32, a, b) {return a / b;}
static inline GenericFunc_Div(s64, a, b) {return a / b;}

static inline GenericFunc_Mult(s8, a, b)  {return a * b;}
static inline GenericFunc_Mult(s16, a, b) {return a * b;}
static inline GenericFunc_Mult(s32, a, b) {return a * b;}
static inline GenericFunc_Mult(s64, a, b) {return a * b;}

GenericFunc_AddSafe(u8,  a, b);
GenericFunc_AddSafe(u16, a, b);
GenericFunc_AddSafe(u32, a, b);
GenericFunc_AddSafe(u64, a, b);

GenericFunc_SubSafe(u8,  a, b);
GenericFunc_SubSafe(u16, a, b);
GenericFunc_SubSafe(u32, a, b);
GenericFunc_SubSafe(u64, a, b);

GenericFunc_DivSafe(u8,  a, b);
GenericFunc_DivSafe(u16, a, b);
GenericFunc_DivSafe(u32, a, b);
GenericFunc_DivSafe(u64, a, b);

GenericFunc_MultSafe(u8,  a, b);
GenericFunc_MultSafe(u16, a, b);
GenericFunc_MultSafe(u32, a, b);
GenericFunc_MultSafe(u64, a, b);

GenericFunc_AddSafe(s8,  a, b);
GenericFunc_AddSafe(s16, a, b);
GenericFunc_AddSafe(s32, a, b);
GenericFunc_AddSafe(s64, a, b);

GenericFunc_SubSafe(s8, a, b);
GenericFunc_SubSafe(s16, a, b);
GenericFunc_SubSafe(s32, a, b);
GenericFunc_SubSafe(s64, a, b);

GenericFunc_DivSafe(s8, a, b);
GenericFunc_DivSafe(s16, a, b);
GenericFunc_DivSafe(s32, a, b);
GenericFunc_DivSafe(s64, a, b);

GenericFunc_MultSafe(s8, a, b);
GenericFunc_MultSafe(s16, a, b);
GenericFunc_MultSafe(s32, a, b);
GenericFunc_MultSafe(s64, a, b);
#endif
