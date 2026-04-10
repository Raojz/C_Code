/**
 * @file bit_operation.h
 * @brief 位操作模块头文件
 * @details 涵盖位运算符、位操作技巧、硬件寄存器操作等知识点。
 *          位操作是嵌入式开发的核心技能，用于硬件控制和数据处理。
 * 
 * @section main_features 主要内容
 * - 位运算符：&、|、^、~、<<、>>
 * - 位操作技巧：设置、清除、翻转、检查
 * - 硬件寄存器：GPIO、外设配置
 * - 位掩码应用：数据提取、打包、权限控制
 * - 位操作算法：位计数、反转、交换
 * 
 * @section embedded_importance 嵌入式开发重要性
 * - 硬件寄存器操作是嵌入式开发的核心
 * - 位操作用于控制外设
 * - 位掩码用于数据提取和设置
 * - 位操作是高效的数据处理方式
 * 
 * @section best_practices 最佳实践
 * - 使用宏定义位操作
 * - 注意无符号数的使用
 * - 硬件寄存器使用volatile
 * - 注意位操作的副作用
 * 
 * @author Embedded C Learning Project
 * @date 2026-03-22
 * @version 1.0.0
 * 
 * @note 本文件遵循 MISRA-C:2012 编码规范
 */

#ifndef BIT_OPERATION_H
#define BIT_OPERATION_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common/common_types.h"

/*============================================================================*/
/*                           权限定义                                           */
/*============================================================================*/

/** @brief 读权限 */
#define PERM_READ    (1U << 0)

/** @brief 写权限 */
#define PERM_WRITE   (1U << 1)

/** @brief 执行权限 */
#define PERM_EXECUTE (1U << 2)

/** @brief 删除权限 */
#define PERM_DELETE  (1U << 3)

/*============================================================================*/
/*                           演示函数声明                                       */
/*============================================================================*/

/**
 * @brief 位运算符演示
 */
void demo_bit_operators(void);

/**
 * @brief 位操作技巧演示
 */
void demo_bit_tricks(void);

/**
 * @brief 硬件寄存器操作演示
 */
void demo_hardware_register(void);

/**
 * @brief 位掩码应用演示
 */
void demo_bitmask_application(void);

/**
 * @brief 位操作算法演示
 */
void demo_bit_algorithms(void);

#ifdef __cplusplus
}
#endif

#endif
