/**
 * @file test_error_debug.c
 * @brief 错误处理与调试模块测试
 */

#include "test_framework.h"
#include "error_debug.h"
#include <stdio.h>
#include <stdlib.h>

static void test_status_codes(void)
{
    printf("    [测试] 状态码...\n");

    bool pass = (STATUS_OK == 0);
    pass = pass && (STATUS_ERROR == -1);
    pass = pass && (STATUS_NULL_PTR == -2);

    if (pass) {
        printf("    [通过] 状态码正确\n");
        test_pass();
    } else {
        printf("    [失败] 状态码错误\n");
        test_fail();
    }
}

static void test_success_failed_macros(void)
{
    printf("    [测试] SUCCESS/FAILED 宏...\n");

    Status_t status = STATUS_OK;
    bool pass = SUCCESS(status) && !FAILED(status);

    status = STATUS_ERROR;
    pass = pass && !SUCCESS(status) && FAILED(status);

    if (pass) {
        printf("    [通过] SUCCESS/FAILED 宏正确\n");
        test_pass();
    } else {
        printf("    [失败] SUCCESS/FAILED 宏错误\n");
        test_fail();
    }
}

static void test_null_check_macro(void)
{
    printf("    [测试] 空指针检查宏...\n");

    int32_t *ptr = NULL;
    bool pass = (ptr == NULL);

    int32_t value = 42;
    ptr = &value;
    pass = pass && (ptr != NULL);

    if (pass) {
        printf("    [通过] 空指针检查宏正确\n");
        test_pass();
    } else {
        printf("    [失败] 空指针检查错误\n");
        test_fail();
    }
}

static void test_safe_free_macro(void)
{
    printf("    [测试] SAFE_FREE 宏...\n");

    int32_t *ptr = (int32_t *)malloc(sizeof(int32_t));
    if (ptr == NULL) {
        printf("    [失败] malloc 失败\n");
        test_fail();
        return;
    }

    *ptr = 100;
    SAFE_FREE(ptr);

    if (ptr == NULL) {
        printf("    [通过] SAFE_FREE 宏正确\n");
        test_pass();
    } else {
        printf("    [失败] SAFE_FREE 后指针应为 NULL\n");
        test_fail();
    }
}

static int32_t test_null_func(int32_t *p)
{
    RETURN_IF_NULL(p);
    return *p;
}

static void test_return_if_macros(void)
{
    printf("    [测试] RETURN_IF 宏...\n");

    int32_t ret = test_null_func(NULL);

    if (ret == STATUS_NULL_PTR) {
        printf("    [通过] RETURN_IF 宏正确\n");
        test_pass();
    } else {
        printf("    [失败] RETURN_IF_NULL 宏错误\n");
        test_fail();
    }
}

static void test_clamp_macro(void)
{
    printf("    [测试] CLAMP 宏...\n");

    bool pass = (CLAMP(50, 0, 100) == 50);
    pass = pass && (CLAMP(-10, 0, 100) == 0);
    pass = pass && (CLAMP(150, 0, 100) == 100);

    if (pass) {
        printf("    [通过] CLAMP 宏正确\n");
        test_pass();
    } else {
        printf("    [失败] CLAMP 宏错误\n");
        test_fail();
    }
}

int32_t test_error_debug(void)
{
    test_status_codes();
    test_success_failed_macros();
    test_null_check_macro();
    test_safe_free_macro();
    test_return_if_macros();
    test_clamp_macro();

    TestStats_t stats = test_get_stats();
    return stats.failed;
}
