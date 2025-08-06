#include <c_base/env.h>
#include <c_base/os/os_mem.h>

#ifndef OPT_MEMORY_BASE

static MemoryBase os_base = {.reserve = os_mem_reserve,
                             .commit = os_mem_commit,
                             .decommit = os_mem_decommit,
                             .release = os_mem_release,
                             .ctx = null};

MemoryBase* global_memory_base = &os_base;
#endif

#if defined(PLATFORM_LINUX)
#include "impl/linux/linux_mem.c"
#else
#error "os_mem.c -> unsupported platform"
#endif
