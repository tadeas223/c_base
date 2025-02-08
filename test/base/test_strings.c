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
    
    printf("1 %b\n", r_str1.ok);
    printf("2 %b\n", r_str2.ok);
    printf("3 %b\n", r_str3.ok);
    printf("4 %b\n", r_str4.ok);
    printf("5 %b\n", r_str5.ok);
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
    
    printf("1 %b\n", r_str1.ok);
    printf("2 %b\n", r_str2.ok);
    printf("3 %b\n", r_str3.ok);
    printf("4 %b\n", r_str4.ok);
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
    
    printf("1 %b\n", r_str1.ok);
    printf("2 %b\n", r_str2.ok);
    printf("3 %b\n", r_str3.ok);
    printf("4 %b\n", r_str4.ok);
    printf("5 %b\n", r_str5.ok);
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

static const struct CMUnitTest grp1[] = {
    cmocka_unit_test(test_strings_parse_u8),
    cmocka_unit_test(test_strings_parse_u16),
    cmocka_unit_test(test_strings_parse_u32),
    cmocka_unit_test(test_strings_parse_u64),
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
