#ifndef OBJECTS_H
#define OBJECTS_H

#include "c_base/os/os_threads.h"
#include <c_base/base/types.h>

#define Ref(obj) ClassObject_ref(obj)
#define Unref(obj) ClassObject_unref(obj)
#define Pass(obj) ClassObject_pass(obj)

#define Lock(obj) ClassObject_lock(obj)
#define Unlock(obj) ClassObject_unlock(obj)

typedef struct {
  u64 id;
} Interface;

typedef struct {
  u64 id;
} Class;

extern u32 refs;

/******************************
 * Class
 ******************************/
Class Class_construct(u64 id);

/******************************
 * Interface
 ******************************/
Interface Interface_construct(u64 id);
bool Interface_initialized(Interface* self);

/****************************** ClassObject
 ******************************/
typedef struct {
  Class class;
  u32 references;
  Mutex mutex;
  void (*destroy)(void*);
  Interface** interfaces;
} ClassObject;
Id(ClassObject)

// construct
ClassObject ClassObject_construct(
  void (*destroy)(void*), Interface** interfaces);
ClassObject ClassObject_construct_extend(
  u32 id, void (*destroy)(void* self), Interface** interfaces);

// methods
void ClassObject_destroy(void* self);

void* ClassObject_ref(void* self);
void ClassObject_unref(void* self);
void* ClassObject_pass(void* self);

void ClassObject_lock(void* self);
void ClassObject_unlock(void* self);

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
IHashable IHashable_construct(
  bool (*equals)(void* a, void* b), u32 (*hash)(void* self));

// methods
bool IHashable_equals(void* a, void* b);
u32 IHashable_hash(void* self);

/******************************
 * other
 ******************************/
u32 hash(void* ptr, u64 size);

/******************************
 * C_Ptr
 ******************************/
typedef struct C_Ptr C_Ptr;

C_Ptr* C_Ptr_new(void* ptr);
C_Ptr* C_Ptr_new_size(u64 size);

void* C_Ptr_get_ptr(C_Ptr* self);

void C_Ptr_destroy(void* self);

#endif
