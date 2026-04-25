/**
 * @file performance.h
 * @brief 性能测试模块头文件
 * @details 提供对各模块进行性能测试的功能，包括时间测量、内存使用分析等。
 *          该模块帮助学习者了解不同数据结构和算法的性能特性。
 *
 * @section performance_main_features 主要功能
 * - 时间测量：精确测量函数执行时间
 * - 内存分析：跟踪动态内存分配和释放
 * - 性能基准：提供常见操作的性能参考值
 * - 依赖管理：管理模块间的依赖关系
 *
 * @section performance_usage 使用方法
 * @code
 * #include "performance.h"
 *
 * // 测试数组操作性能
 * test_array_performance();
 *
 * // 测试链表操作性能
 * test_linked_list_performance();
 *
 * // 查看依赖关系
 * print_module_dependencies();
 * @endcode
 *
 * @author Embedded C Learning Project
 * @date 2026-04-21
 * @version 1.0.0
 */

#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common/common_types.h"
#include <stdint.h>

/*============================================================================*/
/*                           性能测试类型定义                                   */
/*============================================================================*/

/**
 * @brief 性能测试结果结构体
 */
typedef struct {
    const char *test_name;    /**< 测试名称 */
    uint64_t    execution_time_us; /**< 执行时间（微秒） */
    size_t      memory_used;   /**< 使用的内存（字节） */
    uint32_t    iterations;    /**< 迭代次数 */
    bool        passed;        /**< 是否通过测试 */
} PerformanceResult_t;

/**
 * @brief 模块依赖信息结构体
 */
typedef struct {
    const char *module_name;   /**< 模块名称 */
    const char *dependencies;  /**< 依赖的模块（逗号分隔） */
    const char *description;   /** 模块描述 */
} ModuleDependency_t;

/*============================================================================*/
/*                           性能测试函数声明                                   */
/*============================================================================*/

/**
 * @brief 初始化性能测试模块
 * @retval STATUS_OK 初始化成功
 * @retval STATUS_ERROR 初始化失败
 */
Status_t performance_init(void);

/**
 * @brief 测试数组操作性能
 * @details 测试数组在不同规模下的插入、删除、查找操作性能
 */
void test_array_performance(void);

/**
 * @brief 测试栈操作性能
 * @details 测试栈的push和pop操作性能
 */
void test_stack_performance(void);

/**
 * @brief 测试队列操作性能
 * @details 测试队列的enqueue和dequeue操作性能
 */
void test_queue_performance(void);

/**
 * @brief 测试链表操作性能
 * @details 测试链表的插入、删除、查找操作性能
 */
void test_linked_list_performance(void);

/**
 * @brief 运行所有数据结构性能测试
 * @details 依次测试所有数据结构模块的性能并输出结果
 */
void run_all_performance_tests(void);

/**
 * @brief 打印模块依赖关系
 * @details 显示当前工程中各模块之间的依赖关系
 */
void print_module_dependencies(void);

/**
 * @brief 检查模块依赖是否满足
 * @param module_name 模块名称
 * @return true 依赖满足，false 依赖不满足
 */
bool check_module_dependencies(const char *module_name);

/**
 * @brief 比较两种实现的性能
 * @param name1 第一种实现的名称
 * @param time1 第一种实现的执行时间（微秒）
 * @param name2 第二种实现的名称
 * @param time2 第二种实现的执行时间（微秒）
 * @details 输出性能比较结果，指出哪种实现更快以及快多少
 */
void compare_performance(const char *name1, uint64_t time1,
                        const char *name2, uint64_t time2);

/**
 * @brief 测量函数执行时间
 * @param func 要测量的函数指针
 * @param iterations 测量次数
 * @return 平均执行时间（微秒）
 * @details 多次执行函数并计算平均时间，提高测量精度
 */
uint64_t measure_function_time(void (*func)(void), uint32_t iterations);

#ifdef __cplusplus
}
#endif

#endif
