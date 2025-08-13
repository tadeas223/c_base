#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include <c_base/base/macros.h>
#include <c_base/base/memory/objects.h>

extern u32 test_hook_destroyed;

#define AssertClassEqual(ptr, ID)                                              \
  do {                                                                         \
    Class* _obj = (void*)ptr;                                                  \
    assert_int_equal(_obj->id, ID);                                            \
  } while (0)

#define TestHook(T, obj)                                                       \
  do {                                                                         \
    ClassObject* _obj = (void*)obj;                                            \
    _obj->destroy = test_hook_destroy_##T;                                     \
  } while (0)

#define CreateTestHook(T, destroy_func)                                        \
  static void test_hook_destroy_##T(void* self) {                              \
    destroy_func(self);                                                        \
    test_hook_destroyed++;                                                     \
  }

#define AssertHookDestroyed(num, code)                                         \
  do {                                                                         \
    test_hook_destroyed = 0;                                                   \
    {code} assert_int_equal(num, test_hook_destroyed);                         \
  } while (0)

int check_refs_equals_zero(void** state);

int test_teardown(void** state);

#endif
