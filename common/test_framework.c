/**
 * @file test_framework.c
 * @brief 统一测试框架实现
 * @details 提供测试管理和统计功能。
 *
 * @author Embedded C Learning Project
 * @date 2026-04-11
 * @version 1.0.0
 */

#include "test_framework.h"
#include <stdio.h>

/*============================================================================*/
/*                           私有变量                                           */
/*============================================================================*/

/** @brief 全局测试统计 */
static TestStats_t s_test_stats = {0, 0, 0};

/*============================================================================*/
/*                           测试框架函数实现                                   */
/*============================================================================*/

void test_init(void)
{
    s_test_stats.total = 0;
    s_test_stats.passed = 0;
    s_test_stats.failed = 0;
}

TestStats_t test_get_stats(void)
{
    return s_test_stats;
}

void test_pass(void)
{
    s_test_stats.total++;
    s_test_stats.passed++;
}

void test_fail(void)
{
    s_test_stats.total++;
    s_test_stats.failed++;
}

void test_print_report(void)
{
    printf("\n");
    printf("================================================================================\n");
    printf("                              测试报告\n");
    printf("================================================================================\n");
    printf("  总测试数: %d\n", s_test_stats.total);
    printf("  通过:     %d\n", s_test_stats.passed);
    printf("  失败:     %d\n", s_test_stats.failed);
    printf("  通过率:   %.1f%%\n",
           s_test_stats.total > 0 ?
           (float)s_test_stats.passed * 100.0f / (float)s_test_stats.total : 0.0f);
    printf("================================================================================\n");
}

int32_t run_all_tests(void)
{
    int32_t total_failed = 0;

    test_init();

    printf("\n================================================================================\n");
    printf("                        开始运行所有模块测试\n");
    printf("================================================================================\n");

    /* 运行各模块测试 */
    printf("\n[01/11] 基础语法模块测试...\n");
    total_failed += test_basic_syntax();

    printf("\n[02/11] 控制流模块测试...\n");
    total_failed += test_control_flow();

    printf("\n[03/11] 函数与作用域模块测试...\n");
    total_failed += test_function_scope();

    printf("\n[04/11] 数组与字符串模块测试...\n");
    total_failed += test_array_string();

    printf("\n[05/11] 指针与内存模块测试...\n");
    total_failed += test_pointer_memory();

    printf("\n[06/11] 结构体与联合体模块测试...\n");
    total_failed += test_struct_union();

    printf("\n[07/11] 预处理指令模块测试...\n");
    total_failed += test_preprocessor();

    printf("\n[08/11] 文件I/O模块测试...\n");
    total_failed += test_file_io();

    printf("\n[09/11] 位运算模块测试...\n");
    total_failed += test_bit_operation();

    printf("\n[10/11] 错误处理与调试模块测试...\n");
    total_failed += test_error_debug();

    printf("\n[11/11] 数据结构模块测试...\n");
    total_failed += test_data_structure();

    test_print_report();

    return total_failed;
}
