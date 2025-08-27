#include "c_base/base/memory/handles.h"
#include "c_base/base/strings/format.h"
#include "c_base/base/strings/strings.h"
#include <c_base/base/errors/errors.h>
#include <c_base/base/memory/allocator.h>
#include <c_base/base/memory/objects.h>
#include <c_base/ds/C_Array.h>
#include <c_base/ds/C_HashTable.h>
#include <c_base/ds/C_List.h>
#include <c_base/system.h>

#define HASH_DEFAULT_CAP 256

static Interface* C_HashTable_interfaces[3];
static IFormattable C_HashTable_i_formattable = {0};
static IHashable C_HashTable_i_hashable = {0};
struct C_HashTable {
  ClassObject base;
  u32 cap;
  C_Array* /* C_List*<C_KeyValue*> */ data;
};

static Interface* C_KeyValue_interfaces[2];
static IHashable C_KeyValue_i_hashable = {0};
typedef struct {
  ClassObject base;
  void* key;
  void* value;
} C_KeyValue;

static u32 C_KeyValue_hash(void* self) {
  C_KeyValue* self_cast = self;
  u32 hash =
    31 * IHashable_hash(self_cast->key) + IHashable_hash(self_cast->value);
  return hash;
}

static bool C_keyValue_equals(void* a, void* b) {
  C_KeyValue* a_cast = a;
  C_KeyValue* b_cast = b;

  return IHashable_equals(a_cast->key, b_cast->key) &&
         IHashable_equals(a_cast->value, b_cast->value);
}

static void C_KeyValue_destroy(void* self) {
  C_KeyValue* self_cast = self;
  Unref(self_cast->key);
  Unref(self_cast->value);
}

static C_KeyValue* C_KeyValue_new_P(void* key, void* value) {
  if (!Interface_initialized((Interface*)&C_KeyValue_i_hashable)) {
    C_KeyValue_i_hashable =
      IHashable_construct(C_keyValue_equals, C_KeyValue_hash);

    C_KeyValue_interfaces[0] = (Interface*)&C_KeyValue_i_hashable;
    C_KeyValue_interfaces[1] = null;
  }

  C_KeyValue* self = allocate(sizeof(C_KeyValue));
  self->base = ClassObject_construct(C_KeyValue_destroy, C_KeyValue_interfaces);
  self->key = Ref(key);
  self->value = Ref(value);

  return self;
}

C_HashTable* C_HashTable_new(void) {
  return C_HashTable_new_cap(HASH_DEFAULT_CAP);
}

C_HashTable* C_HashTable_new_cap(u32 cap) {
  if (!Interface_initialized((Interface*)&C_HashTable_i_formattable)) {
    C_HashTable_i_formattable = IFormattable_construct_format(
      C_HashTable_to_str_R, C_HashTable_to_str_format_R);
    C_HashTable_i_hashable =
      IHashable_construct(C_HashTable_equals, C_HashTable_hash);

    C_HashTable_interfaces[0] = (Interface*)&C_HashTable_i_formattable;
    C_HashTable_interfaces[1] = (Interface*)&C_HashTable_i_hashable;
    C_HashTable_interfaces[2] = null;
  }

  C_HashTable* self = allocate(sizeof(C_HashTable));
  self->base =
    ClassObject_construct(C_HashTable_destroy, C_HashTable_interfaces);

  self->cap = cap;
  self->data = C_Array_new(self->cap);

  return self;
}

void C_HashTable_destroy(void* self) {
  C_HashTable* self_cast = self;
  Unref(self_cast->data);
}

void C_HashTable_put_P(C_HashTable* self, void* key, void* value) {
  Ref(self);
  Ref(key);
  Ref(value);

  u32 index = IHashable_hash(key) % self->cap;

  C_List* list = C_Array_at_B(self->data, index);
  if (list == null) {
    list = C_List_new();
    C_Array_put_P(self->data, index, Pass(list));
  }

  C_ListForeach(list, {
    C_KeyValue* key_value = value;
    if (IHashable_equals(key_value->key, key)) {
      crash(E(EG_Datastructures, E_InvalidPointer,
        SV("C_HashTable_put_P -> key is already in the hash table")));
    }
  });

  C_KeyValue* key_value = C_KeyValue_new_P(Pass(key), Pass(value));

  C_List_push_P(list, Pass(key_value));

  Unref(self);
}

