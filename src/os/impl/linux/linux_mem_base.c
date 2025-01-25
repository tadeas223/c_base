#include "base/mem.h"
#include "os/os_mem.h"
#include <stdatomic.h>

void*
os_memory_base_reserve(void* ctx, u64 size) {
    return os_memory_reserve(size);
}

void
os_memory_base_commit(void* ctx, void* ptr, u64 size) {
    os_memory_commit(ptr, size);
}

void
os_memory_base_decommit(void* ctx, void* ptr, u64 size) {
    os_memory_decommit(ptr, size);
}

void
os_memory_base_release(void* ctx, void* ptr, u64 size) {
    os_memory_release(ptr, size);
}

m_MemoryBase*
os_memory_base() {
    static m_MemoryBase memory = {};
    if(memory.release == null) {
        memory.release = os_memory_base_release;
        memory.commit = os_memory_base_commit;
        memory.decommit = os_memory_base_decommit;
        memory.reserve = os_memory_base_reserve;
    }

    return &memory;
}
