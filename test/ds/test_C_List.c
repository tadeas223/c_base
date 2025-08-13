// clang-format off
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>
// clang-format on

#include "../test_helpers.h"
#include "c_base/base/strings/strings.h"
#include <c_base/base/memory/handles.h>
#include <c_base/base/memory/objects.h>
#include <c_base/base/varargs.h>
#include <c_base/ds/C_Array.h>
#include <c_base/ds/C_List.h>
#include <c_base/os/os_io.h>

CreateTestHook(C_List, C_List_destroy)
CreateTestHook(C_Handle_u32, C_Handle_u32_destroy)

static void test_C_ListForeach(void** state) {
  (void)state;

  C_List* list = C_List_new();
  C_List_push_P(list, Pass(C_Handle_u32_new(0)));
  C_List_push_P(list, Pass(C_Handle_u32_new(1)));
  C_List_push_P(list, Pass(C_Handle_u32_new(2)));
  C_List_push_P(list, Pass(C_Handle_u32_new(3)));

  C_ListForeach(
    list, { assert_int_equal(iter, C_Handle_bool_get_value(value)); });

  Unref(list);
}

static void test_C_List_new(void** state) {
  (void)state;
  C_List* list = C_List_new();

  AssertClassEqual(list, ClassObject_id);
  assert_int_equal(C_List_get_len(list), 0);
  /* capacity could be changed later */

  Unref(list);
}

static void test_C_List_destroy(void** state) {
  (void)state;

  C_List* list = C_List_new();

  C_Handle_u32* handle = C_Handle_u32_new(10);
  TestHook(C_Handle_u32, handle);

  C_List_push_P(list, Pass(handle));

  AssertHookDestroyed(1, { C_List_destroy(list); });

  deallocate(list);
  refs--; // reset refs after deallocation
}

static void test_C_List_to_array_PR(void** state) {
  (void)state;

  /* test passing */ {
    C_List* list = C_List_new();
    TestHook(C_List, list);
    AssertHookDestroyed(1, { Unref(C_List_to_array_PR(Pass(list))); });
  }

  /* test logic */ {
    C_List* list = C_List_new();
    C_List_push_P(list, Pass(C_Handle_u32_new(1)));
    C_List_push_P(list, Pass(C_Handle_u32_new(2)));
    C_List_push_P(list, Pass(C_Handle_u32_new(3)));
    C_List_push_P(list, Pass(C_Handle_u32_new(4)));

    C_Array* reference_array = C_Array_new(4);
    C_Array_put_P(reference_array, 0, Pass(C_Handle_u32_new(1)));
    C_Array_put_P(reference_array, 1, Pass(C_Handle_u32_new(2)));
    C_Array_put_P(reference_array, 2, Pass(C_Handle_u32_new(3)));
    C_Array_put_P(reference_array, 3, Pass(C_Handle_u32_new(4)));

    C_Array* result = C_List_to_array_PR(list);

    assert_true(C_Array_equals(reference_array, result));

    Unref(list);
    Unref(reference_array);
    Unref(result);
  }
}

static void test_C_List_push_P(void** state) {
  (void)state;

  /* test passing */ {
    C_List* list = C_List_new();
    TestHook(C_List, list);

    C_Handle_u32* handle = C_Handle_u32_new(10);
    TestHook(C_Handle_u32, handle);

    AssertHookDestroyed(2, { C_List_push_P(Pass(list), Pass(handle)); });
  }

  /* test logic */ {
    C_List* list = C_List_new();

    C_List_push_P(list, Pass(C_Handle_u32_new(0)));
    C_List_push_P(list, Pass(C_Handle_u32_new(1)));
    C_List_push_P(list, Pass(C_Handle_u32_new(2)));
    C_List_push_P(list, Pass(C_Handle_u32_new(3)));

    assert_int_equal(4, C_List_get_len(list));

    C_ListForeach(
      list, { assert_int_equal(iter, C_Handle_u32_get_value(value)); });

    Unref(list);
  }
}

