/**
 * @file test_preprocessor.c
 * @brief 预处理指令模块测试
 */

#include "test_framework.h"
#include "preprocessor.h"
#include <stdio.h>

#define TEST_ADD(a, b) ((a) + (b))
#define TEST_SQUARE(x) ((x) * (x))
#define TEST_MAX(a, b) (((a) > (b)) ? (a) : (b))
#define TEST_STRINGIFY(x) #x
#define TEST_CONCAT(a, b) a##b

static void test_simple_macros(void)
{
    printf("    [测试] 简单宏定义...\n");

    bool pass = (TEST_ADD(3, 4) == 7);
    pass = pass && (TEST_SQUARE(5) == 25);
    pass = pass && (TEST_MAX(10, 20) == 20);

    if (pass) {
        printf("    [通过] 简单宏定义正确\n");
        test_pass();
    } else {
        printf("    [失败] 简单宏定义错误\n");
        test_fail();
    }
}

static void test_stringify_concat(void)
{
    printf("    [测试] 字符串化和连接宏...\n");

    const char *str = TEST_STRINGIFY(hello);
    int32_t TEST_CONCAT(var, 1) = 42;

    bool pass = (str[0] == 'h') && (var1 == 42);

    if (pass) {
        printf("    [通过] 字符串化和连接宏正确\n");
        test_pass();
    } else {
        printf("    [失败] 字符串化或连接宏错误\n");
        test_fail();
    }
}

static void test_conditional_compilation(void)
{
    printf("    [测试] 条件编译...\n");

#ifndef TEST_FEATURE_ENABLED
    int32_t value = 0;
#else
    int32_t value = 100;
#endif

    if (value == 0) {
        printf("    [通过] 条件编译正确\n");
        test_pass();
    } else {
        printf("    [失败] 条件编译错误\n");
        test_fail();
    }
}

static void test_predefined_macros(void)
{
    printf("    [测试] 预定义宏...\n");

    bool pass = (__LINE__ > 0);
    pass = pass && (__FILE__[0] != '\0');
    pass = pass && (__DATE__[0] >= 'A' && __DATE__[0] <= 'Z');

    if (pass) {
        printf("    [通过] 预定义宏正确\n");
        test_pass();
    } else {
        printf("    [失败] 预定义宏错误\n");
        test_fail();
    }
}

static void test_macro_side_effects(void)
{
    printf("    [测试] 宏副作用陷阱...\n");

    int32_t x = 5;
    int32_t result = MIN(x++, 10);

    if (x >= 6) {
        printf("    [通过] 宏副作用测试完成\n");
        test_pass();
    } else {
        printf("    [失败] 宏副作用处理错误\n");
        test_fail();
    }
}

int32_t test_preprocessor(void)
{
    test_simple_macros();
    test_stringify_concat();
    test_conditional_compilation();
    test_predefined_macros();
    test_macro_side_effects();

    TestStats_t stats = test_get_stats();
    return stats.failed;
}
