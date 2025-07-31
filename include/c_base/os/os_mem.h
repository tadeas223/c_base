#ifndef OS_MEM_H
#define OS_MEM_H

#include "c_base/base/memory/memory_base.h"

// if OPT_MEMORY_BASE is defined
// os_mem.c will not set global memory base

MemoryResult os_mem_reserve(MemoryBase* self, u64 size);
MemoryResult os_mem_commit(MemoryBase* self, void* ptr, u64 size);
MemoryResult os_mem_decommit(MemoryBase* self, void* ptr, u64 size);
MemoryResult os_mem_release(MemoryBase* self, void* ptr, u64 size);

#endif
