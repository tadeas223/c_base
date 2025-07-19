#ifndef OBJECTS_H
#define OBJECTS_H

#include "c_base/base/types.h"

#define Wrap_R(T, val) Concat(C_Handle_##T, _new)(val)
#define PWrap_R(T, val) Pass(Concat(C_Handle_##T, _new)(val))
#define Unwrap(T, handle) Concat(C_Handle_##T, _get_value)(handle)

#define Type(T) GenericType_C_Handle(T)
#define TypeImpl(T) GenericTypeImpl_C_Handle(T)

#define Ref(obj) ClassObject_ref(obj)
#define Unref(obj) ClassObject_unref(obj)
#define Pass(obj) ClassObject_pass(obj)

typedef struct {
  u64 id;
} Interface;

typedef struct {
  u64 id;
} Class;

extern u32 refs;

/******************************
 * Class Object
 ******************************/
typedef struct {
  Class class;
  u32 references;
  void (*destroy)(void*);
  Interface** interfaces;
} ClassObject;
Id(ClassObject)

// construct
ClassObject ClassObject_construct(void (*destroy)(void*), Interface** interfaces);

// methods
void ClassObject_destroy(void* self);

void* ClassObject_ref(void* self);
void ClassObject_unref(void* self);
void* ClassObject_pass(void* self);

bool ClassObject_contains_interface(void* self, u64 id);
Interface* ClassObject_get_interface(void* self, u64 id);

/******************************
 * IHashable
 ******************************/
typedef struct {
  Interface interface;
  bool (*equals)(void* a, void* b);
  u32 (*hash)(void* self);
} IHashable;
Id(IHashable)

// construct
IHashable IHashable_construct(u64 id, bool (*equals)(void* a, void* b), u32 (*hash)(void* self));

// methods
bool IHashable_equals(void* a, void* b);
u32 IHashable_hash(void* self);


/******************************
 * C_Handle
 ******************************/
#define GenericType_C_Handle(T)\
typedef struct {\
  ClassObject base;\
\
  T value;\
} C_Handle_##T;\
\
C_Handle_##T* Concat(C_Handle_##T, _new)(T value);\
\
void Concat(C_Handle_##T, _destroy)(void* self);\
\
T Concat(C_Handle_##T, _get_value)(C_Handle_##T* self);\
\
u32 Concat(C_Handle_##T, _hash)(void* self);\
bool Concat(C_Handle_##T, _equals)(void* a, void* b);

#define GenericTypeImpl_C_Handle(T)\
static Interface* Concat(C_Handle_##T, _interfaces)[2];\
C_Handle_##T* Concat(C_Handle_##T, _new)(T value) {\
  static IHashable i_hashable = {0};\
  if(i_hashable.interface.id == 0) {\
    i_hashable = IHashable_construct(IHashable_id,\
        Concat(C_Handle_##T, _equals),\
        Concat(C_Handle_##T, _hash)\
      );\
    Concat(C_Handle_##T, _interfaces)[0] = (Interface*)&i_hashable;\
    Concat(C_Handle_##T, _interfaces)[1] = null;\
  }\
  \
  \
  C_Handle_##T* self = allocate(sizeof(C_Handle_##T));\
  self->base = ClassObject_construct(Concat(C_Handle_##T, _destroy), Concat(C_Handle_##T, _interfaces));\
\
  self->value = value;\
\
  return self;\
}\
\
void Concat(C_Handle_##T, _destroy)(void* self) {(void)self;}\
\
bool Concat(C_Handle_##T, _equals)(void* a, void* b) {\
  if(a == b) return true;\
  \
  C_Handle_##T* a_cast = a;\
  C_Handle_##T* b_cast = b;\
  \
  return mem_equals(&a_cast->value, &b_cast->value, sizeof(T));\
}\
\
u32 Concat(C_Handle_##T, _hash)(void* self) {\
  C_Handle_##T* self_cast = self;\
  return hash(&self_cast->value, sizeof(T));\
}\
\
\
T Concat(C_Handle_##T, _get_value)(C_Handle_##T* self) {\
  return self->value;\
}

/******************************
 * other
 ******************************/
u32 hash(void* ptr, u64 size);

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

#endif
