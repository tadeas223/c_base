// clang-format off
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>
// clang-format on

#include "../test_helpers.h"
#include "c_base/base/memory/allocator.h"
#include "c_base/base/strings/strings.h"
#include "c_base/ds/C_HashTable.h"
#include <c_base/base/memory/handles.h>
#include <c_base/base/memory/objects.h>
#include <c_base/base/varargs.h>
#include <c_base/ds/ds.h>
#include <c_base/os/os_io.h>

CreateTestHook(C_HashTable, C_HashTable_destroy)
CreateTestHook(C_Handle_u32, C_Handle_u32_destroy)

static void test_C_HashTable_new(void** state) {
  (void)state;

  C_HashTable* table = C_HashTable_new();

  AssertClassEqual(table, ClassObject_id);

  Unref(table);
}

static void test_C_HashTable_new_cap(void** state) {
  (void)state;

  C_HashTable* table = C_HashTable_new_cap(2048);

  AssertClassEqual(table, ClassObject_id);
  assert_int_equal(2048, C_HashTable_get_cap(table));

  Unref(table);
}

static void test_C_HashTable_destroy(void** state) {
  (void)state;

  C_HashTable* table = C_HashTable_new_cap(2048);

  C_Handle_u32* key = C_Handle_u32_new(10);
  C_Handle_u32* value = C_Handle_u32_new(20);

  TestHook(C_Handle_u32, key);
  TestHook(C_Handle_u32, value);

  C_HashTable_put_P(table, Pass(key), Pass(value));

  AssertHookDestroyed(2, { C_HashTable_destroy(table); });

  deallocate(table);
  refs--; // reset refs after deallcation
}

static void test_C_HashTable_put_P(void** state) {
  (void)state;

  /* test passing */ {
    C_HashTable* table = C_HashTable_new();
    TestHook(C_HashTable, table);

    C_Handle_u32* key = C_Handle_u32_new(10);
    C_Handle_u32* value = C_Handle_u32_new(20);

    TestHook(C_Handle_u32, key);
    TestHook(C_Handle_u32, value);

    AssertHookDestroyed(
      3, { C_HashTable_put_P(Pass(table), Pass(key), Pass(value)); });
  }

  /* test logic */ {
    C_HashTable* table = C_HashTable_new();
    TestHook(C_HashTable, table);

    C_Handle_u32* key = C_Handle_u32_new(10);
    C_Handle_u32* value = C_Handle_u32_new(20);

    C_HashTable_put_P(table, key, value);
    C_HashTable_put_P(table, value, key);

    assert_int_equal(value, C_HashTable_at_PB(table, key));
    assert_int_equal(key, C_HashTable_at_PB(table, value));

    Unref(table);
    Unref(key);
    Unref(value);
  }
}

static void test_C_HashTable_at_PB(void** state) {
  (void)state;

  C_HashTable* table = C_HashTable_new();
  TestHook(C_HashTable, table);

  C_Handle_u32* key = C_Handle_u32_new(10);
  C_Handle_u32* value = C_Handle_u32_new(20);

  C_HashTable_put_P(table, key, value);
  C_HashTable_put_P(table, value, key);

  assert_int_equal(value, C_HashTable_at_PB(table, key));
  assert_int_equal(key, C_HashTable_at_PB(table, value));

  Unref(table);
  Unref(key);
  Unref(value);
}

static void test_C_HashTable_at_PR(void** state) {
  (void)state;

  C_HashTable* table = C_HashTable_new();

  C_Handle_u32* key = C_Handle_u32_new(10);
  C_Handle_u32* value = C_Handle_u32_new(20);

  C_HashTable_put_P(table, key, value);
  C_HashTable_put_P(table, value, key);

  C_Handle_u32* at_key = C_HashTable_at_PR(table, key);
  C_Handle_u32* at_value = C_HashTable_at_PR(table, value);

  assert_int_equal(value, at_key);
  assert_int_equal(key, at_value);

  Unref(table);
  Unref(key);
  Unref(value);
  Unref(at_key);
  Unref(at_value);
}

