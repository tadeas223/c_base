#include "base/errors.h"
#include "os/os_mem.h"

#include <errno.h>
#include <stdio.h>
#include <sys/mman.h>

Result(void_p)
os_memory_reserve(MemoryBase* self, u64 size) {
    void* ptr = mmap(null, size, PROT_NONE, MAP_PRIVATE|MAP_ANON, -1, 0);
    if(ptr == MAP_FAILED) {
        return ResultErr(void_p, E(EG_OS_Memory, E_Unspecified, S("mmap failed"))); 
    }

    return ResultOk(void_p, ptr);
}

Result(none)
os_memory_release(MemoryBase* self, void* ptr, u64 size) {
    if(size == 0) return ResultOk(none, null); 
    int val = munmap(ptr, size);
    if(val < 0) {
        return ResultErr(none, E(EG_OS_Memory, E_Unspecified, S("munmap failed"))); 
    }

    return ResultOk(none, null);
}

Result(none)
os_memory_commit(MemoryBase* self, void* ptr, u64 size) {
    if(size == 0) return ResultOk(none, null); 
    int val = mprotect(ptr, size, PROT_READ|PROT_WRITE);
    if(val < 0) {
        printf("ptr = %p, size = %llu\n", ptr, size); 
        return ResultErr(none, E(EG_OS_Memory, E_Unspecified, S("mprotect commit failed"))); 
    }
    
    return ResultOk(none, null);
}

Result(none)
os_memory_decommit(MemoryBase* self, void* ptr, u64 size) {
    if(size == 0) return ResultOk(none, null); 
    int val = mprotect(ptr, size, PROT_NONE);
    if(val < 0) {
        return ResultErr(none, E(EG_OS_Memory, E_Unspecified, S("mprotect decommit failed"))); 
    }

    return ResultOk(none, null);
}
