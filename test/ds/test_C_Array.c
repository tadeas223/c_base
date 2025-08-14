// clang-format off
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>
// clang-format on

#include "../test_helpers.h"
#include "c_base/base/memory/allocator.h"
#include "c_base/base/strings/strings.h"
#include <c_base/base/memory/handles.h>
#include <c_base/base/memory/objects.h>
#include <c_base/base/varargs.h>
#include <c_base/ds/C_Array.h>
#include <c_base/os/os_io.h>

CreateTestHook(C_Array, C_Array_destroy)
CreateTestHook(C_Handle_u32, C_Handle_u32_destroy)

static void test_C_ArrayForeach(void** state) {
  (void)state;

  C_Array* array = C_Array_new(4);
  C_Array_put_P(array, 0, Pass(C_Handle_u32_new(0)));
  C_Array_put_P(array, 1, Pass(C_Handle_u32_new(1)));
  C_Array_put_P(array, 2, Pass(C_Handle_u32_new(2)));
  C_Array_put_P(array, 3, Pass(C_Handle_u32_new(3)));

  C_ArrayForeach(
    array, { assert_int_equal(iter, C_Handle_bool_get_value(value)); });

  Unref(array);
}

static void test_C_Array_new(void** state) {
  (void)state;

  C_Array* array = C_Array_new(10);

  AssertClassEqual(array, ClassObject_id);
  assert_int_equal(10, C_Array_get_len(array));

  C_ArrayForeach(array, { assert_ptr_equal(null, value); });

  Unref(array);
}

static void test_C_Array_destroy(void** state) {
  (void)state;

  C_Array* array = C_Array_new(1);
  C_Handle_u32* handle = C_Handle_u32_new(0);

  TestHook(C_Handle_u32, handle);

  C_Array_put_P(array, 0, Pass(handle));
  AssertHookDestroyed(1, { C_Array_destroy(array); });

  deallocate(array);
  refs--; // reset refs after manual deallocation
}

static void test_C_Array_put_P(void** state) {
  (void)state;

  /* test passing */ {
    C_Array* array = C_Array_new(1);
    C_Handle_u32* handle = C_Handle_u32_new(10);

    TestHook(C_Array, array);
    TestHook(C_Handle_u32, handle);

    AssertHookDestroyed(2, { C_Array_put_P(Pass(array), 0, Pass(handle)); });
  }

  /* test logic */ {
    C_Array* array = C_Array_new(3);
    C_ArrayForeach(
      array, { C_Array_put_P(array, iter, Pass(C_Handle_u32_new(iter))); });

    C_ArrayForeach(
      array, { assert_int_equal(iter, C_Handle_u32_get_value(value)); });

    Unref(array);
  }
}

static void test_C_Array_at_B(void** state) {
  (void)state;

  C_Array* array = C_Array_new(3);
  C_ArrayForeach(
    array, { C_Array_put_P(array, iter, Pass(C_Handle_u32_new(iter))); });

  assert_int_equal(0, C_Handle_u32_get_value(C_Array_at_B(array, 0)));
  assert_int_equal(1, C_Handle_u32_get_value(C_Array_at_B(array, 1)));
  assert_int_equal(2, C_Handle_u32_get_value(C_Array_at_B(array, 2)));

  Unref(array);
}

static void test_C_Array_at_R(void** state) {
  (void)state;

  C_Array* array = C_Array_new(3);
  C_ArrayForeach(
    array, { C_Array_put_P(array, iter, Pass(C_Handle_u32_new(iter))); });

  C_Handle_u32* val0 = C_Array_at_R(array, 0);
  C_Handle_u32* val1 = C_Array_at_R(array, 1);
  C_Handle_u32* val2 = C_Array_at_R(array, 2);

  assert_int_equal(0, C_Handle_u32_get_value(val0));
  assert_int_equal(1, C_Handle_u32_get_value(val1));
  assert_int_equal(2, C_Handle_u32_get_value(val2));

  Unref(val0);
  Unref(val1);
  Unref(val2);
  Unref(array);
}

static void test_C_Array_peek_B(void** state) {
  (void)state;

  C_Array* array = C_Array_new(3);
  C_ArrayForeach(
    array, { C_Array_put_P(array, iter, Pass(C_Handle_u32_new(iter))); });

  C_Handle_u32* peek = C_Array_peek_B(array);

  assert_int_equal(2, C_Handle_u32_get_value(peek));

  Unref(array);
}

