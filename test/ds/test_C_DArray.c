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
#include <c_base/ds/C_DArray.h>
#include <c_base/os/os_io.h>

CreateTestHook(C_DArray, C_DArray_destroy)
CreateTestHook(C_Handle_u32, C_Handle_u32_destroy)

static void test_C_DArrayForeach(void** state) {
  (void)state;

  C_DArray* darray = C_DArray_new();
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(0)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(1)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(2)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(3)));

  C_DArrayForeach(
    darray, { assert_int_equal(iter, C_Handle_bool_get_value(value)); });

  Unref(darray);
}

static void test_C_DArray_new(void** state) {
  (void)state;

  C_DArray* darray = C_DArray_new();

  AssertClassEqual(darray, ClassObject_id);
  assert_int_equal(C_DArray_get_len(darray), 0);
  /* capacity could be changed later */

  Unref(darray);
}

static void test_C_DArray_new_cap(void** state) {
  (void)state;

  C_DArray* darray = C_DArray_new_cap(10);

  AssertClassEqual(darray, ClassObject_id);
  assert_int_equal(C_DArray_get_len(darray), 0);
  assert_int_equal(C_DArray_get_cap(darray), 10);

  Unref(darray);
}

static void test_C_DArray_destroy(void** state) {
  (void)state;

  C_DArray* darray = C_DArray_new();

  C_Handle_u32* handle = C_Handle_u32_new(10);
  TestHook(C_Handle_u32, handle);

  C_DArray_push_P(darray, Pass(handle));

  AssertHookDestroyed(1, { C_DArray_destroy(darray); });

  deallocate(darray);
  refs--; // reset refs after deallocation
}

static void test_C_DArray_to_array_PR(void** state) {
  (void)state;

  /* test passing */ {
    C_DArray* darray = C_DArray_new();
    TestHook(C_DArray, darray);
    AssertHookDestroyed(1, { Unref(C_DArray_to_array_PR(Pass(darray))); });
  }

  /* test logic */ {
    C_DArray* darray = C_DArray_new();
    C_DArray_push_P(darray, Pass(C_Handle_u32_new(1)));
    C_DArray_push_P(darray, Pass(C_Handle_u32_new(2)));
    C_DArray_push_P(darray, Pass(C_Handle_u32_new(3)));
    C_DArray_push_P(darray, Pass(C_Handle_u32_new(4)));

    C_Array* reference_array = C_Array_new(4);
    C_Array_put_P(reference_array, 0, Pass(C_Handle_u32_new(1)));
    C_Array_put_P(reference_array, 1, Pass(C_Handle_u32_new(2)));
    C_Array_put_P(reference_array, 2, Pass(C_Handle_u32_new(3)));
    C_Array_put_P(reference_array, 3, Pass(C_Handle_u32_new(4)));

    C_Array* result = C_DArray_to_array_PR(darray);

    assert_true(C_Array_equals(reference_array, result));

    Unref(darray);
    Unref(reference_array);
    Unref(result);
  }
}

static void test_C_DArray_push_P(void** state) {
  (void)state;

  /* test passing */ {
    C_DArray* darray = C_DArray_new();
    TestHook(C_DArray, darray);

    C_Handle_u32* handle = C_Handle_u32_new(10);
    TestHook(C_Handle_u32, handle);

    AssertHookDestroyed(2, { C_DArray_push_P(Pass(darray), Pass(handle)); });
  }

  /* test logic */ {
    C_DArray* darray = C_DArray_new();

    C_DArray_push_P(darray, Pass(C_Handle_u32_new(0)));
    C_DArray_push_P(darray, Pass(C_Handle_u32_new(1)));
    C_DArray_push_P(darray, Pass(C_Handle_u32_new(2)));
    C_DArray_push_P(darray, Pass(C_Handle_u32_new(3)));

    assert_int_equal(4, C_DArray_get_len(darray));

    C_DArrayForeach(
      darray, { assert_int_equal(iter, C_Handle_u32_get_value(value)); });

    Unref(darray);
  }
}

static void test_C_DArray_push_front_P(void** state) {
  (void)state;

  /* test passing */ {
    C_DArray* darray = C_DArray_new();
    TestHook(C_DArray, darray);

    C_Handle_u32* handle = C_Handle_u32_new(10);
    TestHook(C_Handle_u32, handle);

    AssertHookDestroyed(
      2, { C_DArray_push_front_P(Pass(darray), Pass(handle)); });
  }
}

static void test_C_DArray_pop_R(void** state) {
  (void)state;

  C_DArray* darray = C_DArray_new();
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(10)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(20)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(30)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(40)));

  C_Handle_u32* pop = C_DArray_pop_R(darray);

  assert_int_equal(3, C_DArray_get_len(darray));
  assert_int_equal(40, C_Handle_u32_get_value(pop));
  assert_int_equal(30, C_Handle_u32_get_value(C_DArray_peek_B(darray)));

  Unref(pop);
  Unref(darray);
}

