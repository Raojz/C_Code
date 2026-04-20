/**
 * @file test_array_string.c
 * @brief 数组与字符串模块测试
 */

#include "test_framework.h"
#include "array_string.h"
#include <stdio.h>
#include <string.h>

static void test_array_basics(void)
{
    printf("    [测试] 数组基础...\n");

    int32_t arr[5] = {1, 2, 3, 4, 5};
    bool pass = (arr[0] == 1) && (arr[4] == 5);

    if (pass) {
        printf("    [通过] 数组基础正确\n");
        test_pass();
    } else {
        printf("    [失败] 数组索引访问错误\n");
        test_fail();
    }
}

static void test_array_sum(void)
{
    printf("    [测试] 数组求和...\n");

    int32_t arr[5] = {1, 2, 3, 4, 5}, sum = 0;
    for (int32_t i = 0; i < 5; i++) sum += arr[i];

    if (sum == 15) {
        printf("    [通过] 数组求和正确\n");
        test_pass();
    } else {
        printf("    [失败] 数组求和错误\n");
        test_fail();
    }
}

static void test_string_basics(void)
{
    printf("    [测试] 字符串基础...\n");

    const char *str = "Hello";
    char buffer[10] = "Test";
    bool pass = (strlen(str) == 5) && (buffer[0] == 'T');

    if (pass) {
        printf("    [通过] 字符串基础正确\n");
        test_pass();
    } else {
        printf("    [失败] 字符串基础错误\n");
        test_fail();
    }
}

static void test_string_copy(void)
{
    printf("    [测试] 字符串拷贝...\n");

    char dest[20];
    const char *src = "Hello World";
    strcpy(dest, src);

    if (strcmp(dest, src) == 0) {
        printf("    [通过] 字符串拷贝正确\n");
        test_pass();
    } else {
        printf("    [失败] 字符串拷贝错误\n");
        test_fail();
    }
}

static void test_string_concat(void)
{
    printf("    [测试] 字符串连接...\n");

    char buffer[30] = "Hello";
    strcat(buffer, " ");
    strcat(buffer, "World");

    if (strcmp(buffer, "Hello World") == 0) {
        printf("    [通过] 字符串连接正确\n");
        test_pass();
    } else {
        printf("    [失败] 字符串连接错误\n");
        test_fail();
    }
}

static void test_2d_array(void)
{
    printf("    [测试] 二维数组...\n");

    int32_t matrix[2][3] = {{1, 2, 3}, {4, 5, 6}};
    bool pass = (matrix[0][0] == 1) && (matrix[1][2] == 6);

    if (pass) {
        printf("    [通过] 二维数组正确\n");
        test_pass();
    } else {
        printf("    [失败] 二维数组访问错误\n");
        test_fail();
    }
}

int32_t test_array_string(void)
{
    test_array_basics();
    test_array_sum();
    test_string_basics();
    test_string_copy();
    test_string_concat();
    test_2d_array();

    TestStats_t stats = test_get_stats();
    return stats.failed;
}
