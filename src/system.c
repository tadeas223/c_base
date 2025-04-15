#include "system.h"
#include "base/errors.h"
#include "os/os_io.h"

#include <stdlib.h>

void
crash(Error err) {
    ConsoleWriteLn(S("UNEXPECTED ERROR"));
    ConsoleWriteLn(S("Error Group: "), ErrorGroupName(err));
    ConsoleWriteLn(S("Error: "), ErrorName(err));
    ConsoleWriteLn(S("Message: "), err.msg);
    exit(1);
}
