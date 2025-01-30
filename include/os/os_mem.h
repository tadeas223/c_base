#ifndef OS_MEM_H
#define OS_MEM_H

#include "base/mem.h"
#include "base/types.h"

void* os_memory_reserve(u64 size);
void os_memory_commit(void* ptr, u64 size);
void os_memory_decommit(void* ptr, u64 size);
void os_memory_release(void* ptr, u64 size);

m_MemoryBase* os_memory_base();
#endif
