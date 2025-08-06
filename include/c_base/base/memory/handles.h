#ifndef HANDLES_H
#define HANDLES_H

#include <c_base/base/macros.h>
#include <c_base/base/memory/allocator.h>
#include <c_base/base/memory/memory.h>
#include <c_base/base/memory/objects.h>
#include <c_base/base/strings/string_convert.h>
#include <c_base/base/strings/strings.h>
#include <c_base/base/types.h>

#define GenericType_C_Handle(T)                                                \
  typedef struct C_Handle_##T C_Handle_##T;                                    \
  C_String* Concat(C_Handle_##T, _to_str_R)(void* self);                       \
  C_String* Concat(C_Handle_##T, _to_str_format_R)(void* self,                 \
                                                   C_String* format);          \
  u32 Concat(C_Handle_##T, _hash)(void* self);                                 \
  bool Concat(C_Handle_##T, _equals)(void* a, void* b);                        \
                                                                               \
  C_Handle_##T* Concat(C_Handle_##T, _new)(T value);                           \
  void Concat(C_Handle_##T, _destroy)(void* self);                             \
  T Concat(C_Handle_##T, _get_value)(C_Handle_##T * self);

#define GenericTypeImpl_C_Handle(T, to_str_func, to_str_format_func)           \
  struct C_Handle_##T {                                                        \
    ClassObject base;                                                          \
                                                                               \
    T value;                                                                   \
  };                                                                           \
  static IHashable Concat(C_Handle_##T, _i_hashable) = {0};                    \
  static IFormattable Concat(C_Handle_##T, _i_formattable) = {0};              \
  static Interface* Concat(C_Handle_##T, _interfaces)[3];                      \
                                                                               \
  u32 Concat(C_Handle_##T, _hash)(void* self) {                                \
    C_Handle_##T* self_cast = self;                                            \
    return hash(&self_cast->value, sizeof(T));                                 \
  }                                                                            \
                                                                               \
  bool Concat(C_Handle_##T, _equals)(void* a, void* b) {                       \
    if (a == b)                                                                \
      return true;                                                             \
                                                                               \
    C_Handle_##T* a_cast = a;                                                  \
    C_Handle_##T* b_cast = b;                                                  \
                                                                               \
    return mem_equals(&a_cast->value, &b_cast->value, sizeof(T));              \
  }                                                                            \
                                                                               \
  void Concat(C_Handle_##T, _destroy)(void* self) { (void)self; }              \
                                                                               \
  C_String* Concat(C_Handle_##T, _to_str_R)(void* self) {                      \
    C_Handle_##T* self_cast = self;                                            \
    return to_str_func(self_cast->value);                                      \
  }                                                                            \
  C_String* Concat(C_Handle_##T, _to_str_format_R)(void* self,                 \
                                                   C_String* format) {         \
    C_Handle_##T* self_cast = self;                                            \
    return to_str_format_func(self_cast->value, format);                       \
  }                                                                            \
                                                                               \
  C_Handle_##T* Concat(C_Handle_##T, _new)(T value) {                          \
    if (!Interface_initialized(                                                \
            (Interface*)&Concat(C_Handle_##T, _i_hashable))) {                 \
      Concat(C_Handle_##T, _i_hashable) = IHashable_construct(                 \
          Concat(C_Handle_##T, _equals), Concat(C_Handle_##T, _hash));         \
      Concat(C_Handle_##T, _i_formattable) = IFormattable_construct_format(    \
          Concat(C_Handle_##T, _to_str_R),                                     \
          Concat(C_Handle_##T, _to_str_format_R));                             \
                                                                               \
      Concat(C_Handle_##T, _interfaces)[0] =                                   \
          (Interface*)&Concat(C_Handle_##T, _i_hashable);                      \
      Concat(C_Handle_##T, _interfaces)[1] =                                   \
          (Interface*)&Concat(C_Handle_##T, _i_formattable);                   \
      Concat(C_Handle_##T, _interfaces)[2] = null;                             \
    }                                                                          \
                                                                               \
    C_Handle_##T* self = allocate(sizeof(C_Handle_##T));                       \
    self->base = ClassObject_construct(Concat(C_Handle_##T, _destroy),         \
                                       Concat(C_Handle_##T, _interfaces));     \
                                                                               \
    self->value = value;                                                       \
                                                                               \
    return self;                                                               \
  }                                                                            \
                                                                               \
  T Concat(C_Handle_##T, _get_value)(C_Handle_##T * self) {                    \
    return self->value;                                                        \
  }

GenericType_C_Handle(u8)
GenericType_C_Handle(u16)
GenericType_C_Handle(u32)
GenericType_C_Handle(u64)

GenericType_C_Handle(s8)
GenericType_C_Handle(s16)
GenericType_C_Handle(s32)
GenericType_C_Handle(s64)

GenericType_C_Handle(b8)
GenericType_C_Handle(b16)
GenericType_C_Handle(b32)
GenericType_C_Handle(b64)

GenericType_C_Handle(bool)

#endif