static void test_C_List_push_front_P(void** state) {
  (void)state;

  /* test passing */ {
    C_List* list = C_List_new();
    TestHook(C_List, list);

    C_Handle_u32* handle = C_Handle_u32_new(10);
    TestHook(C_Handle_u32, handle);

    AssertHookDestroyed(2, { C_List_push_front_P(Pass(list), Pass(handle)); });
  }
}

static void test_C_List_pop_R(void** state) {
  (void)state;

  C_List* list = C_List_new();
  C_List_push_P(list, Pass(C_Handle_u32_new(10)));
  C_List_push_P(list, Pass(C_Handle_u32_new(20)));
  C_List_push_P(list, Pass(C_Handle_u32_new(30)));
  C_List_push_P(list, Pass(C_Handle_u32_new(40)));

  C_Handle_u32* pop = C_List_pop_R(list);

  assert_int_equal(3, C_List_get_len(list));
  assert_int_equal(40, C_Handle_u32_get_value(pop));
  assert_int_equal(30, C_Handle_u32_get_value(C_List_peek_B(list)));

  Unref(pop);
  Unref(list);
}

static void test_C_List_pop_front_R(void** state) {
  (void)state;

  C_List* list = C_List_new();
  C_List_push_P(list, Pass(C_Handle_u32_new(10)));
  C_List_push_P(list, Pass(C_Handle_u32_new(20)));
  C_List_push_P(list, Pass(C_Handle_u32_new(30)));
  C_List_push_P(list, Pass(C_Handle_u32_new(40)));

  C_Handle_u32* pop = C_List_pop_front_R(list);

  assert_int_equal(3, C_List_get_len(list));
  assert_int_equal(10, C_Handle_u32_get_value(pop));
  assert_int_equal(20, C_Handle_u32_get_value(C_List_peek_front_B(list)));

  Unref(pop);
  Unref(list);
}

static void test_C_List_peek_B(void** state) {
  (void)state;

  C_List* list = C_List_new();
  C_List_push_P(list, Pass(C_Handle_u32_new(10)));
  C_List_push_P(list, Pass(C_Handle_u32_new(20)));
  C_List_push_P(list, Pass(C_Handle_u32_new(30)));
  C_List_push_P(list, Pass(C_Handle_u32_new(40)));

  C_Handle_u32* peek = C_List_peek_B(list);

  assert_int_equal(4, C_List_get_len(list));
  assert_int_equal(40, C_Handle_u32_get_value(peek));

  Unref(list);
}

static void test_C_List_peek_R(void** state) {
  (void)state;

  C_List* list = C_List_new();
  C_List_push_P(list, Pass(C_Handle_u32_new(10)));
  C_List_push_P(list, Pass(C_Handle_u32_new(20)));
  C_List_push_P(list, Pass(C_Handle_u32_new(30)));
  C_List_push_P(list, Pass(C_Handle_u32_new(40)));

  C_Handle_u32* peek = C_List_peek_R(list);

  assert_int_equal(4, C_List_get_len(list));
  assert_int_equal(40, C_Handle_u32_get_value(peek));

  Unref(list);
  Unref(peek);
}

static void test_C_List_peek_front_B(void** state) {
  (void)state;

  C_List* list = C_List_new();
  C_List_push_P(list, Pass(C_Handle_u32_new(10)));
  C_List_push_P(list, Pass(C_Handle_u32_new(20)));
  C_List_push_P(list, Pass(C_Handle_u32_new(30)));
  C_List_push_P(list, Pass(C_Handle_u32_new(40)));

  C_Handle_u32* peek = C_List_peek_front_B(list);

  assert_int_equal(4, C_List_get_len(list));
  assert_int_equal(10, C_Handle_u32_get_value(peek));

  Unref(list);
}

