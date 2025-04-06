#ifndef MEM_H
#define MEM_H

#include "base/errors.h"
#include "base/types.h"

Extern_ErrorGroup(EG_Memory);

#define MEM_ALIGN sizeof(void*)

TypeStructNoDecl(MemoryBase, struct MemoryBase);

typedef Result(void_p) MemoryBaseReserveFunc(MemoryBase *self, u64 size);
typedef Result(none) MemoryBaseReleaseFunc(MemoryBase *self, void *ptr, u64 size);
typedef Result(none) MemoryBaseCommitFunc(MemoryBase *self, void *ptr, u64 size);
typedef Result(none) MemoryBaseDecommitFunc(MemoryBase *self, void *ptr, u64 size);

struct MemoryBase {
    MemoryBaseReserveFunc *reserve;
    MemoryBaseReleaseFunc *release;
    MemoryBaseCommitFunc *commit;
    MemoryBaseDecommitFunc *decommit;
};

void global_memory_base_set(MemoryBase *base);
MemoryBase* global_memory_base_get();

u64 mem_align_forward(u64 val, u64 align);
u64 mem_align_backward(u64 val, u64 align);
void mem_copy(void* dest, void* src, u64 size);

#endif
