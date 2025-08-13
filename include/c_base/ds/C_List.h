#ifndef LIST_H
#define LIST_H

#include <c_base/base/errors/errors.h>
#include <c_base/base/strings/strings.h>
#include <c_base/ds/C_Array.h>
#include <c_base/ds/ds.h>

#define C_ListForeach(list, code)                                              \
  do {                                                                         \
    for (u32 iter = 0; iter < C_List_get_len(list); iter++) {                  \
      void* value = C_List_at_B(list, iter);                                   \
      {                                                                        \
        code                                                                   \
      }                                                                        \
    }                                                                          \
  } while (0)

typedef struct Node {
  struct Node* next;
  void* value;
} Node;

typedef struct C_List C_List;

/******************************
 * new/dest
 ******************************/
C_List* C_List_new(void);
void C_List_destroy(void* self);

/******************************
 * logic
 ******************************/
C_Array* C_List_to_array_PR(C_List* self);

void C_List_push_P(C_List* self, void* value);
void C_List_push_front_P(C_List* self, void* value);

void* C_List_pop_R(C_List* self);
void* C_List_pop_front_R(C_List* self);

void* C_List_peek_B(C_List* self);
void* C_List_peek_R(C_List* self);

void* C_List_peek_front_B(C_List* self);
void* C_List_peek_front_R(C_List* self);

void C_List_add_P(C_List* self, u32 index, void* value);

void* C_List_at_B(C_List* self, u32 index);
void* C_List_at_R(C_List* self, u32 index);

void* C_List_remove_R(C_List* self, u32 index);

void C_List_clear(C_List* self);

u32 C_List_hash(void* self);
bool C_List_equals(void* a, void* b);

C_String* C_List_to_str_format_R(void* self, C_String* format);
C_String* C_List_to_str_R(void* self);

/******************************
 * get/set
 ******************************/
u32 C_List_get_len(C_List* self);

#endif
