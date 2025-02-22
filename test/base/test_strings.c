#include "base/mem.h"
#include "os/os_mem.h"
#include <stdio.h>
#include <stdlib.h>

/* make cmocka work for c89 */
#define inline

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
#include "base/strings.h"


/****************************************
 * String creation
****************************************/

static void test_str8()
{
    /* idk what to test */
    char arr[] = "hello";
    String8 string = str8((u8*) arr, 5);

    assert_ptr_equal(arr, string.str);
    assert_int_equal(5, string.count);
}

static void test_str8_range()
{
    char arr[] = "hello";
    String8 string = str8_range((u8*) arr, (u8*) arr + 5);

    assert_ptr_equal(arr, string.str);
    assert_int_equal(5, string.count);
}

static void test_str8_alloc()
{
    m_Arena arena;
    m_arena_begin_base(&arena, os_memory_base());
    {
        String8Alloc a_string = str8_alloc(&arena, 5);

        assert_int_equal(5, a_string.string.count);

        a_string.string.str[0] = 'h';
        a_string.string.str[1] = 'e';
        a_string.string.str[2] = 'l';
        a_string.string.str[3] = 'l';
        a_string.string.str[4] = 'o';
    }
    m_arena_end(&arena);
}

static void test_str8_cstr_count()
{
    const char* cstr = "hello";
    String8 string = str8_cstr_count(cstr, 5);
    
    assert_int_equal(string.count, 5);
    assert_ptr_equal(string.str, cstr);
}

static void test_str8_cstr()
{
    const char* cstr = "hello";
    String8 string = str8_cstr(cstr);
    
    assert_int_equal(string.count, 5);
    assert_ptr_equal(string.str, cstr);
}

static const struct CMUnitTest grp_creation[] = {
    cmocka_unit_test(test_str8),
    cmocka_unit_test(test_str8_range),
    cmocka_unit_test(test_str8_alloc),
    cmocka_unit_test(test_str8_cstr_count),
    cmocka_unit_test(test_str8_cstr),
};


/****************************************
 * String manipulation
****************************************/

static void test_str8_to_cstr()
{
    m_Arena arena;
    m_arena_begin_base(&arena, os_memory_base());
    {
        String8 string = str8_cstr_count("hello", 5);
        const char* cstr = str8_to_cstr(&arena, string);

        assert_string_equal(cstr, "hello");
    }
    m_arena_end(&arena);
}

static void test_str8_substr()
{
    String8 string = str8_cstr("hello,world");
    String8 substr = str8_substr(string, 6, 5);
    String8 substr2 = str8_substr(string, 0, 5);
    
    assert_true(str8_equals(substr, Str8Lit("world")));
    assert_true(str8_equals(substr2, Str8Lit("hello")));
}

static void test_str8_split()
{
    m_Arena arena;
    m_arena_begin_base(&arena, os_memory_base());
    {
        String8 string = str8_cstr("hello,world");
        String8List split = str8_split(&arena, string, ',');
        
        assert_int_equal(split.node_count, 2);
        assert_int_equal(split.str_count, 10);

        assert_true(str8_equals(split.head->string, str8_cstr("hello")));
        assert_true(str8_equals(split.tail->string, str8_cstr("world")));
    }
    m_arena_reset(&arena);
    {
        String8 string = str8_cstr("hello,world");
        String8List split = str8_split(&arena, string, 'z');
        
        assert_int_equal(split.node_count, 1);
        assert_int_equal(split.str_count, 11);

        assert_null(split.tail);
    }
}

static void test_str8_join()
{
    m_Arena arena;
    m_arena_begin_base(&arena, os_memory_base());
    {
        String8 string = str8_cstr("hello,world");
        String8List split = str8_split(&arena, string, ',');
        
        String8Alloc a_join = str8_join(&arena, &split);

        assert_true(str8_equals(a_join.string, str8_cstr("helloworld")));
    }
    m_arena_end(&arena);
}

static const struct CMUnitTest grp_manipulation[] = {
    cmocka_unit_test(test_str8_to_cstr),
    cmocka_unit_test(test_str8_substr),
    cmocka_unit_test(test_str8_split),
    cmocka_unit_test(test_str8_join),
};


/****************************************
 * String list
****************************************/