static void test_C_DArray_pop_front_R(void** state) {
  (void)state;

  C_DArray* darray = C_DArray_new();
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(10)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(20)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(30)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(40)));

  C_Handle_u32* pop = C_DArray_pop_front_R(darray);

  assert_int_equal(3, C_DArray_get_len(darray));
  assert_int_equal(10, C_Handle_u32_get_value(pop));
  assert_int_equal(20, C_Handle_u32_get_value(C_DArray_peek_front_B(darray)));

  Unref(pop);
  Unref(darray);
}

static void test_C_DArray_peek_B(void** state) {
  (void)state;

  C_DArray* darray = C_DArray_new();
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(10)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(20)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(30)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(40)));

  C_Handle_u32* peek = C_DArray_peek_B(darray);

  assert_int_equal(4, C_DArray_get_len(darray));
  assert_int_equal(40, C_Handle_u32_get_value(peek));

  Unref(darray);
}

static void test_C_DArray_peek_R(void** state) {
  (void)state;

  C_DArray* darray = C_DArray_new();
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(10)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(20)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(30)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(40)));

  C_Handle_u32* peek = C_DArray_peek_R(darray);

  assert_int_equal(4, C_DArray_get_len(darray));
  assert_int_equal(40, C_Handle_u32_get_value(peek));

  Unref(darray);
  Unref(peek);
}

static void test_C_DArray_peek_front_B(void** state) {
  (void)state;

  C_DArray* darray = C_DArray_new();
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(10)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(20)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(30)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(40)));

  C_Handle_u32* peek = C_DArray_peek_front_B(darray);

  assert_int_equal(4, C_DArray_get_len(darray));
  assert_int_equal(10, C_Handle_u32_get_value(peek));

  Unref(darray);
}

static void test_C_DArray_peek_front_R(void** state) {
  (void)state;

  C_DArray* darray = C_DArray_new();
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(10)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(20)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(30)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(40)));

  C_Handle_u32* peek = C_DArray_peek_front_R(darray);

  assert_int_equal(4, C_DArray_get_len(darray));
  assert_int_equal(10, C_Handle_u32_get_value(peek));

  Unref(darray);
  Unref(peek);
}

static void test_C_DArray_add_P(void** state) {
  (void)state;

  /* test passing */ {
    C_DArray* darray = C_DArray_new();
    TestHook(C_DArray, darray);
    C_Handle_u32* handle = C_Handle_u32_new(10);
    TestHook(C_Handle_u32, handle);

    AssertHookDestroyed(2, { C_DArray_add_P(Pass(darray), 0, Pass(handle)); });
  }

  /* test logic */ {
    C_DArray* darray = C_DArray_new();

    C_DArray_add_P(darray, 0, Pass(C_Handle_u32_new(2)));
    C_DArray_add_P(darray, 0, Pass(C_Handle_u32_new(0)));
    C_DArray_add_P(darray, 2, Pass(C_Handle_u32_new(3)));
    C_DArray_add_P(darray, 1, Pass(C_Handle_u32_new(1)));

    C_DArrayForeach(
      darray, { assert_int_equal(iter, C_Handle_u32_get_value(value)); });

    Unref(darray);
  }
}

static void test_C_DArray_at_R(void** state) {
  (void)state;

  C_DArray* darray = C_DArray_new();
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(10)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(20)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(30)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(40)));

  C_Handle_u32* val0 = C_DArray_at_R(darray, 0);
  C_Handle_u32* val1 = C_DArray_at_R(darray, 1);
  C_Handle_u32* val2 = C_DArray_at_R(darray, 2);
  C_Handle_u32* val3 = C_DArray_at_R(darray, 3);

  assert_int_equal(10, C_Handle_u32_get_value(val0));
  assert_int_equal(20, C_Handle_u32_get_value(val1));
  assert_int_equal(30, C_Handle_u32_get_value(val2));
  assert_int_equal(40, C_Handle_u32_get_value(val3));

  assert_int_equal(4, C_DArray_get_len(darray));

  Unref(val0);
  Unref(val1);
  Unref(val2);
  Unref(val3);
  Unref(darray);
}

static void test_C_DArray_at_B(void** state) {
  (void)state;

  C_DArray* darray = C_DArray_new();
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(10)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(20)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(30)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(40)));

  C_Handle_u32* val0 = C_DArray_at_B(darray, 0);
  C_Handle_u32* val1 = C_DArray_at_B(darray, 1);
  C_Handle_u32* val2 = C_DArray_at_B(darray, 2);
  C_Handle_u32* val3 = C_DArray_at_B(darray, 3);

  assert_int_equal(10, C_Handle_u32_get_value(val0));
  assert_int_equal(20, C_Handle_u32_get_value(val1));
  assert_int_equal(30, C_Handle_u32_get_value(val2));
  assert_int_equal(40, C_Handle_u32_get_value(val3));

  assert_int_equal(4, C_DArray_get_len(darray));

  Unref(darray);
}

