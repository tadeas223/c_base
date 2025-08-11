#include <c_base/base/errors/errors.h>
#include <c_base/base/memory/allocator.h>
#include <c_base/base/memory/objects.h>
#include <c_base/base/strings/format.h>
#include <c_base/base/strings/strings.h>
#include <c_base/ds/C_Array.h>
#include <c_base/ds/C_List.h>
#include <c_base/system.h>

static Interface* C_List_interfaces[3];
static IHashable C_List_i_hashable = {0};
static IFormattable C_List_i_formattable = {0};
struct C_List {
  ClassObject base;
  Node* head;
  Node* tail;
  u32 len;
};

/******************************
 * new/dest
 ******************************/
C_List* C_List_new(void) {
  if (!Interface_initialized((Interface*)&C_List_i_hashable)) {
    C_List_i_hashable = IHashable_construct(C_List_equals, C_List_hash);
    C_List_i_formattable =
        IFormattable_construct_format(C_List_to_str_R, C_List_to_str_format_R);

    C_List_interfaces[0] = (Interface*)&C_List_i_hashable;
    C_List_interfaces[1] = (Interface*)&C_List_i_formattable;
    C_List_interfaces[2] = null;
  }

  C_List* self = allocate(sizeof(C_List));
  self->base = ClassObject_construct(C_List_destroy, C_List_interfaces);

  self->len = 0;
  self->head = null;
  self->tail = null;

  return self;
}

void C_List_destroy(void* self) {
  C_List* self_cast = self;
  Node* node = self_cast->head;

  while (node) {
    Node* remove_node = node;

    node = node->next;

    Unref(remove_node->value);
    deallocate(remove_node);
  }
}

/******************************
 * logic
 ******************************/
C_Array* C_List_to_array_PR(C_List* self) {
  Ref(self);
  C_Array* array = C_Array_new(self->len);
  C_ArrayForeach(array,
                 { C_Array_put_P(array, iter, C_List_at_B(self, iter)); });
  Unref(self);
  return array;
}

void C_List_push_P(C_List* self, void* value) {
  Ref(value);
  Node* new_node = allocate(sizeof(Node));
  new_node->value = value;
  new_node->next = null;

  switch (self->len) {
  case 0:
    self->head = new_node;
    break;
  case 1:
    self->tail = new_node;
    self->head->next = self->tail;
    break;
  default:
    self->tail->next = new_node;
    self->tail = new_node;
    break;
  }

  self->len++;
}

void C_List_push_front_P(C_List* self, void* value) {
  Ref(value);
  Node* new_node = allocate(sizeof(Node));
  new_node->value = value;
  new_node->next = self->head;

  self->head = new_node;

  self->len++;
}

void* C_List_pop_R(C_List* self) {
  if (self->len == 0) {
    crash(E(EG_Datastructures, E_OutOfBounds,
            SV("C_List_pop_R -> list is empty")));
  }

  Node* pop_node;

  switch (self->len) {
  case 1:
    pop_node = self->head;
    self->head = null;
    break;
  default: {
    pop_node = self->tail;
    Node* loop_node = self->head;

    while (loop_node->next != self->tail) {
      loop_node = loop_node->next;
    }

    loop_node->next = null;
    self->tail = loop_node;
  }
  }

  self->len--;

  void* value = pop_node->value;
  deallocate(pop_node);

  return value;
}

void* C_List_pop_front_R(C_List* self) {
  if (self->len == 0) {
    crash(E(EG_Datastructures, E_OutOfBounds,
            SV("C_List_pop_front_R -> list is empty")));
  }

  Node* pop_node = self->head;
  self->head = self->head->next;

  void* value = pop_node->value;

  self->len--;
  deallocate(pop_node);
  return value;
}

static void* __C_List_peek(C_List* self) {
  if (self->len == 0) {
    crash(E(EG_Datastructures, E_OutOfBounds,
            SV("__C_List_peek -> list is empty")));
  }

  return Ref(self->tail->value);
}

static void* __C_List_peek_front(C_List* self) {
  if (self->len == 0) {
    crash(E(EG_Datastructures, E_OutOfBounds,
            SV("__C_List_peek_front -> list is empty")));
  }

  return Ref(self->head->value);
}

void C_List_add_P(C_List* self, u32 index, void* value) {
  if (self->len == 0) {
    crash(
        E(EG_Datastructures, E_OutOfBounds, SV("C_List_add -> list is empty")));
  } else if (index > self->len) {
    crash(E(EG_Datastructures, E_OutOfBounds,
            SV("C_List_add -> index is outside of the list")));
  }

  Ref(value);

  if (index == self->len) {
    C_List_push_P(self, value);
    goto ret;
  }

  Node* new_node = allocate(sizeof(Node));
  new_node->value = value;

  Node* loop_node = self->head;
  for (u32 i = 0; i < index - 1; i++) {
    loop_node = loop_node->next;
  }

  new_node = loop_node->next;
  loop_node->next = new_node;

  self->len++;

ret:
  return;
}

