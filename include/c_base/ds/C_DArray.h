#ifndef DARRAY_H
#define DARRAY_H

#include <c_base/base/types.h>
#include <c_base/ds/ds.h>

#define C_DArrayForeach(darray, code)                                          \
  do {                                                                         \
    for (u32 iter = 0; iter < C_DArray_get_len(darray); iter++) {              \
      void* value = C_Darray_at_B(darray, iter);                               \
      {                                                                        \
        code                                                                   \
      }                                                                        \
    }                                                                          \
  } while (0);

typedef struct C_DArray C_DArray;

C_DArray* C_DArray_new(void);
C_DArray* C_DArray_new_cap(u32 cap);

void C_Darray_destroy(void* self);

void C_DArray_push_P(C_DArray* self, void* value);
void C_DArray_push_front_P(C_DArray* self, void* value);

void* C_DArray_pop_R(C_DArray* self);
void* C_DArray_pop_front_R(C_DArray* self);

void* C_DArray_peek_R(C_DArray* self);
void* C_DArray_peek_front_R(C_DArray* self);

void* C_DArray_peek_B(C_DArray* self);
void* C_DArray_peek_front_B(C_DArray* self);

void C_Darray_add_P(C_DArray* self, u32 index, void* value);

void* C_Darray_at_R(C_DArray* self, u32 index);
void* C_Darray_at_B(C_DArray* self, u32 index);
void* C_Darray_remove_R(C_DArray* self, u32 index);

void C_DArray_resize(C_DArray* self, u32 cap);
void C_DArray_compress(C_DArray* self);
void C_DArray_clear(C_DArray* self);

u32 C_DArray_get_cap(C_DArray* self);
u32 C_DArray_get_len(C_DArray* self);

#endif