static void test_C_DArray_remove_R(void** state) {
  (void)state;

  C_DArray* darray = C_DArray_new();
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(10)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(20)));

  C_Handle_u32* val1 = C_DArray_remove_R(darray, 1);
  C_Handle_u32* val0 = C_DArray_remove_R(darray, 0);

  assert_int_equal(20, C_Handle_u32_get_value(val1));
  assert_int_equal(10, C_Handle_u32_get_value(val0));

  assert_int_equal(0, C_DArray_get_len(darray));

  Unref(val0);
  Unref(val1);
  Unref(darray);
}

static void test_C_DArray_resize(void** state) {
  (void)state;

  C_DArray* darray = C_DArray_new();

  C_DArray_resize(darray, 10);
  assert_int_equal(10, C_DArray_get_cap(darray));
  C_DArray_resize(darray, 5);
  assert_int_equal(5, C_DArray_get_cap(darray));

  Unref(darray);
}

static void test_C_DArray_compress(void** state) {
  (void)state;

  C_DArray* darray = C_DArray_new();
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(10)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(20)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(30)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(40)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(50)));

  C_DArray_compress(darray);
  assert_int_equal(5, C_DArray_get_cap(darray));

  C_DArray_push_P(darray, Pass(C_Handle_u32_new(50)));

  Unref(darray);
}

static void test_C_DArray_clear(void** state) {
  (void)state;

  C_DArray* darray = C_DArray_new();
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(10)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(20)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(30)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(40)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(50)));

  C_DArray_clear(darray);
  assert_int_equal(0, C_DArray_get_len(darray));

  C_DArray_push_P(darray, Pass(C_Handle_u32_new(50)));

  Unref(darray);
}

static void test_C_DArray_equals(void** state) {
  (void)state;

  C_DArray* darray = C_DArray_new();
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(10)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(20)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(30)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(40)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(50)));

  C_DArray* darray2 = C_DArray_new();
  C_DArray_push_P(darray2, Pass(C_Handle_u32_new(10)));
  C_DArray_push_P(darray2, Pass(C_Handle_u32_new(20)));
  C_DArray_push_P(darray2, Pass(C_Handle_u32_new(30)));
  C_DArray_push_P(darray2, Pass(C_Handle_u32_new(40)));
  C_DArray_push_P(darray2, Pass(C_Handle_u32_new(50)));

  assert_true(C_DArray_equals(darray, darray2));

  Unref(C_DArray_pop_R(darray));

  assert_false(C_DArray_equals(darray, darray2));

  Unref(darray);
  Unref(darray2);
}

static void test_C_DArray_to_str_format_R(void** state) {
  (void)state;

  C_DArray* darray = C_DArray_new();
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(1)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(2)));
  C_DArray_push_P(darray, Pass(C_Handle_u32_new(3)));

  C_String* correct_result = S("{1, 2, 3}");
  C_String* format = S("start={;end=};sep=, ");
  C_String* result = C_DArray_to_str_format_R(darray, format);

  assert_true(C_String_equals(correct_result, result));

  Unref(format);
  Unref(correct_result);
  Unref(result);
  Unref(darray);
}

int main(void) {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_C_DArrayForeach),
    cmocka_unit_test(test_C_DArray_new),
    cmocka_unit_test(test_C_DArray_new_cap),
    cmocka_unit_test(test_C_DArray_destroy),
    cmocka_unit_test(test_C_DArray_to_array_PR),
    cmocka_unit_test(test_C_DArray_push_P),
    cmocka_unit_test(test_C_DArray_push_front_P),
    cmocka_unit_test(test_C_DArray_pop_R),
    cmocka_unit_test(test_C_DArray_pop_front_R),
    cmocka_unit_test(test_C_DArray_peek_R),
    cmocka_unit_test(test_C_DArray_peek_B),
    cmocka_unit_test(test_C_DArray_peek_front_R),
    cmocka_unit_test(test_C_DArray_peek_front_B),
    cmocka_unit_test(test_C_DArray_add_P),
    cmocka_unit_test(test_C_DArray_at_R),
    cmocka_unit_test(test_C_DArray_at_B),
    cmocka_unit_test(test_C_DArray_remove_R),
    cmocka_unit_test(test_C_DArray_resize),
    cmocka_unit_test(test_C_DArray_compress),
    cmocka_unit_test(test_C_DArray_clear),
    cmocka_unit_test(test_C_DArray_equals),
    cmocka_unit_test(test_C_DArray_to_str_format_R),
  };

  return cmocka_run_group_tests(tests, null, test_teardown);
}
