#ifndef OBJECTS_H
#define OBJECTS_H

#include "base/memory/allocator.h"
#include "base/macros.h"
#include "base/types.h"
#include <stdio.h>

#define Wrap(T, value) Concat(C_Handle_##T, _new)(value)
#define Unwrap(T, val) (Concat(C_Handle_##T, _value)(val))

#define GenericType_C_Handle(T)\
typedef struct C_Handle_##T C_Handle_##T;\
C_Handle_##T* Concat(C_Handle_##T, _new)(T value);\
void Concat(C_Handle_##T, _destroy)(void* self);\
\
T Concat(C_Handle_##T, _value)(C_Handle_##T* self);

#define GenericTypeImpl_C_Handle(T)\
struct C_Handle_##T {\
  ComplexBase base;\
  T value;\
};\
\
C_Handle_##T* Concat(C_Handle_##T, _new)(T value) {\
  C_Handle_##T* self = allocate(sizeof(C_Handle_##T));\
  new(self, Concat(C_Handle_##T, _destroy));\
\
  self->value = value;\
\
  return self;\
}\
void Concat(C_Handle_##T, _destroy)(void* self) {(void)self;}\
T Concat(C_Handle_##T, _value)(C_Handle_##T* self) {\
  return self->value;\
}

#define Type(T)\
GenericType_C_Handle(T)

#define TypeImpl(T)\
GenericTypeImpl_C_Handle(T)

#define Using(T, var, code)\
do {\
  T value = var;\
  { code }\
  unref(var);\
} while(0);

extern u32 refs;

typedef void (*DestroyFunc)(void* self);
Type(DestroyFunc)

typedef struct {
  u32 references;
  DestroyFunc destroy_func;
} ComplexBase;
Type(ComplexBase)

ComplexBase ComplexBase_construct(DestroyFunc destroy_func);

void new(void* obj, DestroyFunc destroy_func);
void* ref(void* obj);
void* unref(void* obj);
void destroy(void* obj);

// {{{ Type
Type(u8)
Type(u16)
Type(u32)
Type(u64)

Type(s8)
Type(s16)
Type(s32)
Type(s64)

Type(b8)
Type(b16)
Type(b32)
Type(b64)
// }}}

#endif
