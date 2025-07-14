#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <c_base/base/types.h>

void* allocate(u64 size);
void deallocate(void* ptr);
void* reallocate(void* ptr, u64 size);

#endif
