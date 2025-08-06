#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <c_base/base/types.h>
#include <c_base/ds/ds.h>

typedef struct C_HashTable C_HashTable;

C_HashTable* C_HashTable_new(void);
C_HashTable* C_HashTable_new_cap(u32 cap);
void C_HashTable_destroy(void* self);

void C_HashTable_put_P(C_HashTable* self, void* key, void* value);

void* C_HashTable_at_PB(C_HashTable* self, void* key);
void* C_HashTable_at_PR(C_HashTable* self, void* key);

bool C_HashTable_contains(C_HashTable* self, void* key);

void* C_HashTable_remove(C_HashTable* self, void* key);

void C_HashTable_clear(C_HashTable* self);

#endif
