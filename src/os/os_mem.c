#include "os/os_mem.h"
#include "base/defines.h"

# if defined(PLATFORM_LINUX)
#   include "impl/linux/linux_mem.c"
# endif
