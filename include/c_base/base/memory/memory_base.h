#ifndef MEMORY_BASE_H
#define MEMORY_BASE_H

#include "c_base/base/errors/errors.h"

typedef struct {
  bool ok;
  Error error;
  void* ptr;
  u64 size;
} MemoryResult;

typedef struct MemoryBase MemoryBase;

typedef MemoryResult (*MemoryBaseReserveFunc)(MemoryBase* self, u64 size);
typedef MemoryResult (*MemoryBaseCommitFunc)(MemoryBase* self, void* ptr,
                                             u64 size);
typedef MemoryResult (*MemoryBaseDecommitFunc)(MemoryBase* self, void* ptr,
                                               u64 size);
typedef MemoryResult (*MemoryBaseReleaseFunc)(MemoryBase* self, void* ptr,
                                              u64 size);

struct MemoryBase {
  MemoryBaseReserveFunc reserve;
  MemoryBaseCommitFunc commit;
  MemoryBaseDecommitFunc decommit;
  MemoryBaseReleaseFunc release;
  void* ctx;
};

extern MemoryBase* global_memory_base;
#endif
