/* make cmocka work for c89 */
#include <stdio.h>
#include <stdlib.h>
#define inline

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "base/strings.h"

static void test_strings_parse_u8() {
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

static void test_strings_parse_u16() {
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

static void test_strings_parse_u32() {
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

static void test_strings_parse_u64() {
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

static void test_strings_parse_s8() {
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

static void test_strings_parse_s16() {
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

static void test_strings_parse_s32() {
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

static void test_strings_parse_s64() {
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

static const struct CMUnitTest grp1[] = {
    cmocka_unit_test(test_strings_parse_u8),
    cmocka_unit_test(test_strings_parse_u16),
    cmocka_unit_test(test_strings_parse_u32),
    cmocka_unit_test(test_strings_parse_u64),

    cmocka_unit_test(test_strings_parse_s8),
    cmocka_unit_test(test_strings_parse_s16),
    cmocka_unit_test(test_strings_parse_s32),
    cmocka_unit_test(test_strings_parse_s64),
};

static const struct CMUnitTest grp2[] = {

};

int main(int argc, char **argv) {
    if(argc > 1) {
        switch (argv[1][0]) {
            case '0': 
                return cmocka_run_group_tests(grp1, NULL, NULL);
                break;
            case '1':
                return cmocka_run_group_tests(grp2, NULL, NULL);
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