static void* __C_List_at(C_List* self, u32 index) {
  if (self->len == 0) {
    crash(E(EG_Datastructures, E_OutOfBounds,
            SV("__C_List_at -> list is empty")));
  } else if (index >= self->len) {
    crash(E(EG_Datastructures, E_OutOfBounds,
            SV("__C_List_at -> index is outside of the list")));
  }

  void* result;

  if (index == 0) {
    result = self->head->value;
    goto ret;
  } else if (index == self->len - 1) {
    result = self->tail->value;
    goto ret;
  }

  Node* node = self->head;
  for (u32 i = 0; i < index; i++) {
    node = node->next;
  }
  result = node->value;

ret:
  return Ref(result);
}

void* C_List_remove_R(C_List* self, u32 index) {
  if (self->len == 0) {
    crash(E(EG_Datastructures, E_OutOfBounds,
            SV("__C_List_remove -> list is empty")));
  } else if (index >= self->len) {
    crash(E(EG_Datastructures, E_OutOfBounds,
            SV("__C_List_remove -> index is outside of the list")));
  }

  void* result;

  if (index == self->len - 1) {
    result = C_List_pop_R(self);
    goto ret;
  } else if (index == 0) {
    result = C_List_pop_front_R(self);
    goto ret;
  }

  Node* node = self->head;
  for (u32 i = 0; i < index - 1; i++) {
    node = node->next;
  }
  result = node->next->value;

  node->next = node->next->next;

  self->len--;
ret:
  return result;
}

void C_List_clear(C_List* self) {
  Node* node = self->head;
  while (node) {
    Node* remove_node = node;
    node = node->next;
    Unref(remove_node->value);
    deallocate(remove_node);
  }

  self->len = 0;
  self->head = null;
  self->tail = null;
}

u32 C_List_hash(void* self) {
  u32 hash_code = 0;
  C_List_Foreach(self, { hash_code = 31 * hash_code + IHashable_hash(value); });
  return hash_code;
}

bool C_List_equals(void* a, void* b) {
  C_List* a_cast = a;
  C_List* b_cast = b;

  if (a_cast->len != b_cast->len)
    return false;

  C_List_Foreach(a_cast, {
    if (IHashable_equals(C_List_at_B(b_cast, iter), value)) {
      return false;
    }
  });

  return true;
}

C_String* C_List_to_str_format_R(void* self, C_String* format) {
  C_String* start = format_get_value_PR(format, PS("start"));
  C_String* end = format_get_value_PR(format, PS("end"));
  C_String* sepparator = format_get_value_PR(format, PS("sep"));

  C_List* str_list = C_List_new();

  C_List_push_P(str_list, start);

  C_List_Foreach(self, {
    C_List_push_P(str_list, Pass(IFormattable_to_str_PR(value)));
    C_List_push_P(str_list, sepparator);
  });

  if (C_List_get_len(self) != 0) {
    Unref(C_List_pop_R(str_list));
  }
  C_List_push_P(str_list, end);

  C_String* result = C_String_join_PR(Pass(C_List_to_array_PR(str_list)));

  Unref(start);
  Unref(end);
  Unref(sepparator);
  Unref(str_list);

  return result;
}

C_String* C_List_to_str_R(void* self) {
  C_String* format = S("start=[;end=];sep=, ");
  C_String* result = C_List_to_str_format_R(self, format);
  Unref(format);
  return result;
}

// {{{ _R _B wrappers
void* C_List_peek_B(C_List* self) {
  void* result = __C_List_peek(self);
  Unref(result);
  return result;
}

void* C_List_peek_R(C_List* self) {
  void* result = __C_List_peek(self);
  return result;
}

void* C_List_peek_front_B(C_List* self) {
  void* result = __C_List_peek_front(self);
  Unref(result);
  return result;
}

void* C_List_peek_front_R(C_List* self) {
  void* result = __C_List_peek_front(self);
  return result;
}

void* C_List_at_B(C_List* self, u32 index) {
  void* result = __C_List_at(self, index);
  Unref(result);
  return result;
}

void* C_List_at_R(C_List* self, u32 index) {
  void* result = __C_List_at(self, index);
  return result;
}

// }}}

/******************************
 * get/set
 ******************************/
u32 C_List_get_len(C_List* self) { return self->len; }
