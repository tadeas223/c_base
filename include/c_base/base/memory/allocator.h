#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <c_base/base/macros.h>
#include <c_base/base/types.h>

#define AllocatorCommitSize Kilobytes(4)
#define AllocatorReserveSize Gigabytes(4)

void* allocate(u64 size);
void deallocate(void* ptr);
void* reallocate(void* ptr, u64 size);

#endif
