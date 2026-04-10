/**
 * @file control_flow.h
 * @brief 控制流模块头文件
 * @details 涵盖条件语句、循环结构、跳转语句等C语言控制流知识点。
 *          控制流是程序逻辑的基础，掌握各种控制结构是编写清晰代码的关键。
 * 
 * @section main_features 主要内容
 * - 条件语句：if-else、switch-case
 * - 循环结构：for、while、do-while
 * - 跳转语句：break、continue、goto、return
 * - 嵌套控制：多重循环、条件嵌套
 * - 状态机：使用控制流实现有限状态机
 * 
 * @section embedded_importance 嵌入式开发重要性
 * - 状态机是嵌入式系统的核心设计模式
 * - 循环结构用于轮询、数据处理
 * - 条件语句用于事件判断、错误处理
 * 
 * @section best_practices 最佳实践
 * - 避免过深的嵌套（建议不超过3层）
 * - switch语句必须有default分支
 * - 谨慎使用goto，仅用于错误处理跳转
 * - 循环中注意看门狗喂狗
 * 
 * @author Embedded C Learning Project
 * @date 2026-03-22
 * @version 1.0.0
 * 
 * @note MISRA-C对控制流有严格限制，如禁止无限循环使用非while(TRUE)形式
 */

#ifndef CONTROL_FLOW_H
#define CONTROL_FLOW_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common/common_types.h"

/*============================================================================*/
/*                           演示函数声明                                       */
/*============================================================================*/

/**
 * @brief if语句演示
 * @details 展示if-else语句的各种用法：
 *          - 基本if语句
 *          - if-else二选一
 *          - if-else if-else多选一
 *          - 嵌套if语句
 *          - 条件表达式的陷阱
 */
void demo_if_statement(void);

/**
 * @brief switch语句演示
 * @details 展示switch-case语句的使用：
 *          - 基本switch结构
 *          - case穿透和break
 *          - default分支的重要性
 *          - 多case共享代码块
 *          - switch与if的选择
 */
void demo_switch_statement(void);

/**
 * @brief for循环演示
 * @details 展示for循环的使用：
 *          - 基本for循环结构
 *          - 循环变量作用域
 *          - 多变量for循环
 *          - 无限循环：for(;;)
 *          - for循环与数组遍历
 */
void demo_for_loop(void);

/**
 * @brief while循环演示
 * @details 展示while循环的使用：
 *          - 基本while循环
 *          - 条件在前的循环
 *          - 无限循环：while(1)
 *          - while与for的选择
 */
void demo_while_loop(void);

/**
 * @brief do-while循环演示
 * @details 展示do-while循环的使用：
 *          - 基本do-while结构
 *          - 至少执行一次的特性
 *          - 典型应用：输入验证、菜单选择
 */
void demo_do_while_loop(void);

/**
 * @brief break和continue演示
 * @details 展示跳转语句的使用：
 *          - break：跳出循环
 *          - continue：跳过本次迭代
 *          - 嵌套循环中的break
 *          - 使用标志变量替代多层break
 */
void demo_break_continue(void);

/**
 * @brief goto语句演示
 * @details 展示goto语句的使用：
 *          - goto的基本语法
 *          - 合理使用场景：错误处理跳转
 *          - 滥用goto的危害
 *          - MISRA-C对goto的限制
 * @warning 谨慎使用goto，可能导致代码难以维护
 */
void demo_goto_statement(void);

/**
 * @brief 嵌套控制结构演示
 * @details 展示控制结构的嵌套使用：
 *          - 循环嵌套
 *          - 条件嵌套
 *          - 混合嵌套
 *          - 避免过深嵌套的方法
 */
void demo_nested_control(void);

/**
 * @brief 状态机演示
 * @details 展示使用控制流实现有限状态机：
 *          - 状态机的基本概念
 *          - 使用switch实现状态机
 *          - 状态转换表
 *          - 嵌入式中的典型应用（协议解析、设备控制）
 * @note 状态机是嵌入式系统的核心设计模式
 */
void demo_state_machine(void);

#ifdef __cplusplus
}
#endif

#endif
