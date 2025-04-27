#include "base/arena.h"
#include "os/os_mem.h"
#include "base/types.h"
#include "base/varargs.h"
#include "test_helpers.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

static void test_VarargsLen(void **state) {
    assert_int_equal(5, VarargsLen(1,2,3,4,5));
    assert_int_equal(1, VarargsLen(1));
    assert_int_equal(11, VarargsLen(1,2,3,4,5,6,7,8,9,10,11));
}

int main(void) {
    Arena arena;
    arena_init_base(&arena, os_memory_base());

    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_VarargsLen),
    };

    return cmocka_run_group_tests(tests, null, null);
}
