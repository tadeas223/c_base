#include <c_base/ds/list.h>
#include <c_base/base/errors/errors.h>
#include <c_base/base/memory/allocator.h>
#include <c_base/base/memory/objects.h>
#include <c_base/system.h>

GenericValImpl_ErrorCode(EG_List)

struct C_List {
  ComplexBase base;
  Node* head;
  Node* tail;
  u32 len;
};

/******************************
 * new/dest
 ******************************/
C_List* C_List_new(void) {
  C_List* self = allocate(sizeof(C_List));
  new(self, C_List_destroy);

  self->len = 0;
  self->head = null;
  self->tail = null;

  return self;
}

void C_List_destroy(void* self) {
  C_List* self_cast = self;
  Node* node = self_cast->head;

  while(node) {
    Node* remove_node = node;

    node = node->next;

    unref(remove_node->value);
    deallocate(remove_node);
  }
}

/******************************
 * logic
 ******************************/
void C_List_push(C_List* self, void* value) {
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

void C_List_push_front(C_List* self, void* value) {
  Node* new_node = allocate(sizeof(Node));
  new_node->value = value;
  new_node->next = self->head;

  self->head = new_node;

  self->len++;
}

void* C_List_pop_R(C_List* self) {
  if(self->len == 0) {
    crash(E(EG_List, E_OutOfBounds, SV("C_List_pop_R -> list is empty")));
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
      
      while(loop_node->next != self->tail) {
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
  if(self->len == 0) {
    crash(E(EG_List, E_OutOfBounds, SV("C_List_pop_front_R -> list is empty")));
  }

  Node* pop_node = self->head;
  self->head = self->head->next;
  
  void* value = pop_node->value;
  
  self->len--;
  deallocate(pop_node);
  return value;
}

static void* __C_List_peek(C_List* self) {
  if(self->len == 0) {
    crash(E(EG_List, E_OutOfBounds, SV("__C_List_peek -> list is empty")));
  }

  return ref(self->tail->value);
}

static void* __C_List_peek_front(C_List* self) {
  if(self->len == 0) {
    crash(E(EG_List, E_OutOfBounds, SV("__C_List_peek_front -> list is empty")));
  }

  return ref(self->head->value);
}

void C_List_add(C_List* self, u32 index, void* value) {
  if(self->len == 0) {
    crash(E(EG_List, E_OutOfBounds, SV("C_List_add -> list is empty")));
  } else if(index > self->len) {
    crash(E(EG_List, E_OutOfBounds, SV("C_List_add -> index is outside of the list")));
  }

  if(index == self->len) {
    C_List_push(self, value);
    goto ret;
  }

  Node* new_node = allocate(sizeof(Node));
  new_node->value = value;

  Node* loop_node = self->head;
  for(u32 i = 0; i < index-1; i++) {
    loop_node = loop_node->next; 
  }

  new_node = loop_node->next;
  loop_node->next = new_node;
  
  self->len++;

ret:
  return;
}

static void* __C_List_at(C_List* self, u32 index) {
  if(self->len == 0) {
    crash(E(EG_List, E_OutOfBounds, SV("__C_List_at -> list is empty")));
  } else if(index >= self->len) {
    crash(E(EG_List, E_OutOfBounds, SV("__C_List_at -> index is outside of the list")));
  }
  
  void* result;

  if(index == 0) {
    result = self->head->value;
    goto ret;
  } else if(index == self->len-1) {
    result = self->tail->value;
    goto ret;
  }

  Node* node = self->head;
  for(u32 i = 0; i < index; i++) {
    node = node->next; 
  }
  result = node->value;

ret:
  return ref(result);
}

void* C_List_remove_R(C_List* self, u32 index) {
  if(self->len == 0) {
    crash(E(EG_List, E_OutOfBounds, SV("__C_List_remove -> list is empty")));
  } else if(index >= self->len) {
    crash(E(EG_List, E_OutOfBounds, SV("__C_List_remove -> index is outside of the list")));
  }
  
  void* result;

  if(index == self->len-1) {
    result = C_List_pop_R(self);
    goto ret;
  } else if(index == 0) {
    result = C_List_pop_front_R(self);
    goto ret;
  }
  
  Node* node = self->head;
  for(u32 i = 0; i < index-1; i++) {
    node = node->next;
  }
  result = node->next->value;

  node->next = node->next->next;
  
  self->len--;
ret:
  return result;
}

void C_List_clear(C_List *self) {
  Node* node = self->head;
  while(node) {
    Node* remove_node = node;
    node = node->next;
    unref(remove_node->value); 
    deallocate(remove_node); 
  }

  self->len = 0;
  self->head = null;
  self->tail = null;
}

// {{{ _R _B wrappers
void* C_List_peek_B(C_List* self) {
  void* result = __C_List_peek(self);
  unref(result);
  return result;
}

void* C_List_peek_R(C_List* self) {
  void* result = __C_List_peek(self);
  return result;
}

void* C_List_peek_front_B(C_List* self) {
  void* result = __C_List_peek_front(self);
  unref(result);
  return result;
}

void* C_List_peek_front_R(C_List* self) {
  void* result = __C_List_peek_front(self);
  return result;
}

void* C_List_at_B(C_List* self, u32 index) {
  void* result = __C_List_at(self, index);
  unref(result);
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
u32 C_List_get_len(C_List* self) {
  return self->len;
}
