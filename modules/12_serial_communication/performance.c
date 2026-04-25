/**
 * @file performance.c
 * @brief 性能测试模块实现文件
 * @details 实现对各模块进行性能测试的功能，包括时间测量、内存使用分析等。
 *          该模块帮助学习者了解不同数据结构和算法的性能特性。
 *
 * @section performance_implementation 实现细节
 * - 使用高精度计时器（QueryPerformanceCounter）
 * - 提供自动化的性能测试流程
 * - 支持自定义测试用例
 * - 生成详细的性能报告
 *
 * @author Embedded C Learning Project
 * @date 2026-04-21
 * @version 1.0.0
 */

#include "performance.h"
#include "common/common_types.h"
#include "common/common_utils.h"
#include "../11_data_structure/data_structure.h"
#include "../11_data_structure/test_perf_helpers.h"
#include "../04_array_string/array_string.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

/*============================================================================*/
/*                           私有变量定义                                       */
/*============================================================================*/

/** @brief 性能测试结果数组 */
static PerformanceResult_t performance_results[50];
static uint32_t result_count = 0;

/** @brief 模块依赖信息数组 */
static const ModuleDependency_t module_dependencies[] = {
    {"01_basic_syntax", "", "基础语法模块"},
    {"02_control_flow", "01_basic_syntax", "控制流模块"},
    {"03_function_scope", "01_basic_syntax,02_control_flow", "函数与作用域模块"},
    {"04_array_string", "01_basic_syntax,02_control_flow,03_function_scope", "数组与字符串模块"},
    {"05_pointer_memory", "01_basic_syntax,02_control_flow,03_function_scope,04_array_string", "指针与内存模块"},
    {"06_struct_union", "01_basic_syntax,02_control_flow,03_function_scope,05_pointer_memory", "结构体与联合体"},
    {"07_preprocessor", "01_basic_syntax", "预处理指令模块"},
    {"08_file_io", "01_basic_syntax,07_preprocessor", "文件I/O操作模块"},
    {"09_bit_operation", "01_basic_syntax,06_struct_union", "位运算与嵌入式"},
    {"10_error_debug", "01_basic_syntax,03_function_scope", "错误处理与调试"},
    {"11_data_structure", "01_basic_syntax,02_control_flow,03_function_scope,04_array_string,05_pointer_memory", "数据结构模块"},
    {"12_serial_communication", "01_basic_syntax", "串口通信模块"}
};

/*============================================================================*/
/*                           私有函数声明                                       */
/*============================================================================*/

/**
 * @brief 初始化测试数据
 * @param size 数据规模
 * @return 分配的内存指针
 */
static int32_t* init_test_data(size_t size);

/**
 * @brief 释放测试数据
 * @param data 测试数据指针
 */
static void free_test_data(int32_t *data);

/*============================================================================*/
/*                           性能测试函数实现                                   */
/*============================================================================*/

Status_t performance_init(void)
{
    result_count = 0;
    memset(performance_results, 0, sizeof(performance_results));
    return STATUS_OK;
}

void test_array_performance(void)
{
    print_separator("数组操作性能测试");

    const size_t sizes[] = {100, 1000, 10000, 50000};
    const size_t size_count = sizeof(sizes) / sizeof(sizes[0]);

    printf("测试不同规模的数组性能...\n\n");

    for (size_t i = 0; i < size_count; i++) {
        size_t size = sizes[i];
        PerformanceResult_t result;

        // 初始化数据
        int32_t *data = init_test_data(size);
        if (!data) {
            printf("错误：无法分配内存用于测试\n");
            return;
        }

        // 测试数组创建时间
        result.test_name = "数组创建";
        result.iterations = 10;
        result.execution_time_us = measure_function_time(
            (void (*)(void))test_array_create, result.iterations);
        result.memory_used = size * sizeof(int32_t);
        result.passed = true;

        printf("数组大小: %llu\n", (unsigned long long)size);
        printf("  创建时间: %.2f μs\n", (double)result.execution_time_us / result.iterations);
        printf("  内存使用: %llu bytes\n", (unsigned long long)result.memory_used);

        // 测试数组查找时间（二分查找）
        result.test_name = "数组查找(二分)";
        result.iterations = 1000;
        result.execution_time_us = measure_function_time(
            (void (*)(void))test_array_binary_search, result.iterations);
        result.passed = true;

        printf("  查找时间: %.2f μs\n", (double)result.execution_time_us / result.iterations);

        // 测试数组遍历时间
        result.test_name = "数组遍历";
        result.iterations = 100;
        result.execution_time_us = measure_function_time(
            (void (*)(void))test_array_traverse, result.iterations);
        result.passed = true;

        printf("  遍历时间: %.2f μs\n", (double)result.execution_time_us / result.iterations);

        printf("  --------------------\n");

        free_test_data(data);
    }
}

