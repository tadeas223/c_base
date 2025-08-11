#include <c_base/base/errors/errors.h>
#include <c_base/base/memory/allocator.h>
#include <c_base/base/memory/memory.h>
#include <c_base/base/memory/objects.h>
#include <c_base/system.h>

u32 refs = 0;
/******************************
 * Class
 ******************************/
Class Class_construct(u64 id) {
  Class self;
  self.id = id;
  return self;
}

/******************************
 * Interface
 ******************************/
Interface Interface_construct(u64 id) {
  Interface self;
  self.id = id;
  return self;
}

bool Interface_initialized(Interface* self) { return self->id != 0; }

/******************************
 * ClassObject
 ******************************/
IdImpl(ClassObject)

// construct
ClassObject ClassObject_construct(
  void (*destroy)(void* self), Interface** interfaces) {
  ClassObject self;
  self.class = Class_construct(ClassObject_id);

  self.references = 1;
  refs++;
  self.destroy = destroy;
  self.interfaces = interfaces;
  return self;
}

ClassObject ClassObject_construct_extend(
  u32 id, void (*destroy)(void* self), Interface** interfaces) {
  ClassObject self;
  self.class = Class_construct(id);

  self.references = 1;
  refs++;
  self.destroy = destroy;
  self.interfaces = interfaces;
  return self;
}

// methods
void ClassObject_destroy(void* self) {
  if (self == null)
    return;
  ClassObject* self_cast = self;
  refs -= self_cast->references;
  self_cast->destroy(self);
}

void* ClassObject_ref(void* self) {
  if (self == null)
    return null;
  ClassObject* self_cast = self;
  self_cast->references++;
  refs++;
  return self;
}

void ClassObject_unref(void* self) {
  if (self == null) {
    return;
  }
  ClassObject* self_cast = self;
  if (self_cast->references == 0) {
    crash(E(EG_Memory, E_Unspecified,
      SV("ClassObject_unref -> object was already destroyed")));
  }
  self_cast->references--;
  refs--;
  if (self_cast->references == 0) {
    self_cast->destroy(self);
    deallocate(self);
  }
}

void* ClassObject_pass(void* self) {
  if (self == null)
    return null;
  ClassObject* self_cast = self;
  self_cast->references--;
  refs--;
  return self;
}

bool ClassObject_contains_interface(void* self, u64 id) {
  ClassObject* self_cast = self;
  if (self_cast->interfaces == null) {
    return false;
  }

  u32 i = 0;
  while (self_cast->interfaces[i]) {
    if (self_cast->interfaces[i]->id == id) {
      return true;
    }
    i++;
  }
  return false;
}

Interface* ClassObject_get_interface(void* self, u64 id) {
  ClassObject* self_cast = self;
  u32 i = 0;
  if (self_cast->interfaces == null) {
    goto crash;
  }

  while (self_cast->interfaces[i]) {
    if (self_cast->interfaces[i]->id == id) {
      return self_cast->interfaces[i];
    }
    i++;
  }

crash:
  crash(E(EG_Unspecified, E_InvalidArgument,
    SV("ClassObject_get_interfaces -> object does not implement this "
       "interface")));
  return null;
}

/******************************
  IHashable
 ******************************/
IdImpl(IHashable)

// construct
IHashable IHashable_construct(
  bool (*equals)(void* a, void* b), u32 (*hash)(void* self)) {
  IHashable self;
  self.interface = Interface_construct(IHashable_id);

  self.equals = equals;
  self.hash = hash;

  return self;
}

// methods
bool IHashable_equals(void* a, void* b) {
  if (a == b)
    return true;
  IHashable* i_hashable =
    (IHashable*)ClassObject_get_interface(a, IHashable_id);
  return i_hashable->equals(a, b);
}

u32 IHashable_hash(void* self) {
  IHashable* i_hashable =
    (IHashable*)ClassObject_get_interface(self, IHashable_id);
  return i_hashable->hash(self);
}

/******************************
 * other
 ******************************/
u32 hash(void* ptr, u64 size) {
  u8* bytes = ptr;

  u64 hash = 2166136261;
  u64 prime = 16777619;

  for (u64 i = 0; i < size; ++i) {
    hash ^= bytes[i];
    hash *= prime;
  }

  return hash;
}

/******************************
 * C_Ptr
 ******************************/
struct C_Ptr {
  ClassObject base;

  void* ptr;
};

C_Ptr* C_Ptr_new(void* ptr) {
  C_Ptr* self = allocate(sizeof(C_Ptr));
  self->base = ClassObject_construct(C_Ptr_destroy, null);

  self->ptr = ptr;
  return self;
}

C_Ptr* C_Ptr_new_size(u64 size) {
  C_Ptr* self = allocate(sizeof(C_Ptr));
  self->base = ClassObject_construct(C_Ptr_destroy, null);

  self->ptr = allocate(size);
  return self;
}

void* C_Ptr_get_ptr(C_Ptr* self) { return self->ptr; }

void C_Ptr_destroy(void* self) {
  C_Ptr* self_cast = self;
  deallocate(self_cast->ptr);
}
