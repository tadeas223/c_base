#include "os/os_mem.h"

#include <stdatomic.h>
#include <sys/mman.h>

void*
os_memory_reserve(u64 size) {
    return mmap(nullptr, size, PROT_NONE, MAP_PRIVATE | MAP_ANON, -1, 0);
}

void
os_memory_commit(void* ptr, u64 size) {
    mprotect(ptr, size, PROT_READ | PROT_WRITE);
}

void
os_memory_decommit(void* ptr, u64 size) {
    mprotect(ptr, size, PROT_NONE);
}

void
os_memory_release(void* ptr, u64 size) {
    munmap(ptr, size);
}
