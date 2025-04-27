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

static void test_Kilobytes(void** state) {
    assert_int_equal(Kilobytes(1), 1024);
    assert_int_equal(Kilobytes(2), 2048);
    assert_int_equal(Kilobytes(5), 5120);
}

static void test_Megabytes(void** state) {
    assert_int_equal(Megabytes(1), 1048576);
    assert_int_equal(Megabytes(2), 2097152);
    assert_int_equal(Megabytes(5), 5242880);
}

static void test_Gigabytes(void** state) {
    assert_int_equal(Gigabytes(1), 1073741824);
    assert_int_equal(Gigabytes(2), 2147483648);
    assert_int_equal(Gigabytes(5), 5368709120);
}

int main(void) {
    Arena arena;
    arena_init_base(&arena, os_memory_base());

    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_Kilobytes), 
        cmocka_unit_test(test_Megabytes), 
        cmocka_unit_test(test_Gigabytes), 
    };

    return cmocka_run_group_tests(tests, null, null);
}
