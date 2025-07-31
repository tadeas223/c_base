#include "c_base/ds/darray.h"
#include "c_base/base/errors/errors.h"
#include "c_base/base/memory/allocator.h"
#include "c_base/base/memory/memory.h"
#include "c_base/base/memory/objects.h"
#include "c_base/system.h"

GenericValImpl_ErrorCode(EG_DArray)

struct C_DArray {
  ClassObject base;

  u32 cap;
  u32 len;
  void** data;
};

C_DArray* C_DArray_new(void) { return C_DArray_new_cap(1); }

C_DArray* C_DArray_new_cap(u32 cap) {
  C_DArray* self = allocate(sizeof(C_DArray));
  self->base = ClassObject_construct(C_Darray_destroy, null);

  self->cap = cap;
  self->len = 0;
  self->data = allocate(self->cap * sizeof(void*));

  return self;
}

void C_Darray_destroy(void* self) {
  C_DArray* self_cast = self;
  deallocate(self_cast->data);
}

void C_DArray_push_P(C_DArray* self, void* value) {
  if (self->len + 1 > self->cap) {
    C_DArray_resize(self, self->cap * 2);
  }

  self->data[self->len] = Ref(value);
  self->len++;
}

void C_DArray_push_front_P(C_DArray* self, void* value) {
  if (self->len + 1 > self->cap) {
    C_DArray_resize(self, self->cap * 2);
  }

  mem_copy(self->data + 1, self->data, self->len * sizeof(void*));
  self->data[0] = Ref(value);

  self->len++;
}

void* C_DArray_pop_R(C_DArray* self) {
  if (self->len == 0) {
    crash(E(EG_DArray, E_OutOfBounds, SV("C_DArray_pop_R -> darray is empty")));
  }

  void* value = self->data[self->len - 1];
  self->data[self->len - 1] = null;
  self->len--;
  return value;
}

void* C_DArray_pop_front_R(C_DArray* self) {
  if (self->len == 0) {
    crash(E(EG_DArray, E_OutOfBounds,
            SV("C_DArray_pop_front_R -> darray is empty")));
  }

  void* value = self->data[0];
  self->data[0] = null;
  self->len--;

  mem_copy(self->data, self->data + 1, self->len + sizeof(void*));
  return value;
}

static void* __C_DArray_peek(C_DArray* self) {
  if (self->len == 0) {
    crash(
        E(EG_DArray, E_OutOfBounds, SV("__C_DArray_peek -> darray is empty")));
  }

  return Ref(self->data[self->len - 1]);
}

static void* __C_DArray_peek_front(C_DArray* self) {
  if (self->len == 0) {
    crash(E(EG_DArray, E_OutOfBounds,
            SV("__C_DArray_peek_front -> darray is empty")));
  }

  return Ref(self->data[0]);
}

void C_Darray_add_P(C_DArray* self, u32 index, void* value) {
  if (index > self->len) {
    crash(E(EG_DArray, E_OutOfBounds,
            SV("C_DArray_add_P -> index is outside of the darray")));
  }

  if (index == self->len) {
    C_DArray_push_P(self, value);
    return;
  }

  if (self->len + 1 > self->cap) {
    C_DArray_resize(self, self->cap * 2);
  }

  mem_copy(self->data + index + 1, self->data + index,
           (self->len - index) * sizeof(void*));
  self->data[index] = Ref(value);

  self->len++;
}

void* __C_Darray_at(C_DArray* self, u32 index) {
  if (index >= self->len) {
    crash(E(EG_DArray, E_OutOfBounds,
            SV("__C_DArray_at -> index is outside of the darray")));
  }

  return Ref(self->data[index]);
}

void* C_Darray_remove_R(C_DArray* self, u32 index) {
  if (index >= self->len) {
    crash(E(EG_DArray, E_OutOfBounds,
            SV("C_DArray_remove_R -> index is outside of the darray")));
  }

  if (self->len + 1 < self->cap) {
    C_DArray_resize(self, self->cap * 2);
  }

  void* result = self->data[index];
  mem_copy(self->data + index, self->data + index + 1,
           (self->len - index - 1) * sizeof(void*));

  self->len--;
  return result;
}

void C_DArray_resize(C_DArray* self, u32 cap) {
  if (cap < self->len) {
    crash(E(EG_DArray, E_OutOfBounds,
            SV("C_DArray_resize -> darray length would be grater that its "
               "capacity")));
  }
  if (cap == 0) {
    crash(E(EG_DArray, E_OutOfBounds,
            SV("C_DArray_resize -> capacity must be at least one")));
  }

  self->data = reallocate(self->data, cap * sizeof(void*));
  self->cap = cap;
}

void C_DArray_compress(C_DArray* self) { C_DArray_resize(self, self->len); }

void C_DArray_clear(C_DArray* self) {
  C_DArrayForeach(self, { Unref(value); });

  self->len = 0;
  C_DArray_resize(self, 1);
}

u32 C_DArray_get_cap(C_DArray* self) { return self->cap; }

u32 C_DArray_get_len(C_DArray* self) { return self->len; }

// {{{ _R _B wrappers
void* C_DArray_peek_R(C_DArray* self) {
  void* result = __C_DArray_peek(self);
  return result;
}

void* C_DArray_peek_B(C_DArray* self) {
  void* result = __C_DArray_peek(self);
  Unref(result);
  return result;
}

void* C_DArray_peek_front_R(C_DArray* self) {
  void* result = __C_DArray_peek_front(self);
  return result;
}

void* C_DArray_peek_front_B(C_DArray* self) {
  void* result = __C_DArray_peek_front(self);
  Unref(result);
  return result;
}

void* C_Darray_at_R(C_DArray* self, u32 index) {
  void* result = __C_Darray_at(self, index);
  return result;
}
void* C_Darray_at_B(C_DArray* self, u32 index) {
  void* result = __C_Darray_at(self, index);
  Unref(result);
  return result;
}

// }}}
