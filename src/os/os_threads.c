#include <c_base/env.h>
#include <c_base/os/os_threads.h>

#if defined(PLATFORM_LINUX)
#include "impl/linux/linux_threads.c"
#else
#error "os_threads.c -> unsupported platform"
#endif
