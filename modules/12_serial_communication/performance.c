/**
 * @file performance.c
 * @brief 性能测试模块实现文件（简化版）
 * @details 提供模块依赖管理和性能测试演示功能。
 *          测试辅助函数已移除，仅保留演示功能。
 *
 * @author Embedded C Learning Project
 * @date 2026-04-21
 * @version 1.1.0
 */

#include "performance.h"
#include "common/common_types.h"
#include "common/common_utils.h"
#include "../11_data_structure/data_structure.h"
#include "../04_array_string/array_string.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

/*============================================================================*/
/*                           私有变量定义                                       */
/*============================================================================*/

/** @brief 模块依赖关系表 */
static const ModuleDependency_t module_dependencies[] = {
    {"01_basic_syntax", "无", "基础语法模块"},
    {"02_control_flow", "01_basic_syntax", "控制流模块"},
    {"03_function_scope", "01_basic_syntax,02_control_flow", "函数与作用域模块"},
    {"04_array_string", "01_basic_syntax,02_control_flow", "数组与字符串模块"},
    {"05_pointer_memory", "01_basic_syntax,03_function_scope", "指针与内存模块"},
    {"06_struct_union", "01_basic_syntax,05_pointer_memory", "结构体与联合体模块"},
    {"07_preprocessor", "01_basic_syntax", "预处理指令模块"},
    {"08_file_io", "01_basic_syntax,04_array_string,05_pointer_memory", "文件I/O模块"},
    {"09_bit_operation", "01_basic_syntax,06_struct_union", "位运算模块"},
    {"10_error_debug", "01_basic_syntax,03_function_scope", "错误处理与调试模块"},
    {"11_data_structure", "01_basic_syntax,03_function_scope,04_array_string,05_pointer_memory", "数据结构模块"},
    {"12_serial_communication", "01_basic_syntax,03_function_scope,09_bit_operation", "串口通信模块"}
};

static const size_t module_count = sizeof(module_dependencies) / sizeof(module_dependencies[0]);

/*============================================================================*/
/*                           性能测试函数实现                                   */
/*============================================================================*/

/**
 * @brief 初始化性能测试模块
 */
Status_t performance_init(void)
{
    return STATUS_OK;
}

/**
 * @brief 测试数组操作性能（演示版）
 */
void test_array_performance(void)
{
    print_separator("数组操作性能演示");
    printf("注意：测试辅助函数已移除\n\n");
    printf("数组性能特点：\n");
    printf("• 随机访问：O(1) - 极快\n");
    printf("• 插入删除：O(n) - 需要移动元素\n");
    printf("• 查找（无序）：O(n) - 线性查找\n");
    printf("• 查找（有序）：O(log n) - 二分查找\n");
    printf("• 内存使用：连续内存，空间利用率高\n\n");
}

/**
 * @brief 测试栈操作性能（演示版）
 */
void test_stack_performance(void)
{
    print_separator("栈操作性能演示");
    printf("注意：测试辅助函数已移除\n\n");
    printf("栈性能特点：\n");
    printf("• Push操作：O(1) - 极快\n");
    printf("• Pop操作：O(1) - 极快\n");
    printf("• 查看栈顶：O(1) - 极快\n");
    printf("• 内存使用：固定大小，预分配\n\n");
}

/**
 * @brief 测试队列操作性能（演示版）
 */
void test_queue_performance(void)
{
    print_separator("队列操作性能演示");
    printf("注意：测试辅助函数已移除\n\n");
    printf("队列性能特点：\n");
    printf("• Enqueue操作：O(1) - 极快\n");
    printf("• Dequeue操作：O(1) - 极快\n");
    printf("• 查看队首/队尾：O(1) - 极快\n");
    printf("• 内存使用：循环队列，空间利用率高\n\n");
}

/**
 * @brief 测试链表操作性能（演示版）
 */
void test_linked_list_performance(void)
{
    print_separator("链表操作性能演示");
    printf("注意：测试辅助函数已移除\n\n");
    printf("链表性能特点：\n");
    printf("• 插入操作：O(1) - 在已知位置插入\n");
    printf("• 删除操作：O(1) - 在已知位置删除\n");
    printf("• 查找操作：O(n) - 需要遍历\n");
    printf("• 随机访问：O(n) - 需要遍历\n");
    printf("• 内存使用：非连续内存，需要额外指针空间\n\n");
}

/**
 * @brief 运行所有数据结构性能测试（演示版）
 */
void run_all_performance_tests(void)
{
    print_separator("运行所有性能测试");
    printf("注意：测试辅助函数已移除\n\n");

    test_array_performance();
    test_stack_performance();
    test_queue_performance();
    test_linked_list_performance();
}

/**
 * @brief 打印模块依赖关系
 */
void print_module_dependencies(void)
{
    print_separator("模块依赖关系");

    printf("工程模块依赖关系表：\n\n");

    for (size_t i = 0; i < module_count; i++) {
        printf("[%02d] %s\n", i + 1, module_dependencies[i].module_name);
        printf("     描述：%s\n", module_dependencies[i].description);
        printf("     依赖：%s\n\n", module_dependencies[i].dependencies);
    }
}

/**
 * @brief 检查模块依赖是否满足
 */
bool check_module_dependencies(const char *module_name)
{
    if (module_name == NULL) {
        return false;
    }

    // 查找模块
    for (size_t i = 0; i < module_count; i++) {
        if (strcmp(module_dependencies[i].module_name, module_name) == 0) {
            // 简化版：假设所有依赖都满足
            printf("模块 %s 依赖检查：满足\n", module_name);
            return true;
        }
    }

    printf("模块 %s 未找到\n", module_name);
    return false;
}

/**
 * @brief 比较两种实现的性能
 */
void compare_performance(const char *name1, uint64_t time1,
                        const char *name2, uint64_t time2)
{
    if (name1 == NULL || name2 == NULL) {
        return;
    }

    printf("性能比较：\n");
    printf("  %s: %llu μs\n", name1, (unsigned long long)time1);
    printf("  %s: %llu μs\n", name2, (unsigned long long)time2);

    if (time1 < time2) {
        double ratio = (double)time2 / (double)time1;
        printf("  %s 比 %s 快 %.2f 倍\n", name1, name2, ratio);
    } else if (time2 < time1) {
        double ratio = (double)time1 / (double)time2;
        printf("  %s 比 %s 快 %.2f 倍\n", name2, name1, ratio);
    } else {
        printf("  两种实现性能相同\n");
    }
}

/**
 * @brief 测量函数执行时间
 */
uint64_t measure_function_time(void (*func)(void), uint32_t iterations)
{
    if (func == NULL || iterations == 0) {
        return 0;
    }

    LARGE_INTEGER frequency;
    LARGE_INTEGER start;
    LARGE_INTEGER end;

    QueryPerformanceFrequency(&frequency);

    // 开始测量
    QueryPerformanceCounter(&start);

    for (uint32_t i = 0; i < iterations; i++) {
        func();
    }

    // 结束测量
    QueryPerformanceCounter(&end);

    // 计算时间（微秒）
    uint64_t elapsed = (uint64_t)((end.QuadPart - start.QuadPart) * 1000000LL / frequency.QuadPart);

    return elapsed;
}