static void test_C_Array_peek_R(void** state) {
  (void)state;

  C_Array* array = C_Array_new(3);
  C_ArrayForeach(
    array, { C_Array_put_P(array, iter, Pass(C_Handle_u32_new(iter))); });

  C_Handle_u32* peek = C_Array_peek_R(array);

  assert_int_equal(2, C_Handle_u32_get_value(peek));

  Unref(peek);
  Unref(array);
}

static void test_C_Array_peek_front_B(void** state) {
  (void)state;

  C_Array* array = C_Array_new(3);
  C_ArrayForeach(
    array, { C_Array_put_P(array, iter, Pass(C_Handle_u32_new(iter))); });

  C_Handle_u32* peek = C_Array_peek_front_B(array);

  assert_int_equal(0, C_Handle_u32_get_value(peek));

  Unref(array);
}

static void test_C_Array_peek_front_R(void** state) {
  (void)state;

  C_Array* array = C_Array_new(3);
  C_ArrayForeach(
    array, { C_Array_put_P(array, iter, Pass(C_Handle_u32_new(iter))); });

  C_Handle_u32* peek = C_Array_peek_front_R(array);

  assert_int_equal(0, C_Handle_u32_get_value(peek));

  Unref(peek);
  Unref(array);
}

static void test_C_Array_equals(void** state) {
  (void)state;

  C_Array* array = C_Array_new(4);
  C_ArrayForeach(
    array, { C_Array_put_P(array, iter, Pass(C_Handle_u32_new(iter))); });

  C_Array* array2 = C_Array_new(4);
  C_ArrayForeach(
    array, { C_Array_put_P(array2, iter, Pass(C_Handle_u32_new(iter))); });

  assert_true(C_Array_equals(array, array2));
  C_Array_put_P(array, 0, Pass(C_Handle_u32_new(20)));

  assert_false(C_Array_equals(array, array2));

  Unref(array);
  Unref(array2);
}

static void test_C_Array_to_str_format_R(void** state) {
  (void)state;

  C_Array* array = C_Array_new(3);
  C_Array_put_P(array, 0, Pass(C_Handle_u32_new(1)));
  C_Array_put_P(array, 1, Pass(C_Handle_u32_new(2)));
  C_Array_put_P(array, 2, Pass(C_Handle_u32_new(3)));

  C_String* correct_result = S("{1, 2, 3}");
  C_String* format = S("start={;end=};sep=, ");
  C_String* result = C_Array_to_str_format_R(array, format);

  assert_true(C_String_equals(correct_result, result));

  Unref(format);
  Unref(correct_result);
  Unref(result);
  Unref(array);
}

static void test_C_Array_clear(void** state) {
  (void)state;

  C_Array* array = C_Array_new(3);
  C_Handle_u32* val0 = C_Handle_u32_new(0);
  C_Handle_u32* val1 = C_Handle_u32_new(1);
  C_Handle_u32* val2 = C_Handle_u32_new(2);

  TestHook(C_Handle_u32, val0);
  TestHook(C_Handle_u32, val1);
  TestHook(C_Handle_u32, val2);

  C_Array_put_P(array, 0, Pass(val0));
  C_Array_put_P(array, 1, Pass(val1));
  C_Array_put_P(array, 2, Pass(val2));

  AssertHookDestroyed(3, { C_Array_clear(array); });

  C_ArrayForeach(array, { assert_true(value == null); });

  Unref(array);
}

int main(void) {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_C_ArrayForeach),
    cmocka_unit_test(test_C_Array_new),
    cmocka_unit_test(test_C_Array_destroy),
    cmocka_unit_test(test_C_Array_put_P),
    cmocka_unit_test(test_C_Array_at_B),
    cmocka_unit_test(test_C_Array_at_R),
    cmocka_unit_test(test_C_Array_peek_B),
    cmocka_unit_test(test_C_Array_peek_R),
    cmocka_unit_test(test_C_Array_peek_front_B),
    cmocka_unit_test(test_C_Array_peek_front_R),
    cmocka_unit_test(test_C_Array_equals),
    cmocka_unit_test(test_C_Array_to_str_format_R),
    cmocka_unit_test(test_C_Array_clear),
  };

  return cmocka_run_group_tests(tests, null, test_teardown);
}
