/**
 * @file data_structure.h
 * @brief 数据结构模块头文件
 * @details 整合嵌入式常用数据结构：静态数组、栈、队列、单向链表。
 *          所有数据结构均使用静态内存分配，适合资源受限的嵌入式环境。
 * 
 * @section main_features 主要内容
 * - 静态数组：带边界检查的安全数组
 * - 栈：LIFO后进先出数据结构
 * - 队列：FIFO先进先出数据结构
 * - 单向链表：动态数据集合管理
 * 
 * @section design_principles 设计原则
 * - 静态内存分配：避免malloc/free，提高系统稳定性
 * - 边界检查：所有操作都有参数验证
 * - 错误处理：所有函数都有返回值
 * - 可移植性：使用标准C99，无平台依赖
 * - MISRA-C合规：遵循编码规范
 * 
 * @section learning_objectives 学习目标
 * - 理解数据结构的基本概念和操作
 * - 掌握静态内存分配技术
 * - 学习嵌入式环境下的资源管理
 * - 了解时间复杂度和空间复杂度
 * 
 * @author Embedded C Learning Project
 * @date 2026-04-10
 * @version 1.0.0
 * 
 * @note 遵循 MISRA-C:2012 编码规范
 */

#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common/common_types.h"
#include "static_array.h"
#include "stack.h"
#include "queue.h"
#include "linked_list.h"

/*============================================================================*/
/*                           演示函数声明                                       */
/*============================================================================*/

/**
 * @brief 静态数组演示
 * @details 展示静态数组的基本操作：
 *          - 初始化和清空
 *          - 元素的添加、删除、访问
 *          - 边界检查和安全访问
 *          - 遍历操作
 */
void demo_static_array(void);

/**
 * @brief 栈演示
 * @details 展示栈的基本操作：
 *          - 初始化和清空
 *          - 压栈和弹栈操作
 *          - 查看栈顶元素
 *          - 栈空/栈满判断
 *          - 应用示例：括号匹配
 */
void demo_stack(void);

/**
 * @brief 队列演示
 * @details 展示队列的基本操作：
 *          - 初始化和清空
 *          - 入队和出队操作
 *          - 查看队首/队尾元素
 *          - 队列空/队列满判断
 *          - 应用示例：数据缓冲
 */
void demo_queue(void);

/**
 * @brief 单向链表演示
 * @details 展示单向链表的基本操作：
 *          - 初始化和清空
 *          - 头部/尾部插入
 *          - 按位置/按值删除
 *          - 遍历和查找
 *          - 链表反转
 */
void demo_linked_list(void);

/**
 * @brief 数据结构综合应用演示
 * @details 展示数据结构的综合应用：
 *          - 使用栈实现表达式求值
 *          - 使用队列实现任务调度
 *          - 使用链表实现动态配置管理
 */
void demo_data_structure_application(void);

#ifdef __cplusplus
}
#endif

#endif
