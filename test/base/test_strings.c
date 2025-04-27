#include "base/arena.h"
#include "os/os_mem.h"
#include "base/types.h"
#include "base/strings.h"
#include "base/strdef.h"
#include "test_helpers.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

static void test_str_at(void **state) {
    String string = S("hello, world");

    for(u32 i = 0; i < string.len; i++) {
        assert_true(string.chars[i] == str_at(string, i)); 
    }
}

static void test_str_set(void **state) {
    Stack_StringCopy(mut, S("hello, world"));

    for(u32 i = 0; i < mut.str.len; i++) {
        str_set(mut, i, 'a'); 
    }

    for(u32 i = 0; i < mut.str.len; i++) {
        assert_true(str_at(mut.str, i) == 'a'); 
    }

}

static void test_str_copy(void **state) {
    Arena *arena = (Arena*)*state;
    
    String string = S("hello, world");
    StringMut copy = str_copy(arena, string);

    assert_true(str_equals(string, copy.str));
}

static void test_str_copy_to(void **state) {
    String string = S("hello, world");
    Stack_String(mut, string.len);

    str_copy_to(mut, string);
    assert_true(str_equals(mut.str, string));

    String string2 = S("hello");
    Stack_String(mut2, 2);

    str_copy_to(mut2, string2);
    assert_int_equal(mut2.str.len, 2);
    assert_true(str_at(mut2.str, 0) == 'h');
    assert_true(str_at(mut2.str, 1) == 'e');

    String string3 = S("he");
    Stack_String(mut3, 5);

    str_copy_to(mut3, string3);
    assert_true(str_at(mut3.str, 0) == 'h');
    assert_true(str_at(mut3.str, 1) == 'e');
}

static void test_str_find_char(void **state) {
    String string = S("hello world");

    Result(u32) r_char = str_find_char(string, 'h');
    assert_true(r_char.ok);
    assert_true(r_char.value == 0);
    
    Result(u32) r_char2 = str_find_char(string, 'l');
    assert_true(r_char2.ok);
    assert_true(r_char2.value == 2);
    
    Result(u32) r_char3 = str_find_char(string, 'z');
    assert_false(r_char3.ok);
    assert_true(r_char3.err.code == E_OutOfBounds);
    assert_true(r_char3.err.grp == EG_Strings);
}

static void test_str_equals(void **state) {
    String string = S("hello");
    String string2 = S("hello");

    String string3 = S("world");

    String string4 = S("hell");
   
    assert_true(str_equals(string, string2));
    assert_false(str_equals(string, string3));
    assert_false(str_equals(string2, string4));
}

static void test_str_to_cstr(void **state) {
    Arena *arena = (Arena*) *state;
    
    String string = S("hello, world");

    char* cstr = str_to_cstr(arena, string);

    u32 index = 0;
    while(cstr[index] != '\0') {
        assert_true(str_at(string, index) == cstr[index]);
        index++;
    }
    assert_int_equal(string.len, index);
}

static void test_str_from_cstr(void **state) {
    char* cstr = "hello, world";
    String string = str_from_cstr(cstr);
    assert_true(str_equals(string, S("hello, world")));
}

static void test_str_substr(void **state) {
    String string = S("hello, world");
    String substr1 = str_substr(string, 0, 5);
    String substr2 = str_substr(string, 7, 5);
    
    assert_true(str_equals(substr1, S("hello")));
    assert_true(str_equals(substr2, S("world")));
}

int main(void) {
    Arena arena;
    arena_init_base(&arena, os_memory_base());

    const struct CMUnitTest tests[] = {
        cmocka_unit_test_prestate(test_str_at, &arena), 
        cmocka_unit_test_prestate(test_str_set, &arena), 
        cmocka_unit_test_prestate(test_str_copy, &arena), 
        cmocka_unit_test_prestate(test_str_copy_to, &arena), 
        cmocka_unit_test_prestate(test_str_equals, &arena), 
        cmocka_unit_test_prestate(test_str_substr, &arena), 
        cmocka_unit_test_prestate(test_str_find_char, &arena), 
        cmocka_unit_test_prestate(test_str_from_cstr, &arena), 
        cmocka_unit_test_prestate(test_str_to_cstr, &arena), 
    };

    return cmocka_run_group_tests(tests, null, test_arena_teardown);
}
