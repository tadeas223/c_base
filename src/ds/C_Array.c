#include <c_base/base/errors/errors.h>
#include <c_base/base/memory/allocator.h>
#include <c_base/base/memory/memory.h>
#include <c_base/base/memory/objects.h>
#include <c_base/base/strings/strings.h>
#include <c_base/ds/C_Array.h>
#include <c_base/system.h>

static Interface* C_Array_interfaces[3];
static IFormattable C_Array_i_formattable = {0};
static IHashable C_Array_i_hashable = {0};

struct C_Array {
  ClassObject base;
  void** data;
  u32 len;
};

/******************************
 * new/dest
 ******************************/
C_Array* C_Array_new(u32 len) {
  if (!Interface_initialized((Interface*)&C_Array_i_formattable)) {
    C_Array_i_formattable = IFormattable_construct_format(
        C_Array_to_str_R, C_Array_to_str_format_R);

    C_Array_i_hashable = IHashable_construct(C_Array_equals, C_Array_hash);

    C_Array_interfaces[0] = (Interface*)&C_Array_i_formattable;
    C_Array_interfaces[1] = (Interface*)&C_Array_i_hashable;
    C_Array_interfaces[2] = null;
  }

  C_Array* self = allocate(sizeof(C_Array));
  self->base = ClassObject_construct(C_Array_destroy, C_Array_interfaces);

  self->len = len;
  self->data = allocate(len * sizeof(void*));

  mem_set(self->data, 0, len * sizeof(void*));

  return self;
}

void C_Array_destroy(void* self) {
  C_Array* self_cast = self;
  C_ArrayForeach(self_cast, { Unref(value); });

  deallocate(self_cast->data);
}

/******************************
 * logic
 ******************************/
void C_Array_put_P(C_Array* self, u32 index, void* value) {
  if (index >= self->len) {
    crash(E(EG_Datastructures, E_OutOfBounds,
            SV("C_Array_put -> index is outside of the array")));
  }

  Ref(value);
  Unref(self->data[index]);
  self->data[index] = value;
}

static void* __C_Array_at(C_Array* self, u32 index) {
  if (index >= self->len) {
    crash(E(EG_Datastructures, E_OutOfBounds,
            SV("__C_Array_at -> index is outside of the array")));
  }

  Ref(self->data[index]);
  return self->data[index];
}

static void* __C_Array_peek(C_Array* self) {
  if (self->len == 0) {
    crash(E(EG_Datastructures, E_OutOfBounds,
            SV("__C_Array_peek -> array is empty")));
  }

  Ref(self->data[self->len - 1]);
  return self->data[self->len - 1];
}

static void* __C_Array_peek_front(C_Array* self) {
  if (self->len == 0) {
    crash(E(EG_Datastructures, E_OutOfBounds,
            SV("__C_Array_peek_front -> array is empty")));
  }

  Ref(self->data[0]);
  return self->data[0];
}

u32 C_Array_hash(void* self) {
  u32 hash_code = 0;
  C_ArrayForeach(self, { hash_code = 31 * hash_code + IHashable_hash(value); });
  return hash_code;
}

bool C_Array_equals(void* a, void* b) {
  C_Array* a_cast = a;
  C_Array* b_cast = b;

  if (a_cast->len != b_cast->len) {
    return false;
  }

  C_ArrayForeach(a_cast, {
    if (!IHashable_equals(C_Array_at_B(b_cast, iter), value)) {
      return false;
    }
  });

  return true;
}

/******************************
 * get/set
 ******************************/
u32 C_Array_get_len(C_Array* self) { return self->len; }

// {{{ _B _R wrappers
/******************************
 * _B _R wrappers
 ******************************/
void* C_Array_at_B(C_Array* self, u32 index) {
  void* result = __C_Array_at(self, index);
  Unref(result);
  return result;
}

void* C_Array_at_R(C_Array* self, u32 index) {
  void* result = __C_Array_at(self, index);
  return result;
}

void* C_Array_peek_B(C_Array* self) {
  void* result = __C_Array_peek(self);
  Unref(result);
  return result;
}

void* C_Array_peek_R(C_Array* self) {
  void* result = __C_Array_peek(self);
  return result;
}

void* C_Array_peek_front_B(C_Array* self) {
  void* result = __C_Array_peek_front(self);
  Unref(result);
  return result;
}

void* C_Array_peek_front_R(C_Array* self) {
  void* result = __C_Array_peek_front(self);
  return result;
}

// }}}
