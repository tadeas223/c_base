#ifndef OS_MEM_H
#define OS_MEM_H

#include "base/errors.h"
#include "base/mem.h"

Extern_ErrorGroup(EG_OS_Memory);

MemoryBase* os_memory_base();

Result(ptr /* void* */) os_memory_reserve(MemoryBase* self, u64 size);
Result(none) os_memory_release(MemoryBase* self, void* ptr, u64 size);
Result(none) os_memory_commit(MemoryBase* self, void* ptr, u64 size);
Result(none) os_memory_decommit(MemoryBase* self, void* ptr, u64 size);

#endif