static void test_str8_list_begin()
{
    String8List list;
    str8_list_begin(&list);
    
    assert_null(list.head);
    assert_null(list.tail);

    assert_int_equal(list.str_count, 0);
    assert_int_equal(list.node_count, 0);
}

static void test_str8_list_push()
{
    m_Arena arena;
    m_arena_begin_base(&arena, os_memory_base());
    {
        String8List list;
        str8_list_begin(&list);
        
        String8 string = str8_cstr("hello");
        String8 string2 = str8_cstr("world");
        String8 string3 = str8_cstr("aaa");
        
        str8_list_push(&arena, &list, string);
        
        assert_int_equal(list.node_count, 1);
        assert_int_equal(list.str_count, 5);
        assert_true(str8_equals(list.head->string, string));

        str8_list_push(&arena, &list, string2);

        assert_int_equal(list.node_count, 2);
        assert_int_equal(list.str_count, 10);
        assert_true(str8_equals(list.head->string, string));
        assert_true(str8_equals(list.tail->string, string2));
        
        str8_list_push(&arena, &list, string3);

        assert_int_equal(list.node_count, 3);
        assert_int_equal(list.str_count, 13);
        assert_true(str8_equals(list.head->string, string));
        assert_true(str8_equals(list.tail->string, string3));
        
        String8Alloc a_string = str8_join(&arena, &list);
        
        assert_true(str8_equals(a_string.string, str8_cstr("helloworldaaa")));
    }
    m_arena_end(&arena);
}

static void test_str8_list_pop()
{
    m_Arena arena;
    m_arena_begin_base(&arena, os_memory_base());
    {
        String8List list;
        str8_list_begin(&list);
        
        String8 string = str8_cstr("hello");
        String8 string2 = str8_cstr("world");
        String8 string3 = str8_cstr("aaa");
        
        str8_list_push(&arena, &list, string);
        str8_list_push(&arena, &list, string2);
        str8_list_push(&arena, &list, string3);
        
        str8_list_pop(&list);
        
        assert_int_equal(list.node_count, 2);
        assert_int_equal(list.str_count, 10);
        assert_true(str8_equals(list.head->string, string));
        assert_true(str8_equals(list.tail->string, string2));

        str8_list_pop(&list);

        assert_int_equal(list.node_count, 1);
        assert_int_equal(list.str_count, 5); assert_true(str8_equals(list.head->string, string));
        assert_null(list.tail);
        
        str8_list_pop(&list);
        
        assert_int_equal(list.node_count, 0);
        assert_int_equal(list.str_count, 0);
        assert_null(list.head);
        assert_null(list.tail);
    }
    m_arena_end(&arena);
}

static const struct CMUnitTest grp_string_list[] = {
    cmocka_unit_test(test_str8_list_begin),
    cmocka_unit_test(test_str8_list_push),
    cmocka_unit_test(test_str8_list_pop),
};

/****************************************
 * Simple finctions and macros
****************************************/

static void test_str8_equals()
{
    String8 string = str8_cstr("hello");
    String8 string2 = str8_cstr("hello");
    String8 string3 = str8_cstr("world");

    assert_true(str8_equals(string, string2));
    assert_false(str8_equals(string, string3));
}

static void test_str8_at()
{
    String8 string = str8_cstr("hello");
    
    assert_true(str8_at(string, 0) == 'h');
    assert_true(str8_at(string, 1) == 'e');
    assert_true(str8_at(string, 2) == 'l');
    assert_true(str8_at(string, 3) == 'l');
    assert_true(str8_at(string, 4) == 'o');
}

static const struct CMUnitTest grp_simple_functions[] = {
    cmocka_unit_test(test_str8_equals),
    cmocka_unit_test(test_str8_at),
};


/****************************************
 * Char checking
****************************************/

static void test_IsNumber()
{
    assert_true(IsNumber('0'));
    assert_true(IsNumber('4'));
    assert_true(IsNumber('9'));
    assert_false(IsNumber('a'));
    assert_false(IsNumber('c'));
}

static void test_IsUpper()
{
    assert_true(IsUpper('A'));
    assert_true(IsUpper('C'));
    assert_true(IsUpper('F'));
    assert_false(IsUpper('a'));
    assert_false(IsUpper('c'));
    assert_false(IsUpper('f'));
}

static void test_IsLower()
{
    assert_true(IsLower('a'));
    assert_true(IsLower('c'));
    assert_true(IsLower('f'));
    assert_false(IsLower('A'));
    assert_false(IsLower('C'));
    assert_false(IsLower('F'));
}

