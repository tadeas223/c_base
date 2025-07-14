#ifndef MEMORY_H
#define MEMORY_H

#include "base/types.h"

#define SetZero(val) mem_set((val), 0, sizeof(*(val)) - 1)

void mem_copy(void* dest, void* src, u64 size);
void mem_set(void* dest, u8 byte, u64 size);
bool mem_equals(void* a, void* b, u64 size);

#endif