static void test_C_List_peek_front_R(void** state) {
  (void)state;

  C_List* list = C_List_new();
  C_List_push_P(list, Pass(C_Handle_u32_new(10)));
  C_List_push_P(list, Pass(C_Handle_u32_new(20)));
  C_List_push_P(list, Pass(C_Handle_u32_new(30)));
  C_List_push_P(list, Pass(C_Handle_u32_new(40)));

  C_Handle_u32* peek = C_List_peek_front_R(list);

  assert_int_equal(4, C_List_get_len(list));
  assert_int_equal(10, C_Handle_u32_get_value(peek));

  Unref(list);
  Unref(peek);
}

static void test_C_List_add_P(void** state) {
  (void)state;

  /* test passing */ {
    C_List* list = C_List_new();
    TestHook(C_List, list);
    C_Handle_u32* handle = C_Handle_u32_new(10);
    TestHook(C_Handle_u32, handle);

    AssertHookDestroyed(2, { C_List_add_P(Pass(list), 0, Pass(handle)); });
  }

  /* test logic */ {
    C_List* list = C_List_new();

    C_List_add_P(list, 0, Pass(C_Handle_u32_new(2)));
    C_List_add_P(list, 0, Pass(C_Handle_u32_new(0)));
    C_List_add_P(list, 2, Pass(C_Handle_u32_new(3)));
    C_List_add_P(list, 1, Pass(C_Handle_u32_new(1)));

    C_ListForeach(
      list, { assert_int_equal(iter, C_Handle_u32_get_value(value)); });

    Unref(list);
  }
}

static void test_C_List_at_R(void** state) {
  (void)state;

  C_List* list = C_List_new();
  C_List_push_P(list, Pass(C_Handle_u32_new(10)));
  C_List_push_P(list, Pass(C_Handle_u32_new(20)));
  C_List_push_P(list, Pass(C_Handle_u32_new(30)));
  C_List_push_P(list, Pass(C_Handle_u32_new(40)));

  C_Handle_u32* val0 = C_List_at_R(list, 0);
  C_Handle_u32* val1 = C_List_at_R(list, 1);
  C_Handle_u32* val2 = C_List_at_R(list, 2);
  C_Handle_u32* val3 = C_List_at_R(list, 3);

  assert_int_equal(10, C_Handle_u32_get_value(val0));
  assert_int_equal(20, C_Handle_u32_get_value(val1));
  assert_int_equal(30, C_Handle_u32_get_value(val2));
  assert_int_equal(40, C_Handle_u32_get_value(val3));

  assert_int_equal(4, C_List_get_len(list));

  Unref(val0);
  Unref(val1);
  Unref(val2);
  Unref(val3);
  Unref(list);
}

static void test_C_List_at_B(void** state) {
  (void)state;

  C_List* list = C_List_new();
  C_List_push_P(list, Pass(C_Handle_u32_new(10)));
  C_List_push_P(list, Pass(C_Handle_u32_new(20)));
  C_List_push_P(list, Pass(C_Handle_u32_new(30)));
  C_List_push_P(list, Pass(C_Handle_u32_new(40)));

  C_Handle_u32* val0 = C_List_at_B(list, 0);
  C_Handle_u32* val1 = C_List_at_B(list, 1);
  C_Handle_u32* val2 = C_List_at_B(list, 2);
  C_Handle_u32* val3 = C_List_at_B(list, 3);

  assert_int_equal(10, C_Handle_u32_get_value(val0));
  assert_int_equal(20, C_Handle_u32_get_value(val1));
  assert_int_equal(30, C_Handle_u32_get_value(val2));
  assert_int_equal(40, C_Handle_u32_get_value(val3));

  assert_int_equal(4, C_List_get_len(list));

  Unref(list);
}

