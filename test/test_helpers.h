#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

int test_in_new_process(void (*func)(void));

int test_arena_setup(void **state);
int test_arena_teardown(void **state);

#endif