void test_stack_performance(void)
{
    print_separator("栈操作性能测试");

    const size_t sizes[] = {100, 1000, 10000, 50000};
    const size_t size_count = sizeof(sizes) / sizeof(sizes[0]);

    printf("测试不同规模的栈操作性能...\n\n");

    for (size_t i = 0; i < size_count; i++) {
        size_t size = sizes[i];
        PerformanceResult_t result;

        // 测试栈创建时间
        result.test_name = "栈创建";
        result.iterations = 10;
        result.execution_time_us = measure_function_time(
            (void (*)(void))test_stack_create, result.iterations);
        result.memory_used = size * sizeof(int32_t);
        result.passed = true;

        printf("栈大小: %llu\n", (unsigned long long)size);
        printf("  创建时间: %.2f μs\n", (double)result.execution_time_us / result.iterations);
        printf("  内存使用: %llu bytes\n", (unsigned long long)result.memory_used);

        // 测试栈push操作时间
        result.test_name = "栈push操作";
        result.iterations = 1000;
        result.execution_time_us = measure_function_time(
            (void (*)(void))test_stack_push, result.iterations);
        result.passed = true;

        printf("  push时间: %.2f μs\n", (double)result.execution_time_us / result.iterations);

        // 测试栈pop操作时间
        result.test_name = "栈pop操作";
        result.iterations = 1000;
        result.execution_time_us = measure_function_time(
            (void (*)(void))test_stack_pop, result.iterations);
        result.passed = true;

        printf("  pop时间: %.2f μs\n", (double)result.execution_time_us / result.iterations);

        printf("  --------------------\n");
    }
}

void test_queue_performance(void)
{
    print_separator("队列操作性能测试");

    const size_t sizes[] = {100, 1000, 10000, 50000};
    const size_t size_count = sizeof(sizes) / sizeof(sizes[0]);

    printf("测试不同规模的队列操作性能...\n\n");

    for (size_t i = 0; i < size_count; i++) {
        size_t size = sizes[i];
        PerformanceResult_t result;

        // 测试队列创建时间
        result.test_name = "队列创建";
        result.iterations = 10;
        result.execution_time_us = measure_function_time(
            (void (*)(void))test_queue_create, result.iterations);
        result.memory_used = size * sizeof(int32_t);
        result.passed = true;

        printf("队列大小: %llu\n", (unsigned long long)size);
        printf("  创建时间: %.2f μs\n", (double)result.execution_time_us / result.iterations);
        printf("  内存使用: %llu bytes\n", (unsigned long long)result.memory_used);

        // 测试队列入队操作时间
        result.test_name = "队列入队(enqueue)";
        result.iterations = 1000;
        result.execution_time_us = measure_function_time(
            (void (*)(void))test_queue_enqueue, result.iterations);
        result.passed = true;

        printf("  enqueue时间: %.2f μs\n", (double)result.execution_time_us / result.iterations);

        // 测试队列出队操作时间
        result.test_name = "队列出队(dequeue)";
        result.iterations = 1000;
        result.execution_time_us = measure_function_time(
            (void (*)(void))test_queue_dequeue, result.iterations);
        result.passed = true;

        printf("  dequeue时间: %.2f μs\n", (double)result.execution_time_us / result.iterations);

        printf("  --------------------\n");
    }
}

void test_linked_list_performance(void)
{
    print_separator("链表操作性能测试");

    const size_t sizes[] = {100, 1000, 10000, 50000};
    const size_t size_count = sizeof(sizes) / sizeof(sizes[0]);

    printf("测试不同规模的链表操作性能...\n\n");

    for (size_t i = 0; i < size_count; i++) {
        size_t size = sizes[i];
        PerformanceResult_t result;

        // 测试链表创建时间
        result.test_name = "链表创建";
        result.iterations = 10;
        result.execution_time_us = measure_function_time(
            (void (*)(void))test_linked_list_create, result.iterations);
        result.memory_used = size * sizeof(int32_t) + size * sizeof(ListNode_t);
        result.passed = true;

        printf("链表大小: %llu\n", (unsigned long long)size);
        printf("  创建时间: %.2f μs\n", (double)result.execution_time_us / result.iterations);
        printf("  内存使用: %llu bytes (包括节点开销)\n", (unsigned long long)result.memory_used);

        // 测试链表插入时间
        result.test_name = "链表插入操作";
        result.iterations = 100;
        result.execution_time_us = measure_function_time(
            (void (*)(void))test_linked_list_insert, result.iterations);
        result.passed = true;

        printf("  插入时间: %.2f μs\n", (double)result.execution_time_us / result.iterations);

        // 测试链表删除时间
        result.test_name = "链表删除操作";
        result.iterations = 100;
        result.execution_time_us = measure_function_time(
            (void (*)(void))test_linked_list_delete, result.iterations);
        result.passed = true;

        printf("  删除时间: %.2f μs\n", (double)result.execution_time_us / result.iterations);

        // 测试链表查找时间
        result.test_name = "链表查找操作";
        result.iterations = 100;
        result.execution_time_us = measure_function_time(
            (void (*)(void))test_linked_list_search, result.iterations);
        result.passed = true;

        printf("  查找时间: %.2f μs\n", (double)result.execution_time_us / result.iterations);

        printf("  --------------------\n");
    }
}

