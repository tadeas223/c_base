#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "c_base/base/strings/strings.h"
#include <c_base/base/types.h>
#include <c_base/ds/ds_base.h>

typedef struct C_HashTable C_HashTable;

C_HashTable* C_HashTable_new(void);
C_HashTable* C_HashTable_new_cap(u32 cap);
void C_HashTable_destroy(void* self);

void C_HashTable_put_P(C_HashTable* self, void* key, void* value);

void* C_HashTable_at_PB(C_HashTable* self, void* key);
void* C_HashTable_at_PR(C_HashTable* self, void* key);

bool C_HashTable_contains_P(C_HashTable* self, void* key);

void* C_HashTable_remove_PR(C_HashTable* self, void* key);

void C_HashTable_clear(C_HashTable* self);

u32 C_HashTable_get_cap(C_HashTable* self);

u32 C_HashTable_hash(void* self);
bool C_HashTable_equals(void* a, void* b);

C_String* C_HashTable_to_str_format_R(void* self, C_String* format);
C_String* C_HashTable_to_str_R(void* self);

#endif