static void test_C_List_remove_R(void** state) {
  (void)state;

  C_List* list = C_List_new();
  C_List_push_P(list, Pass(C_Handle_u32_new(10)));
  C_List_push_P(list, Pass(C_Handle_u32_new(20)));

  C_Handle_u32* val1 = C_List_remove_R(list, 1);
  C_Handle_u32* val0 = C_List_remove_R(list, 0);

  assert_int_equal(20, C_Handle_u32_get_value(val1));
  assert_int_equal(10, C_Handle_u32_get_value(val0));

  assert_int_equal(0, C_List_get_len(list));

  Unref(val0);
  Unref(val1);
  Unref(list);
}

static void test_C_List_clear(void** state) {
  (void)state;

  C_List* list = C_List_new();
  C_List_push_P(list, Pass(C_Handle_u32_new(10)));
  C_List_push_P(list, Pass(C_Handle_u32_new(20)));
  C_List_push_P(list, Pass(C_Handle_u32_new(30)));
  C_List_push_P(list, Pass(C_Handle_u32_new(40)));
  C_List_push_P(list, Pass(C_Handle_u32_new(50)));

  C_List_clear(list);
  assert_int_equal(0, C_List_get_len(list));

  C_List_push_P(list, Pass(C_Handle_u32_new(50)));

  Unref(list);
}

static void test_C_List_equals(void** state) {
  (void)state;

  C_List* list = C_List_new();
  C_List_push_P(list, Pass(C_Handle_u32_new(10)));
  C_List_push_P(list, Pass(C_Handle_u32_new(20)));
  C_List_push_P(list, Pass(C_Handle_u32_new(30)));
  C_List_push_P(list, Pass(C_Handle_u32_new(40)));
  C_List_push_P(list, Pass(C_Handle_u32_new(50)));

  C_List* list2 = C_List_new();
  C_List_push_P(list2, Pass(C_Handle_u32_new(10)));
  C_List_push_P(list2, Pass(C_Handle_u32_new(20)));
  C_List_push_P(list2, Pass(C_Handle_u32_new(30)));
  C_List_push_P(list2, Pass(C_Handle_u32_new(40)));
  C_List_push_P(list2, Pass(C_Handle_u32_new(50)));

  assert_true(C_List_equals(list, list2));

  Unref(C_List_pop_R(list));

  assert_false(C_List_equals(list, list2));

  Unref(list);
  Unref(list2);
}

static void test_C_List_to_str_format_R(void** state) {
  (void)state;

  C_List* list = C_List_new();
  C_List_push_P(list, Pass(C_Handle_u32_new(1)));
  C_List_push_P(list, Pass(C_Handle_u32_new(2)));
  C_List_push_P(list, Pass(C_Handle_u32_new(3)));

  C_String* correct_result = S("{1, 2, 3}");
  C_String* format = S("start={;end=};sep=, ");
  C_String* result = C_List_to_str_format_R(list, format);

  assert_true(C_String_equals(correct_result, result));

  Unref(format);
  Unref(correct_result);
  Unref(result);
  Unref(list);
}

int main(void) {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_C_ListForeach),
    cmocka_unit_test(test_C_List_new),
    cmocka_unit_test(test_C_List_destroy),
    cmocka_unit_test(test_C_List_to_array_PR),
    cmocka_unit_test(test_C_List_push_P),
    cmocka_unit_test(test_C_List_push_front_P),
    cmocka_unit_test(test_C_List_pop_R),
    cmocka_unit_test(test_C_List_pop_front_R),
    cmocka_unit_test(test_C_List_peek_R),
    cmocka_unit_test(test_C_List_peek_B),
    cmocka_unit_test(test_C_List_peek_front_R),
    cmocka_unit_test(test_C_List_peek_front_B),
    cmocka_unit_test(test_C_List_add_P),
    cmocka_unit_test(test_C_List_at_R),
    cmocka_unit_test(test_C_List_at_B),
    cmocka_unit_test(test_C_List_remove_R),
    cmocka_unit_test(test_C_List_clear),
    cmocka_unit_test(test_C_List_equals),
    cmocka_unit_test(test_C_List_to_str_format_R),
  };

  return cmocka_run_group_tests(tests, null, test_teardown);
}
