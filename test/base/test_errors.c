
#include "base/errors.h"
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

Define_Error(E_Test);
Define_ErrorGroup(EG_Test);

static void test_error(void **state) {
    Error e = error(EG_Unspecified, E_Unspecified, S("error message"));
    
    assert_int_equal(EG_Unspecified, e.grp);
    assert_int_equal(E_Unspecified, e.code);
    assert_true(str_equals(S("error message"), e.msg));
}

static void test_E(void **state) {
    Error e = E(EG_Unspecified, E_Unspecified, S("error message"));
    
    assert_int_equal(EG_Unspecified, e.grp);
    assert_int_equal(E_Unspecified, e.code);
    assert_true(str_equals(S("error message"), e.msg));
}

static void test_ErrorName(void **state) {
    Error e = E(EG_Test, E_Test, S("error message"));
    
    assert_true(str_equals(S("E_Test"), ErrorName(e)));
}

static void test_ErrorGroupName(void **state) {
    Error e = E(EG_Test, E_Test, S("error message"));
    
    assert_true(str_equals(S("EG_Test"), ErrorGroupName(e)));
}

static void test_ResultOk(void **state) {
    Result(u8) result = ResultOk(u8, 5);
    assert_true(result.ok);
    assert_int_equal(5, result.value);
}

static void test_ResultErr(void **state) {
    Result(u8) result = ResultErr(u8, E(EG_Test, E_Test, S("error message")));
    assert_false(result.ok);
    assert_int_equal(EG_Test, result.err.grp);
    assert_int_equal(E_Test, result.err.code);
    assert_true(str_equals(S("error message"), result.err.msg));
}

static Result(u8) test_func_err() {
    return ResultErr(u8, E(EG_Test, E_Test, S("error message")));
}

static Result(u8) test_func_ok() {
    return ResultOk(u8, 1);
}

static Result(none) test_func() {
    u8 value = TryReturn(none, u8, test_func_ok());
    assert_int_equal(value, 5);

    u8 value2 = TryReturn(none, u8, test_func_err());
    // function will return now
    (void)value2;
    
    return ResultErr(none, E(EG_Test, E_Test, S("idk :(")));
}

static void test_TryReturn(void **state) {
    Result(none) result = test_func();
    
    assert_true(result.ok);
    assert_int_equal(EG_Test, result.err.grp);
    assert_int_equal(E_Test, result.err.code);
}


int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_error),
        cmocka_unit_test(test_E),
        cmocka_unit_test(test_ErrorName),
        cmocka_unit_test(test_ErrorGroupName),
        cmocka_unit_test(test_ResultOk),
        cmocka_unit_test(test_ResultErr),
        cmocka_unit_test(test_TryReturn),
    };

    return cmocka_run_group_tests(tests, null, null);
}