static void test_IsAlpha()
{
    assert_true(IsAlpha('A'));
    assert_true(IsAlpha('C'));
    assert_true(IsAlpha('F'));
    assert_true(IsAlpha('a'));
    assert_true(IsAlpha('c'));
    assert_true(IsAlpha('f'));
    assert_false(IsAlpha('0'));
    assert_false(IsAlpha('9'));
    assert_false(IsAlpha(' '));
}

static void test_IsAlphaNum()
{
    assert_true(IsAlphaNum('A'));
    assert_true(IsAlphaNum('C'));
    assert_true(IsAlphaNum('F'));
    assert_true(IsAlphaNum('a'));
    assert_true(IsAlphaNum('c'));
    assert_true(IsAlphaNum('f'));
    assert_true(IsAlphaNum('0'));
    assert_true(IsAlphaNum('9'));
    assert_false(IsAlphaNum(' '));
    assert_false(IsAlphaNum(','));
    assert_false(IsAlphaNum(':'));
}

static void test_NumFromAscii()
{
    assert_int_equal(0, NumFromAscii('0'));
    assert_int_equal(1, NumFromAscii('1'));
    assert_int_equal(2, NumFromAscii('2'));
    assert_int_equal(3, NumFromAscii('3'));
    assert_int_equal(4, NumFromAscii('4'));
    assert_int_equal(5, NumFromAscii('5'));
}

static void test_is_number()
{
    assert_true(is_number('0'));
    assert_true(is_number('4'));
    assert_true(is_number('9'));
    assert_false(is_number('a'));
    assert_false(is_number('c'));
}

static void test_is_upper()
{
    assert_true(is_upper('A'));
    assert_true(is_upper('C'));
    assert_true(is_upper('F'));
    assert_false(is_upper('a'));
    assert_false(is_upper('c'));
    assert_false(is_upper('f'));
}

static void test_is_lower()
{
    assert_true(is_lower('a'));
    assert_true(is_lower('c'));
    assert_true(is_lower('f'));
    assert_false(is_lower('A'));
    assert_false(is_lower('C'));
    assert_false(is_lower('F'));
}

static void test_is_alpha()
{
    assert_true(is_alpha('A'));
    assert_true(is_alpha('C'));
    assert_true(is_alpha('F'));
    assert_true(is_alpha('a'));
    assert_true(is_alpha('c'));
    assert_true(is_alpha('f'));
    assert_false(is_alpha('0'));
    assert_false(is_alpha('9'));
    assert_false(is_alpha(' '));
}

static void test_is_alpha_num()
{
    assert_true(is_alhpa_num('A'));
    assert_true(is_alhpa_num('C'));
    assert_true(is_alhpa_num('F'));
    assert_true(is_alhpa_num('a'));
    assert_true(is_alhpa_num('c'));
    assert_true(is_alhpa_num('f'));
    assert_true(is_alhpa_num('0'));
    assert_true(is_alhpa_num('9'));
    assert_false(is_alhpa_num(' '));
    assert_false(is_alhpa_num(','));
    assert_false(is_alhpa_num(':'));
}

static const struct CMUnitTest grp_char_checking[] = {
    cmocka_unit_test(test_IsNumber),
    cmocka_unit_test(test_IsUpper),
    cmocka_unit_test(test_IsLower),
    cmocka_unit_test(test_IsAlpha),
    cmocka_unit_test(test_IsAlphaNum),
    cmocka_unit_test(test_NumFromAscii),
    cmocka_unit_test(test_is_number),
    cmocka_unit_test(test_is_upper),
    cmocka_unit_test(test_is_lower),
    cmocka_unit_test(test_is_alpha),
    cmocka_unit_test(test_is_alpha_num),
};


/****************************************
 * Integer parsing
****************************************/

static void test_parse_u8() {
    String8 str1 = Str8Lit("123");
    String8 str2 = Str8Lit("+123");
    String8 str3 = Str8Lit("abc");
    String8 str4 = Str8Lit("-123");
    String8 str5 = Str8Lit("300");

    U8Result r_str1 = str8_parse_u8(str1); 
    U8Result r_str2 = str8_parse_u8(str2);
    U8Result r_str3 = str8_parse_u8(str3);
    U8Result r_str4 = str8_parse_u8(str4);
    U8Result r_str5 = str8_parse_u8(str5);
    
    assert_true(r_str1.ok);
    assert_true(r_str2.ok);
    assert_true(!r_str3.ok);
    assert_true(!r_str4.ok);
    assert_true(!r_str5.ok);
    
    assert_int_equal(123, r_str1.value);
    assert_int_equal(123, r_str2.value);
}

