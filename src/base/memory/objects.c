#include "base/memory/objects.h"
#include "base/memory/allocator.h"

// {{{ TypeImpl
TypeImpl(ComplexBase)
TypeImpl(DestroyFunc)
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
// }}}

u32 refs = 0;

ComplexBase ComplexBase_construct(DestroyFunc destroy_func) {
  ComplexBase self;
  self.references = 1;
  self.destroy_func = destroy_func;
  return self;
}

void new(void* obj, DestroyFunc destroy_func) {
  ComplexBase* base = obj;
  *base = ComplexBase_construct(destroy_func);
}

void* ref(void* obj) {
  if(obj == null) goto ret;

  ComplexBase* base = obj;
  base->references++;
  refs++;

ret:
  return obj;
}

void* unref(void* obj) {
  if(obj == null) goto ret;

  ComplexBase* base = obj;
    
  base->references--;
  refs--;
  
  if(base->references == 0) {
    base->destroy_func(obj); 
    deallocate(obj);
  }

ret:
  return obj;
}

void destroy(void* obj) {
  if(obj == null) return;
  ComplexBase* base = obj;
  base->destroy_func(obj);
  refs -= base->references;
  deallocate(obj);
}
