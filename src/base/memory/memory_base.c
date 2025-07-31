#include "c_base/base/memory/memory.h"
#include <c_base/base/memory/memory_base.h>
#include <c_base/system.h>

static MemoryBase* global_base = null;

MemoryBase* MemoryBase_get(void) {
  if (global_base == null) {
    crash(E(EG_Memory, E_InvalidPointer,
            SV("MemoryBase_get -> global base was not set")));
  }

  return global_base;
}

void MemoryBase_set(MemoryBase* base) { global_base = base; }
