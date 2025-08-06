#include <c_base/env.h>
#include <c_base/os/os_dylib.h>

#if defined(PLATFORM_LINUX)
#include "impl/linux/linux_dynlib.c"
#else
#error "os_dynlib.c -> unsupported platform"
#endif
