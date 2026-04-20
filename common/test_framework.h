/**
 * @file test_framework.h
 * @brief 统一测试框架头文件
 * @details 提供测试管理和统计功能，整合所有模块的测试入口。
 *
 * @author Embedded C Learning Project
 * @date 2026-04-11
 * @version 1.0.0
 */

#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common/common_types.h"

/*============================================================================*/
/*                           测试统计结构体                                     */
/*============================================================================*/

/**
 * @brief 测试统计结构体
 */
typedef struct {
    int32_t total;      /**< 总测试数 */
    int32_t passed;     /**< 通过数 */
    int32_t failed;     /**< 失败数 */
} TestStats_t;

/*============================================================================*/
/*                           测试框架函数                                       */
/*============================================================================*/

/**
 * @brief 初始化测试统计
 */
void test_init(void);

/**
 * @brief 获取测试统计
 * @return 测试统计结构体
 */
TestStats_t test_get_stats(void);

/**
 * @brief 打印测试报告
 */
void test_print_report(void);

/**
 * @brief 记录测试通过
 */
void test_pass(void);

/**
 * @brief 记录测试失败
 */
void test_fail(void);

/**
 * @brief 运行所有模块测试
 * @return 失败的测试数量，0表示全部通过
 */
int32_t run_all_tests(void);

/*============================================================================*/
/*                           各模块测试函数声明                                 */
/*============================================================================*/

/**
 * @brief 运行基础语法模块测试
 * @return 失败的测试数量
 */
int32_t test_basic_syntax(void);

/**
 * @brief 运行控制流模块测试
 * @return 失败的测试数量
 */
int32_t test_control_flow(void);

/**
 * @brief 运行函数与作用域模块测试
 * @return 失败的测试数量
 */
int32_t test_function_scope(void);

/**
 * @brief 运行数组与字符串模块测试
 * @return 失败的测试数量
 */
int32_t test_array_string(void);

/**
 * @brief 运行指针与内存模块测试
 * @return 失败的测试数量
 */
int32_t test_pointer_memory(void);

/**
 * @brief 运行结构体与联合体模块测试
 * @return 失败的测试数量
 */
int32_t test_struct_union(void);

/**
 * @brief 运行预处理指令模块测试
 * @return 失败的测试数量
 */
int32_t test_preprocessor(void);

/**
 * @brief 运行文件I/O模块测试
 * @return 失败的测试数量
 */
int32_t test_file_io(void);

/**
 * @brief 运行位运算模块测试
 * @return 失败的测试数量
 */
int32_t test_bit_operation(void);

/**
 * @brief 运行错误处理与调试模块测试
 * @return 失败的测试数量
 */
int32_t test_error_debug(void);

/**
 * @brief 运行数据结构模块测试
 * @return 失败的测试数量
 */
int32_t test_data_structure(void);

#ifdef __cplusplus
}
#endif

#endif