void run_all_performance_tests(void)
{
    print_separator("运行所有数据结构性能测试");

    performance_init();

    test_array_performance();
    printf("\n");

    test_stack_performance();
    printf("\n");

    test_queue_performance();
    printf("\n");

    test_linked_list_performance();

    printf("\n性能测试完成！\n");
}

void print_module_dependencies(void)
{
    print_separator("模块依赖关系");

    printf("%-20s %-50s\n", "模块名称", "依赖模块");
    printf("------------------------------------------------------------\n");

    for (size_t i = 0; i < sizeof(module_dependencies) / sizeof(module_dependencies[0]); i++) {
        const ModuleDependency_t *dep = &module_dependencies[i];
        printf("%-20s %-50s\n", dep->module_name, dep->dependencies);
    }

    printf("\n依赖说明：\n");
    printf("- 空表示无依赖\n");
    printf("- 逗号分隔表示多个依赖项\n");
    printf("- 必须先安装依赖的模块\n");
}

bool check_module_dependencies(const char *module_name)
{
    for (size_t i = 0; i < sizeof(module_dependencies) / sizeof(module_dependencies[0]); i++) {
        const ModuleDependency_t *dep = &module_dependencies[i];
        if (strcmp(dep->module_name, module_name) == 0) {
            if (strlen(dep->dependencies) == 0) {
                return true; // 无依赖
            }

            // 检查依赖是否满足
            char *deps = strdup(dep->dependencies);
            char *token = strtok(deps, ",");
            bool all_deps_satisfied = true;

            while (token != NULL) {
                bool dep_found = false;
                for (size_t j = 0; j < sizeof(module_dependencies) / sizeof(module_dependencies[0]); j++) {
                    if (strcmp(module_dependencies[j].module_name, token) == 0) {
                        dep_found = true;
                        break;
                    }
                }

                if (!dep_found) {
                    all_deps_satisfied = false;
                    printf("错误：模块 %s 依赖的模块 %s 未找到\n", module_name, token);
                    break;
                }

                token = strtok(NULL, ",");
            }

            free(deps);
            return all_deps_satisfied;
        }
    }

    printf("错误：未找到模块 %s\n", module_name);
    return false;
}

void compare_performance(const char *name1, uint64_t time1,
                        const char *name2, uint64_t time2)
{
    double ratio = (double)time1 / time2;

    printf("\n性能比较：\n");
    printf("  %s: %llu μs\n", name1, (unsigned long long)time1);
    printf("  %s: %llu μs\n", name2, (unsigned long long)time2);

    if (ratio < 1.0) {
        printf("  %s 比 %s 快 %.2fx\n", name1, name2, 1.0 / ratio);
    } else if (ratio > 1.0) {
        printf("  %s 比 %s 慢 %.2fx\n", name2, name1, ratio);
    } else {
        printf("  两种实现性能相同\n");
    }
}

uint64_t measure_function_time(void (*func)(void), uint32_t iterations)
{
    LARGE_INTEGER frequency;
    LARGE_INTEGER start, end;
    uint64_t total_time = 0;

    // 获取高精度计时器频率
    QueryPerformanceFrequency(&frequency);

    for (uint32_t i = 0; i < iterations; i++) {
        QueryPerformanceCounter(&start);
        func();
        QueryPerformanceCounter(&end);

        total_time += (end.QuadPart - start.QuadPart) * 1000000 / frequency.QuadPart;
    }

    return total_time;
}

/*============================================================================*/
/*                           私有函数实现                                       */
/*============================================================================*/

static int32_t* init_test_data(size_t size)
{
    int32_t *data = (int32_t*)malloc(size * sizeof(int32_t));
    if (!data) return NULL;

    for (size_t i = 0; i < size; i++) {
        data[i] = (int32_t)(i % 1000);
    }

    return data;
}

static void free_test_data(int32_t *data)
{
    if (data) {
        free(data);
    }
}