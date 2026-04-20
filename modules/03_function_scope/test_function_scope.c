/**
 * @file test_function_scope.c
 * @brief 函数与作用域模块测试
 */

#include "test_framework.h"
#include "function_scope.h"
#include <stdio.h>

static int32_t test_add(int32_t a, int32_t b) { return a + b; }

static int32_t test_counter(void)
{
    static int32_t count = 0;
    count++;
    return count;
}

static void test_function_basics(void)
{
    printf("    [测试] 函数基础...\n");

    if (test_add(3, 4) == 7) {
        printf("    [通过] 函数基础正确\n");
        test_pass();
    } else {
        printf("    [失败] 函数返回值错误\n");
        test_fail();
    }
}

static void test_static_variable(void)
{
    printf("    [测试] 静态局部变量...\n");

    bool pass = (test_counter() == 1);
    pass = pass && (test_counter() == 2);
    pass = pass && (test_counter() == 3);

    if (pass) {
        printf("    [通过] 静态局部变量正确\n");
        test_pass();
    } else {
        printf("    [失败] 静态局部变量错误\n");
        test_fail();
    }
}

static int32_t test_factorial(int32_t n)
{
    if (n <= 1) {
        return 1;
    }
    return n * test_factorial(n - 1);
}

static void test_recursion(void)
{
    printf("    [测试] 递归函数...\n");

    if (test_factorial(5) == 120) {
        printf("    [通过] 递归函数正确\n");
        test_pass();
    } else {
        printf("    [失败] 递归计算错误\n");
        test_fail();
    }
}

static void test_function_pointer(void)
{
    printf("    [测试] 函数指针...\n");

    int32_t (*op)(int32_t, int32_t) = test_add;

    if (op(10, 20) == 30) {
        printf("    [通过] 函数指针正确\n");
        test_pass();
    } else {
        printf("    [失败] 函数指针调用错误\n");
        test_fail();
    }
}

int32_t test_function_scope(void)
{
    test_function_basics();
    test_static_variable();
    test_recursion();
    test_function_pointer();

    TestStats_t stats = test_get_stats();
    return stats.failed;
}
