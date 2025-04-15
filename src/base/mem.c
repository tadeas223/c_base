#include "base/mem.h"
#include "base/errors.h"
#include "base/types.h"
#include "system.h"

Define_ErrorGroup(EG_Memory);

/* if null global_memory_base_get() crashes */
static MemoryBase* global_base = null;

void global_memory_base_set(MemoryBase *base) {
    global_base = base;
}

MemoryBase* global_memory_base_get() {
    if(global_base == null) {
        crash(
            E(
                EG_Memory,
                E_Uninitialzied, 
                S("global memory was not initialized, try running global_memory_base_set()")
            )
        );
    }

    return global_base;
}

u64 mem_align_forward(u64 val, u64 align) {
    u64 modulo = val % align;

    if(modulo != 0) {
        val += align - modulo;
    }
    return val;
}

u64 mem_align_backward(u64 val, u64 align) {
    u64 modulo = val % align;

    if(modulo != 0) {
        val -= modulo; 
    }
    return val;

}

void mem_copy(void *dest, void *src, u64 size) {
    if(size == 0) return;

    u8* dest8 = (u8*) dest;
    u8* src8 = (u8*) src;

    if(dest8 < src8) {
        // copy from left to right
        for(u64 i = 0; i < size; i++) {
            dest8[i] = src8[i]; 
        }
    } else {
        // copy from right to left
        while(size != 0) {
            size--;
            dest8[size] = src8[size]; 
        }
    }
}
