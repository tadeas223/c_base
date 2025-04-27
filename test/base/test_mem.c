#include "base/mem.h"
#include "base/types.h"
#include "base/strings.h"
#include "base/strdef.h"
#include "test_helpers.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

static void test_mem_align_forward(void **state) {
    assert_int_equal(0,  mem_align_forward(0,  8));
    assert_int_equal(8,  mem_align_forward(1,  8));
    assert_int_equal(8,  mem_align_forward(8,  8));
    assert_int_equal(16, mem_align_forward(9,  8));
    assert_int_equal(32, mem_align_forward(31, 8));
}

static void test_mem_align_backward(void **state) {
    assert_int_equal(0,  mem_align_backward(0,  8));
    assert_int_equal(0,  mem_align_backward(1,  8));
    assert_int_equal(8,  mem_align_backward(8,  8));
    assert_int_equal(8,  mem_align_backward(9,  8));
    assert_int_equal(24, mem_align_backward(31, 8));
}

static void test_mem_copy(void **state) {
    String string1 = S("hello");
    Stack_String(string2, 5);

    mem_copy(string2.str.chars, string1.chars, 5);

    assert_true(str_equals(string1, string2.str));
}

static void test_func_global_memory_bae_get() {
    global_memory_base_get();
}

static void test_global_memory_base_get_set(void **state) {
    int status = test_in_new_process(test_func_global_memory_bae_get);     
    assert_int_not_equal(0, status);

    MemoryBase base;
    base.reserve  = (void*) 1;
    base.commit   = (void*) 2;
    base.decommit = (void*) 3;
    base.release  = (void*) 4;

    global_memory_base_set(&base);

    MemoryBase* ret_base = global_memory_base_get();

    assert_true(base.reserve  == ret_base->reserve);
    assert_true(base.commit   == ret_base->commit);
    assert_true(base.decommit == ret_base->decommit);
    assert_true(base.release  == ret_base->release);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_mem_align_forward),
        cmocka_unit_test(test_mem_align_backward),
        cmocka_unit_test(test_mem_copy),
        cmocka_unit_test(test_global_memory_base_get_set),
    };

    return cmocka_run_group_tests(tests, null, null);
}
