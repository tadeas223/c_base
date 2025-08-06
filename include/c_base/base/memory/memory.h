#ifndef MEMORY_H
#define MEMORY_H

#include <c_base/base/errors/errors.h>
#include <c_base/base/types.h>

GenericVal_ErrorCode(EG_Memory)

#define SetZero(val) mem_set((void*)(val), 0, sizeof(*(val)) - 1)
#define MemAlign sizeof(void*)

void mem_copy(void* dest, void* src, u64 size);
void mem_set(void* dest, u8 byte, u64 size);
bool mem_equals(void* a, void* b, u64 size);

u64 mem_align_forward(u64 value, u64 align);

#endif
