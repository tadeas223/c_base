#include "c_base/base/errors/errors.h"
#include "c_base/base/memory/memory.h"
#include "c_base/system.h"
#include <c_base/base/memory/objects.h>
#include <c_base/base/memory/allocator.h>

u32 refs = 0;

/******************************
 * Class Object
 ******************************/
IdImpl(ClassObject)

// construct
ClassObject ClassObject_construct(void (*destroy)(void* self), Interface** interfaces) {
  ClassObject self;
  self.class.id = ClassObject_id; 

  self.references = 1;
  refs++;
  self.destroy = destroy;
  self.interfaces = interfaces;
  return self;
}

// methods
void ClassObject_destroy(void* self) {
  if(self == null) return;
  ClassObject* self_cast = self;
  refs -= self_cast->references;
  self_cast->destroy(self);
}

void* ClassObject_ref(void* self) {
  if(self == null) return null;
  ClassObject* self_cast = self;
  self_cast->references++;
  refs++;
  return self;
}

void ClassObject_unref(void* self) {
  if(self == null) return;
  ClassObject* self_cast = self;
  self_cast->references--;
  refs--;
  if(self_cast->references == 0) {
    self_cast->destroy(self);
    deallocate(self);
  }
}

void* ClassObject_pass(void* self) {
  if(self == null) return null;
  ClassObject* self_cast = self;
  self_cast->references--;
  refs--;
  return self;
}

bool ClassObject_contains_interface(void* self, u64 id) {
  ClassObject* self_cast = self;
  u32 i = 0;
  while(self_cast->interfaces[i]) {
    if(self_cast->interfaces[i]->id == id) {
      return true; 
    }
    i++;
  }
  return false;
}

Interface* ClassObject_get_interface(void* self, u64 id) {
  ClassObject* self_cast = self;
  u32 i = 0;
  if(self_cast->interfaces == null) {
    goto crash;  
  }

  while(self_cast->interfaces[i]) {
    if(self_cast->interfaces[i]->id == id) {
      return self_cast->interfaces[i]; 
    }
    i++;
  }

crash:
  crash(E(EG_Unspecified, E_InvalidArgument, SV("ClassObject_get_interfaces -> object does not implement this interface")));
  return null;
}


/******************************
  IHashable
 ******************************/
IdImpl(IHashable)

// construct
IHashable IHashable_construct(u64 id, bool (*equals)(void* a, void* b), u32 (*hash)(void* self)) {
  IHashable self;
  self.interface.id = id;

  self.equals = equals;
  self.hash = hash;
  
  return self;
}

// methods
bool IHashable_equals(void* a, void* b) {
  IHashable* i_hashable = (IHashable*)ClassObject_get_interface(a, IHashable_id);
  return i_hashable->equals(a, b);
}

u32 IHashable_hash(void* self) {
  IHashable* i_hashable = (IHashable*)ClassObject_get_interface(self, IHashable_id);
  return i_hashable->hash(self);
}

/******************************
 * other
 ******************************/
u32 hash(void* ptr, u64 size) {
  u8* bytes = ptr;

  u64 hash = 2166136261;
  u64 prime = 16777619;

  for(u64 i = 0; i < size; ++i) {
    hash ^= bytes[i];
    hash *= prime;
  }

  return hash;
}

TypeImpl(u8)
TypeImpl(u16)
TypeImpl(u32)
TypeImpl(u64)

TypeImpl(s8)
TypeImpl(s16)
TypeImpl(s32)
TypeImpl(s64)

TypeImpl(b8)
TypeImpl(b16)
TypeImpl(b32)
TypeImpl(b64)
