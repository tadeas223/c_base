#include "mem.h"
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>

void*
m_base_memory_malloc_reserve(void* ctx, u64 size) {
    printf("RESERVE: %llu\n", size);
    return malloc(size);
}

void
m_base_memory_release(void* ctx, void* ptr, u64 size) {
    printf("RELEASE: %p %llu\n", ptr, size);
    return free(ptr);
}

m_BaseMemory*
m_base_memory_malloc() {
    static m_BaseMemory memory = {};
    if(memory.reserve == null) {
        memory.reserve = m_base_memory_malloc_reserve;
        memory.commit = m_memory_change_noop;
        memory.decommit = m_memory_change_noop;
        memory.release = m_base_memory_release;
    }
    return &memory;
}

