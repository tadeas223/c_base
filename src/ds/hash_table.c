#include <c_base/base/errors/errors.h>
#include <c_base/base/memory/allocator.h>
#include <c_base/base/memory/objects.h>
#include <c_base/ds/C_Array.h>
#include <c_base/ds/C_List.h>
#include <c_base/ds/hash_table.h>
#include <c_base/system.h>

#define HASH_DEFAULT_CAP 256

struct C_HashTable {
  ClassObject base;
  u32 cap;
  C_Array* /* C_List*<C_KeyValue*> */ data;
};

typedef struct {
  ClassObject base;
  void* key;
  void* value;
} C_KeyValue;

static void C_KeyValue_destroy(void* self) {
  C_KeyValue* self_cast = self;
  Unref(self_cast->key);
  Unref(self_cast->value);
}

static C_KeyValue* C_KeyValue_new_P(void* key, void* value) {
  C_KeyValue* self = allocate(sizeof(C_KeyValue));
  self->base = ClassObject_construct(C_KeyValue_destroy, null);
  Ref(key);
  Ref(value);
  self->key = key;
  self->value = value;

  return self;
}

C_HashTable* C_HashTable_new(void) {
  return C_HashTable_new_cap(HASH_DEFAULT_CAP);
}

C_HashTable* C_HashTable_new_cap(u32 cap) {
  C_HashTable* self = allocate(sizeof(C_HashTable));
  self->base = ClassObject_construct(C_HashTable_destroy, null);

  self->cap = cap;
  self->data = C_Array_new(self->cap);

  return self;
}

void C_HashTable_destroy(void* self) {
  C_HashTable* self_cast = self;
  C_HashTable_clear(self_cast);
}

void C_HashTable_put_P(C_HashTable* self, void* key, void* value) {
  u32 index = IHashable_hash(key) % self->cap;

  Ref(key);
  Ref(value);

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
}

static void* __C_HashTable_at_P(C_HashTable* self, void* key) {
  Ref(key);
  u32 index = IHashable_hash(key) % self->cap;

  C_List* list = C_Array_at_B(self->data, index);

  if (list == null) {
    crash(E(EG_Datastructures, E_InvalidPointer,
      SV("__C_HashTable_at -> key is not in the hash table")));
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

bool C_HashTable_contains(C_HashTable* self, void* key) {
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
  return result;
}

void* C_HashTable_remove_R(C_HashTable* self, void* key) {
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
  return result;
}

void C_HashTable_clear(C_HashTable* self) {
  Unref(self->data);
  self->cap = 0;
  self->data = null;
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
