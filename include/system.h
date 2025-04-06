#ifndef SYSTEM_H
#define SYSTEM_H

#include "base/errors.h"

#define TryCrash(result_type, expr) ({     \
        Highlight(result_type);            \
        Result(result_type) result = expr; \
        if(!result.ok) {                   \
            crash(result.err);             \
        }                                  \
        result.value;                      \
    })

void crash(Error err);

#endif
