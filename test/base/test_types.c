#include <stdio.h>
#include <stdlib.h>

/* make cmocka work for c89 */
#define inline

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "base/types.h"


/****************************************
 * Macro tests
****************************************/

static void
test_Kilobytes()
{
    assert_int_equal(1024, Kilobytes(1));
    assert_int_equal(2048, Kilobytes(2));
    assert_int_equal(5120, Kilobytes(5));
}

static void
test_Megabytes() {
    assert_int_equal(1048576, Megabytes(1));
    assert_int_equal(2097152, Megabytes(2));
    assert_int_equal(5242880, Megabytes(5));
}

static void
test_Gigabytes() {
    assert_true(1073741824 == Gigabytes(1));
    /* cant test more because of integer overflow */
}

static const struct CMUnitTest grp_macros[] = {
    cmocka_unit_test(test_Kilobytes),
    cmocka_unit_test(test_Megabytes),
    cmocka_unit_test(test_Gigabytes),
};

int
main(int argc, char **argv) {
    if(argc > 1) {
        switch (argv[1][0]) {
            case '0': 
                return cmocka_run_group_tests(grp_macros, NULL, NULL);
            default:
                printf("Missing group");
                exit(1);
        }
    } else {
        printf("Missing argument");
        exit(1);
    }
    return 0;
}
