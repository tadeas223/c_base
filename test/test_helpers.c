// clang-format off
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>
// clang-format on

#include "test_helpers.h"
#include "c_base/os/os_io.h"
#include <c_base/base/memory/objects.h>
#include <c_base/base/types.h>

u32 test_hook_destroyed = 0;

int test_teardown(void** state) {
  int r1 = check_refs_equals_zero(state);
  return r1;
}

int check_refs_equals_zero(void** state) {
  (void)state;
  assert_int_equal(0, refs);
  return 0;
}