static void test_C_HashTable_contains_P(void** state) {
  (void)state;

  /* test passing */ {
    C_HashTable* table = C_HashTable_new();
    TestHook(C_HashTable, table);

    C_Handle_u32* key = C_Handle_u32_new(10);
    TestHook(C_Handle_u32, key);

    AssertHookDestroyed(2, { C_HashTable_contains_P(Pass(table), Pass(key)); });
  }

  /* test logic */ {
    C_HashTable* table = C_HashTable_new();

    C_Handle_u32* key = C_Handle_u32_new(10);
    C_Handle_u32* value = C_Handle_u32_new(20);

    C_HashTable_put_P(table, key, value);
    C_HashTable_put_P(table, value, key);

    C_Handle_u32* at_key = C_HashTable_at_PR(table, key);
    C_Handle_u32* at_value = C_HashTable_at_PR(table, value);

    assert_true(C_HashTable_contains_P(table, key));
    assert_true(C_HashTable_contains_P(table, value));

    assert_false(C_HashTable_contains_P(table, Pass(C_Handle_u32_new(100))));

    Unref(table);
    Unref(key);
    Unref(value);
    Unref(at_key);
    Unref(at_value);
  }
}

static void test_C_HashTable_remove_PR(void** state) {
  (void)state;

  /* test passing */ {
    C_HashTable* table = C_HashTable_new();
    TestHook(C_HashTable, table);

    C_Handle_u32* key = C_Handle_u32_new(10);
    TestHook(C_Handle_u32, key);

    C_HashTable_put_P(table, key, Pass(C_Handle_u32_new(200)));

    AssertHookDestroyed(
      2, { Unref(C_HashTable_remove_PR(Pass(table), Pass(key))); });
  }

  /* test logic */ {
    C_HashTable* table = C_HashTable_new();

    C_Handle_u32* key = C_Handle_u32_new(10);

    C_HashTable_put_P(table, key, Pass(C_Handle_u32_new(200)));

    C_Handle_u32* value = C_HashTable_remove_PR(table, key);
    assert_int_equal(200, C_Handle_u32_get_value(value));

    assert_false(C_HashTable_contains_P(table, key));

    Unref(value);
    Unref(key);
    Unref(table);
  }
}

static void test_C_HashTable_clear(void** state) {
  (void)state;

  C_HashTable* table = C_HashTable_new();
  C_Handle_u32* key = C_Handle_u32_new(10);

  C_HashTable_put_P(table, key, Pass(C_Handle_u32_new(200)));

  C_HashTable_clear(table);

  assert_false(C_HashTable_contains_P(table, key));

  Unref(key);
  Unref(table);
}

static void test_C_HashTable_equals(void** state) {
  (void)state;

  C_HashTable* table = C_HashTable_new();
  C_HashTable_put_P(
    table, Pass(C_Handle_u32_new(10)), Pass(C_Handle_u32_new(10)));
  C_HashTable_put_P(
    table, Pass(C_Handle_u32_new(20)), Pass(C_Handle_u32_new(10)));
  C_HashTable_put_P(
    table, Pass(C_Handle_u32_new(30)), Pass(C_Handle_u32_new(10)));
  C_HashTable_put_P(
    table, Pass(C_Handle_u32_new(40)), Pass(C_Handle_u32_new(10)));
  C_HashTable_put_P(
    table, Pass(C_Handle_u32_new(50)), Pass(C_Handle_u32_new(10)));

  C_HashTable* table2 = C_HashTable_new();
  C_HashTable_put_P(
    table2, Pass(C_Handle_u32_new(10)), Pass(C_Handle_u32_new(10)));
  C_HashTable_put_P(
    table2, Pass(C_Handle_u32_new(20)), Pass(C_Handle_u32_new(10)));
  C_HashTable_put_P(
    table2, Pass(C_Handle_u32_new(30)), Pass(C_Handle_u32_new(10)));
  C_HashTable_put_P(
    table2, Pass(C_Handle_u32_new(40)), Pass(C_Handle_u32_new(10)));
  C_HashTable_put_P(
    table2, Pass(C_Handle_u32_new(50)), Pass(C_Handle_u32_new(10)));

  assert_true(C_HashTable_equals(table, table2));

  Unref(C_HashTable_remove_PR(table, Pass(C_Handle_u32_new(10))));

  assert_false(C_HashTable_equals(table, table2));

  Unref(table);
  Unref(table2);
}

int main(void) {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_C_HashTable_new),
    cmocka_unit_test(test_C_HashTable_new_cap),
    cmocka_unit_test(test_C_HashTable_destroy),
    cmocka_unit_test(test_C_HashTable_put_P),
    cmocka_unit_test(test_C_HashTable_at_PB),
    cmocka_unit_test(test_C_HashTable_at_PR),
    cmocka_unit_test(test_C_HashTable_remove_PR),
    cmocka_unit_test(test_C_HashTable_clear),
    cmocka_unit_test(test_C_HashTable_contains_P),
    cmocka_unit_test(test_C_HashTable_equals),
  };

  return cmocka_run_group_tests(tests, null, test_teardown);
}
