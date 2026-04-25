/**
 * @file test_perf_helpers.h
 * @brief 性能测试辅助函数声明
 * @details 为 performance.c 提供可测量的测试辅助函数，
 *          这些函数用于性能基准测试，测量各种数据结构操作的时间。
 *
 * @section usage 使用说明
 * - 本文件定义了用于性能测试的辅助函数接口
 * - 每个函数执行特定的数据结构操作
 * - 配合 measure_function_time() 进行时间测量
 *
 * @author Embedded C Learning Project
 * @date 2026-04-21
 * @version 1.0.0
 */

#ifndef TEST_PERF_HELPERS_H
#define TEST_PERF_HELPERS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/*============================================================================*/
/*                           数组操作辅助函数                                   */
/*============================================================================*/

/**
 * @brief 测试数组创建操作
 * @note 用于性能测试，创建指定大小的静态数组
 */
void test_array_create(void);

/**
 * @brief 测试数组二分查找操作
 * @note 用于性能测试，在有序数组中执行二分查找
 */
void test_array_binary_search(void);

/**
 * @brief 测试数组遍历操作
 * @note 用于性能测试，遍历整个数组
 */
void test_array_traverse(void);

/*============================================================================*/
/*                           栈操作辅助函数                                     */
/*============================================================================*/

/**
 * @brief 测试栈创建操作
 * @note 用于性能测试，初始化栈结构
 */
void test_stack_create(void);

/**
 * @brief 测试栈压入操作
 * @note 用于性能测试，向栈中压入元素
 */
void test_stack_push(void);

/**
 * @brief 测试栈弹出操作
 * @note 用于性能测试，从栈中弹出元素
 */
void test_stack_pop(void);

/*============================================================================*/
/*                           队列操作辅助函数                                   */
/*============================================================================*/

/**
 * @brief 测试队列创建操作
 * @note 用于性能测试，初始化队列结构
 */
void test_queue_create(void);

/**
 * @brief 测试队列入队操作
 * @note 用于性能测试，向队尾添加元素
 */
void test_queue_enqueue(void);

/**
 * @brief 测试队列出队操作
 * @note 用于性能测试，从队首移除元素
 */
void test_queue_dequeue(void);

/*============================================================================*/
/*                           链表操作辅助函数                                   */
/*============================================================================*/

/**
 * @brief 测试链表创建操作
 * @note 用于性能测试，初始化链表并插入多个节点
 */
void test_linked_list_create(void);

/**
 * @brief 测试链表插入操作
 * @note 用于性能测试，在链表中插入新节点
 */
void test_linked_list_insert(void);

/**
 * @brief 测试链表删除操作
 * @note 用于性能测试，从链表中删除节点
 */
void test_linked_list_delete(void);

/**
 * @brief 测试链表查找操作
 * @note 用于性能测试，在链表中搜索特定值
 */
void test_linked_list_search(void);

#ifdef __cplusplus
}
#endif

#endif /* TEST_PERF_HELPERS_H */