static void* __C_HashTable_at_P(C_HashTable* self, void* key) {
  Ref(key);
  u32 index = IHashable_hash(key) % self->cap;

  C_List* list = C_Array_at_B(self->data, index);

  if (list == null) {
    crash(E(EG_Datastructures, E_InvalidPointer,
      SV("__C_HashTable_at_P -> key is not in the hash table")));
  }

  void* val = 0;
  C_ListForeach(list, {
    C_KeyValue* key_value = value;
    if (IHashable_equals(key_value->key, key)) {
      val = key_value->value;
      goto ret;
    }
  });

ret:
  Unref(key);
  return Ref(val);
}

bool C_HashTable_contains_P(C_HashTable* self, void* key) {
  Ref(self);
  Ref(key);

  u32 index = IHashable_hash(key) % self->cap;

  C_List* list = C_Array_at_B(self->data, index);

  bool result = false;
  if (list == null) {
    goto ret;
  }

  C_ListForeach(list, {
    C_KeyValue* key_value = value;
    if (IHashable_equals(key_value->key, key)) {
      result = true;
      goto ret;
    }
  });

ret:
  Unref(self);
  Unref(key);
  return result;
}

void* C_HashTable_remove_PR(C_HashTable* self, void* key) {
  Ref(self);
  Ref(key);

  u32 index = IHashable_hash(key) % self->cap;

  C_List* list = C_Array_at_B(self->data, index);

  void* result = null;
  if (list == null) {
    crash(E(EG_Datastructures, E_InvalidPointer,
      SV("C_HashTable_remove_R -> key is not in the hash table")));
  }

  C_ListForeach(list, {
    C_KeyValue* key_value = value;
    if (IHashable_equals(key_value->key, key)) {
      result = Ref(key_value->value);
      Unref(C_List_remove_R(list, iter));
      goto ret;
    }
  });

  if (result == null) {
    crash(E(EG_Datastructures, E_InvalidPointer,
      SV("C_HashTable_remove_R -> key is not in the hash table")));
  }

ret:
  Unref(self);
  Unref(key);
  return result;
}

void C_HashTable_clear(C_HashTable* self) { C_Array_clear(self->data); }

u32 C_HashTable_get_cap(C_HashTable* self) { return self->cap; }

u32 C_HashTable_hash(void* self) {
  C_HashTable* self_cast = self;

  u32 hash_code = 0;
  C_ArrayForeach(
    self_cast->data, { hash_code = 31 * hash_code + IHashable_hash(value); });
  return hash_code;
}

bool C_HashTable_equals(void* a, void* b) {
  C_HashTable* a_cast = a;
  C_HashTable* b_cast = b;

  C_ArrayForeach(a_cast->data, {
    if (!IHashable_equals(C_Array_at_B(b_cast->data, iter), value)) {
      return false;
    }
  });

  return true;
}

C_String* C_HashTable_to_str_format_R(void* self, C_String* format) {
  C_HashTable* self_cast = self;

  C_String* start = format_get_value_PR(format, PS("start"));
  C_String* sep = format_get_value_PR(format, PS("sep"));
  C_String* el_sep = format_get_value_PR(format, PS("el_sep"));
  C_String* end = format_get_value_PR(format, PS("end"));

  C_List* list = C_List_new();
  C_List_push_P(list, start);

  C_ArrayForeach(self_cast->data, {
    C_List* l = value;

    if (l == null) {
      continue;
    }

    C_ListForeach(l, {
      C_KeyValue* kvp = value;

      C_List_push_P(list, Pass(IFormattable_to_str_PR(kvp->key)));
      C_List_push_P(list, el_sep);
      C_List_push_P(list, Pass(IFormattable_to_str_PR(kvp->value)));
      C_List_push_P(list, sep);
    });
  });

  if (C_List_get_len(list) != 1) {
    Unref(C_List_pop_R(list));
  }

  C_List_push_P(list, end);

  C_String* result = C_String_join_PR(Pass(C_List_to_array_PR(Pass(list))));
  Unref(start);
  Unref(sep);
  Unref(el_sep);
  Unref(end);

  return result;
}

C_String* C_HashTable_to_str_R(void* self) {
  C_String* format = S("start=[;end=];sep=, ;el_sep= : ");
  C_String* result = C_HashTable_to_str_format_R(self, format);
  Unref(format);
  return result;
}

// {{{ _R _B wrappers

void* C_HashTable_at_PB(C_HashTable* self, void* key) {
  void* result = __C_HashTable_at_P(self, key);
  Unref(result);
  return result;
}

void* C_HashTable_at_PR(C_HashTable* self, void* key) {
  void* result = __C_HashTable_at_P(self, key);
  return result;
}

// }}}
