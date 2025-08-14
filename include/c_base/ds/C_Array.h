#ifndef ARRAY_H
#define ARRAY_H

#include <c_base/base/errors/errors.h>
#include <c_base/base/types.h>
#include <c_base/ds/ds_base.h>

#define C_ArrayForeach(array, code)                                            \
  do {                                                                         \
    for (u32 iter = 0; iter < C_Array_get_len(array); iter++) {                \
      void* value = C_Array_at_B(array, iter);                                 \
      (void)value;                                                             \
      {                                                                        \
        code                                                                   \
      }                                                                        \
    }                                                                          \
  } while (0)

typedef struct C_Array C_Array;

/******************************
 * new/dest
 ******************************/
C_Array* C_Array_new(u32 len);

void C_Array_destroy(void* self);

/******************************
 * logic
 ******************************/
void C_Array_put_P(C_Array* self, u32 index, void* value);

void* C_Array_at_B(C_Array* self, u32 index);
void* C_Array_at_R(C_Array* self, u32 index);

void* C_Array_peek_B(C_Array* self);
void* C_Array_peek_R(C_Array* self);

void* C_Array_peek_front_B(C_Array* self);
void* C_Array_peek_front_R(C_Array* self);

void C_Array_clear(C_Array* self);

bool C_Array_equals(void* a, void* b);
u32 C_Array_hash(void* self);

/******************************
 * get/set
 ******************************/

u32 C_Array_get_len(C_Array* self);

#endif
