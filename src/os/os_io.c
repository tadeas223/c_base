#include "os/os_io.h"

#include "base/defines.h"

DefineError(FileNotFoundError);
DefineError(OutOfDiskSpaceError);
DefineError(PermissionError);
DefineError(InvalidOperationError);

/* include OS dependent stuff */
# if defined(PLATFORM_LINUX)
#   include "os/impl/linux/linux_io.c"
# endif