static void test_parse_u16() {
    String8 str1 = Str8Lit("500");
    String8 str2 = Str8Lit("+500");
    String8 str3 = Str8Lit("abc");
    String8 str4 = Str8Lit("-123");
    String8 str5 = Str8Lit("70000");

    U16Result r_str1 = str8_parse_u16(str1); 
    U16Result r_str2 = str8_parse_u16(str2);
    U16Result r_str3 = str8_parse_u16(str3);
    U16Result r_str4 = str8_parse_u16(str4);
    U16Result r_str5 = str8_parse_u16(str5);
    
    assert_true(r_str1.ok);
    assert_true(r_str2.ok);
    assert_true(!r_str3.ok);
    assert_true(!r_str4.ok);
    assert_true(!r_str5.ok);
    
    assert_int_equal(500, r_str1.value);
    assert_int_equal(500, r_str2.value);
}

static void test_parse_u32() {
    String8 str1 = Str8Lit("100000");
    String8 str2 = Str8Lit("+100000");
    String8 str3 = Str8Lit("abc");
    String8 str4 = Str8Lit("-123");
    String8 str5 = Str8Lit("4294967296");

    U32Result r_str1 = str8_parse_u32(str1); 
    U32Result r_str2 = str8_parse_u32(str2);
    U32Result r_str3 = str8_parse_u32(str3);
    U32Result r_str4 = str8_parse_u32(str4);
    U32Result r_str5 = str8_parse_u32(str5);
    
    assert_true(r_str1.ok);
    assert_true(r_str2.ok);
    assert_true(!r_str3.ok);
    assert_true(!r_str4.ok);
    assert_true(!r_str5.ok);
    
    assert_int_equal(100000, r_str1.value);
    assert_int_equal(100000, r_str2.value);
}

static void test_parse_u64() {
    String8 str1 = Str8Lit("5000000000");
    String8 str2 = Str8Lit("+5000000000");
    String8 str3 = Str8Lit("abc");
    String8 str4 = Str8Lit("-123");
    String8 str5 = Str8Lit("2000000000000000000000");

    U64Result r_str1 = str8_parse_u64(str1); 
    U64Result r_str2 = str8_parse_u64(str2);
    U64Result r_str3 = str8_parse_u64(str3);
    U64Result r_str4 = str8_parse_u64(str4);
    U64Result r_str5 = str8_parse_u64(str5);
    
    assert_true(r_str1.ok);
    assert_true(r_str2.ok);
    assert_true(!r_str3.ok);
    assert_true(!r_str4.ok);
    assert_true(!r_str5.ok);
    
    assert_int_equal(5000000000, r_str1.value);
    assert_int_equal(5000000000, r_str2.value);
}

static void test_parse_s8() {
    String8 str1 = Str8Lit("50");
    String8 str2 = Str8Lit("+50");
    String8 str3 = Str8Lit("-50");
    String8 str4 = Str8Lit("abc");
    String8 str5 = Str8Lit("300");
    String8 str6 = Str8Lit("-300");
    
    S8Result r_str1 = str8_parse_s8(str1);
    S8Result r_str2 = str8_parse_s8(str2);
    S8Result r_str3 = str8_parse_s8(str3);
    S8Result r_str4 = str8_parse_s8(str4);
    S8Result r_str5 = str8_parse_s8(str5);
    S8Result r_str6 = str8_parse_s8(str6);

    assert_true(r_str1.ok);
    assert_true(r_str2.ok);
    assert_true(r_str3.ok);
    assert_true(!r_str4.ok);
    assert_true(!r_str5.ok);
    assert_true(!r_str6.ok);

    assert_int_equal(50, r_str1.value);
    assert_int_equal(50, r_str2.value);
    assert_int_equal(-50, r_str3.value);
}

