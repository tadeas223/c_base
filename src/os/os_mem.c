#include "base/errors.h"
#include "base/enviroment.h"
#include "base/mem.h"
#include "os/os_mem.h"

Define_ErrorGroup(EG_OS_Memory);

#ifdef PLATFORM_LINUX
#include "impl/linux/linux_mem.c"
#else
#error Unknown platform - os_mem.c
#endif


MemoryBase*
os_memory_base() {
    static MemoryBase base = {0};
    if(base.reserve == null) {
        base.reserve = os_memory_reserve;
        base.commit = os_memory_commit;
        base.decommit = os_memory_decommit;
        base.release = os_memory_release;
    }

    return &base;
}
