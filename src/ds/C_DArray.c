#include "c_base/base/strings/format.h"
#include "c_base/base/strings/strings.h"
#include "c_base/ds/C_Array.h"
#include "c_base/ds/C_List.h"
#include <c_base/base/errors/errors.h>
#include <c_base/base/memory/allocator.h>
#include <c_base/base/memory/memory.h>
#include <c_base/base/memory/objects.h>
#include <c_base/ds/C_DArray.h>
#include <c_base/system.h>

static Interface* C_DArray_interfaces[3];
static IFormattable C_DArray_i_formattable = {0};
static IHashable C_DArray_i_hashable = {0};

struct C_DArray {
  ClassObject base;

  u32 cap;
  u32 len;
  void** data;
};

C_DArray* C_DArray_new(void) { return C_DArray_new_cap(1); }

C_DArray* C_DArray_new_cap(u32 cap) {
  if (!Interface_initialized((Interface*)&C_DArray_i_formattable)) {
    C_DArray_i_formattable = IFormattable_construct_format(
      C_DArray_to_str_R, C_DArray_to_str_format_R);
    C_DArray_i_hashable = IHashable_construct(C_DArray_equals, IHashable_hash);

    C_DArray_interfaces[0] = (Interface*)&C_DArray_i_formattable;
    C_DArray_interfaces[1] = (Interface*)&C_DArray_i_hashable;
    C_DArray_interfaces[2] = null;
  }

  C_DArray* self = allocate(sizeof(C_DArray));
  self->base = ClassObject_construct(C_DArray_destroy, C_DArray_interfaces);

  self->cap = cap;
  self->len = 0;
  self->data = allocate(self->cap * sizeof(void*));

  return self;
}

void C_DArray_destroy(void* self) {
  C_DArray* self_cast = self;
  C_DArrayForeach(self_cast, { Unref(value); });
  deallocate(self_cast->data);
}

C_Array* C_DArray_to_array_P(C_DArray* self) {
  Ref(self);
  C_Array* array = C_Array_new(self->len);
  C_DArrayForeach(self, { C_Array_put_P(array, iter, value); });
  Unref(self);
  return array;
}

void C_DArray_push_P(C_DArray* self, void* value) {
  Ref(self);

  if (self->len + 1 > self->cap) {
    C_DArray_resize(self, self->cap * 2);
  }

  self->data[self->len] = Ref(value);
  self->len++;

  Unref(self);
}

void C_DArray_push_front_P(C_DArray* self, void* value) {
  Ref(self);
  if (self->len + 1 > self->cap) {
    C_DArray_resize(self, self->cap * 2);
  }

  mem_copy(self->data + 1, self->data, self->len * sizeof(void*));
  self->data[0] = Ref(value);

  self->len++;
  Unref(self);
}

void* C_DArray_pop_R(C_DArray* self) {
  if (self->len == 0) {
    crash(E(EG_Datastructures, E_OutOfBounds,
      SV("C_DArray_pop_R -> darray is empty")));
  }

  void* value = self->data[self->len - 1];
  self->data[self->len - 1] = null;
  self->len--;
  return value;
}

void* C_DArray_pop_front_R(C_DArray* self) {
  if (self->len == 0) {
    crash(E(EG_Datastructures, E_OutOfBounds,
      SV("C_DArray_pop_front_R -> darray is empty")));
  }

  void* value = self->data[0];
  self->data[0] = null;

  mem_copy(self->data, self->data + 1, self->len * sizeof(void*));
  self->len--;
  return value;
}

static void* __C_DArray_peek(C_DArray* self) {
  if (self->len == 0) {
    crash(E(EG_Datastructures, E_OutOfBounds,
      SV("__C_DArray_peek -> darray is empty")));
  }

  return Ref(self->data[self->len - 1]);
}

static void* __C_DArray_peek_front(C_DArray* self) {
  if (self->len == 0) {
    crash(E(EG_Datastructures, E_OutOfBounds,
      SV("__C_DArray_peek_front -> darray is empty")));
  }

  return Ref(self->data[0]);
}

void C_DArray_add_P(C_DArray* self, u32 index, void* value) {
  if (index > self->len) {
    crash(E(EG_Datastructures, E_OutOfBounds,
      SV("C_DArray_add_P -> index is outside of the darray")));
  }
  if (index == self->len) {
    C_DArray_push_P(self, value);
    return;
  }

  Ref(self);

  if (self->len + 1 > self->cap) {
    C_DArray_resize(self, self->cap * 2);
  }

  mem_copy(self->data + index + 1, self->data + index,
    (self->len - index) * sizeof(void*));
  self->data[index] = Ref(value);

  self->len++;

  Unref(self);
}

void* __C_DArray_at(C_DArray* self, u32 index) {
  if (index >= self->len) {
    crash(E(EG_Datastructures, E_OutOfBounds,
      SV("__C_DArray_at -> index is outside of the darray")));
  }

  return Ref(self->data[index]);
}

void* C_DArray_remove_R(C_DArray* self, u32 index) {
  if (index >= self->len) {
    crash(E(EG_Datastructures, E_OutOfBounds,
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
    crash(E(EG_Datastructures, E_OutOfBounds,
      SV("C_DArray_resize -> darray length would be grater that its "
         "capacity")));
  }
  if (cap == 0) {
    crash(E(EG_Datastructures, E_OutOfBounds,
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

u32 C_DArray_hash(void* self) {
  u32 hash_code = 0;
  C_DArrayForeach(
    self, { hash_code = 31 * hash_code + IHashable_hash(value); });
  return hash_code;
}

bool C_DArray_equals(void* a, void* b) {
  C_DArray* a_cast = a;
  C_DArray* b_cast = b;

  if (a_cast->len != b_cast->len) {
    return false;
  }

  C_DArrayForeach(a_cast, {
    if (!IHashable_equals(C_DArray_at_B(b_cast, iter), value)) {
      return false;
    }
  });
  return true;
}

C_String* C_DArray_to_str_format_R(void* self, C_String* format) {
  C_String* start = format_get_value_PR(format, PS("start"));
  C_String* end = format_get_value_PR(format, PS("end"));
  C_String* sep = format_get_value_PR(format, PS("sep"));

  C_List* list = C_List_new();
  C_List_push_P(list, start);

  C_DArrayForeach(self, {
    C_List_push_P(list, Pass(IFormattable_to_str_PR(value)));
    C_List_push_P(list, sep);
  });

  if (C_DArray_get_len(self) != 0) {
    Unref(C_List_pop_R(list));
  }

  C_List_push_P(list, end);

  C_String* result = C_String_join_PR(Pass(C_List_to_array_PR(Pass(list))));

  Unref(start);
  Unref(end);
  Unref(sep);

  return result;
}

C_String* C_DArray_to_str_R(void* self) {
  C_String* format = S("start=[;end=];sep=, ");
  C_String* result = C_DArray_to_str_format_R(self, format);
  Unref(format);
  return result;
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

void* C_DArray_at_R(C_DArray* self, u32 index) {
  void* result = __C_DArray_at(self, index);
  return result;
}
void* C_DArray_at_B(C_DArray* self, u32 index) {
  void* result = __C_DArray_at(self, index);
  Unref(result);
  return result;
}

// }}}