static void test_parse_s16() {
    String8 str1 = Str8Lit("10000");
    String8 str2 = Str8Lit("+10000");
    String8 str3 = Str8Lit("-10000");
    String8 str4 = Str8Lit("abc");
    String8 str5 = Str8Lit("40000");
    String8 str6 = Str8Lit("-40000");
    
    S16Result r_str1 = str8_parse_s16(str1);
    S16Result r_str2 = str8_parse_s16(str2);
    S16Result r_str3 = str8_parse_s16(str3);
    S16Result r_str4 = str8_parse_s16(str4);
    S16Result r_str5 = str8_parse_s16(str5);
    S16Result r_str6 = str8_parse_s16(str6);

    assert_true(r_str1.ok);
    assert_true(r_str2.ok);
    assert_true(r_str3.ok);
    assert_true(!r_str4.ok);
    assert_true(!r_str5.ok);
    assert_true(!r_str6.ok);

    assert_int_equal(10000, r_str1.value);
    assert_int_equal(10000, r_str2.value);
    assert_int_equal(-10000, r_str3.value);
}

static void test_parse_s32() {
    String8 str1 = Str8Lit("100000");
    String8 str2 = Str8Lit("+100000");
    String8 str3 = Str8Lit("-100000");
    String8 str4 = Str8Lit("abc");
    String8 str5 = Str8Lit("3000000000");
    String8 str6 = Str8Lit("-3000000000");
    
    S32Result r_str1 = str8_parse_s32(str1);
    S32Result r_str2 = str8_parse_s32(str2);
    S32Result r_str3 = str8_parse_s32(str3);
    S32Result r_str4 = str8_parse_s32(str4);
    S32Result r_str5 = str8_parse_s32(str5);
    S32Result r_str6 = str8_parse_s32(str6);

    assert_true(r_str1.ok);
    assert_true(r_str2.ok);
    assert_true(r_str3.ok);
    assert_true(!r_str4.ok);
    assert_true(!r_str5.ok);
    assert_true(!r_str6.ok);

    assert_int_equal(100000, r_str1.value);
    assert_int_equal(100000, r_str2.value);
    assert_int_equal(-100000, r_str3.value);
}

static void test_parse_s64() {
    String8 str1 = Str8Lit("2000000000");
    String8 str2 = Str8Lit("+2000000000");
    String8 str3 = Str8Lit("-2000000000");
    String8 str4 = Str8Lit("abc");
    String8 str5 = Str8Lit("9223372036854775808");
    String8 str6 = Str8Lit("-9223372036854775808");
    
    S64Result r_str1 = str8_parse_s64(str1);
    S64Result r_str2 = str8_parse_s64(str2);
    S64Result r_str3 = str8_parse_s64(str3);
    S64Result r_str4 = str8_parse_s64(str4);
    S64Result r_str5 = str8_parse_s64(str5);
    S64Result r_str6 = str8_parse_s64(str6);

    assert_true(r_str1.ok);
    assert_true(r_str2.ok);
    assert_true(r_str3.ok);
    assert_true(!r_str4.ok);
    assert_true(!r_str5.ok);
    assert_true(!r_str6.ok);

    assert_int_equal(2000000000, r_str1.value);
    assert_int_equal(2000000000, r_str2.value);
    assert_int_equal(-2000000000, r_str3.value);
}

static const struct CMUnitTest grp_int_parsing[] = {
    cmocka_unit_test(test_parse_u8),
    cmocka_unit_test(test_parse_u16),
    cmocka_unit_test(test_parse_u32),
    cmocka_unit_test(test_parse_u64),
    cmocka_unit_test(test_parse_s8),
    cmocka_unit_test(test_parse_s16),
    cmocka_unit_test(test_parse_s32),
    cmocka_unit_test(test_parse_s64),
};

int main(int argc, char **argv) {
    if(argc > 1) {
        switch (argv[1][0]) {
            case '0': 
                return cmocka_run_group_tests(grp_creation, NULL, NULL);
            case '1':
                return cmocka_run_group_tests(grp_manipulation, NULL, NULL);
            case '2':
                return cmocka_run_group_tests(grp_string_list, NULL, NULL);
            case '3':
                return cmocka_run_group_tests(grp_simple_functions, NULL, NULL);
            case '4':
                return cmocka_run_group_tests(grp_char_checking, NULL, NULL);
            case '5':
                return cmocka_run_group_tests(grp_int_parsing, NULL, NULL);
            default:
                printf("Missing group");
                exit(1);
        }
    } else {
        printf("Missing argument");
        exit(1);
    }
    return 0;
}
