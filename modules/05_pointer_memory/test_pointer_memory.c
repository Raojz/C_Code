/**
 * @file test_pointer_memory.c
 * @brief 指针与内存模块测试
 */

#include "test_framework.h"
#include "pointer_memory.h"
#include <stdio.h>
#include <stdlib.h>

static void test_pointer_basics(void)
{
    printf("    [测试] 指针基础...\n");

    int32_t value = 42;
    int32_t *ptr = &value;
    bool pass = (*ptr == 42) && (ptr != NULL);

    if (pass) {
        printf("    [通过] 指针基础正确\n");
        test_pass();
    } else {
        printf("    [失败] 指针解引用错误\n");
        test_fail();
    }
}

static void test_pointer_arithmetic(void)
{
    printf("    [测试] 指针运算...\n");

    int32_t arr[5] = {10, 20, 30, 40, 50};
    int32_t *ptr = arr;

    bool pass = (*(ptr + 2) == 30);
    ptr += 4;
    pass = pass && (*ptr == 50);

    if (pass) {
        printf("    [通过] 指针运算正确\n");
        test_pass();
    } else {
        printf("    [失败] 指针运算错误\n");
        test_fail();
    }
}

static void test_swap_function(void)
{
    printf("    [测试] 交换函数...\n");

    int32_t a = 10, b = 20;
    swap_values(&a, &b);

    if (a == 20 && b == 10) {
        printf("    [通过] 交换函数正确\n");
        test_pass();
    } else {
        printf("    [失败] 交换函数错误\n");
        test_fail();
    }
}

static void test_find_max(void)
{
    printf("    [测试] 查找最大值...\n");

    int32_t arr[5] = {10, 50, 30, 40, 20};
    int32_t *max_ptr = find_max(arr, 5);

    if (max_ptr != NULL && *max_ptr == 50) {
        printf("    [通过] 查找最大值正确\n");
        test_pass();
    } else {
        printf("    [失败] 查找最大值错误\n");
        test_fail();
    }
}

static void test_dynamic_memory(void)
{
    printf("    [测试] 动态内存分配...\n");

    int32_t *arr = (int32_t *)malloc(5 * sizeof(int32_t));
    if (arr == NULL) {
        printf("    [失败] malloc 返回 NULL\n");
        test_fail();
        return;
    }

    for (int32_t i = 0; i < 5; i++) arr[i] = i * 10;

    bool pass = (arr[4] == 40);
    free(arr);

    if (pass) {
        printf("    [通过] 动态内存分配正确\n");
        test_pass();
    } else {
        printf("    [失败] 动态数组写入错误\n");
        test_fail();
    }
}

static void test_function_pointer_array(void)
{
    printf("    [测试] 函数指针数组...\n");

    OperationFunc_t ops[4] = {add_op, sub_op, mul_op, div_op};

    bool pass = (ops[0](10, 5) == 15);
    pass = pass && (ops[1](10, 5) == 5);
    pass = pass && (ops[2](10, 5) == 50);
    pass = pass && (ops[3](10, 5) == 2);

    if (pass) {
        printf("    [通过] 函数指针数组正确\n");
        test_pass();
    } else {
        printf("    [失败] 函数指针数组错误\n");
        test_fail();
    }
}

int32_t test_pointer_memory(void)
{
    test_pointer_basics();
    test_pointer_arithmetic();
    test_swap_function();
    test_find_max();
    test_dynamic_memory();
    test_function_pointer_array();

    TestStats_t stats = test_get_stats();
    return stats.failed;
}
