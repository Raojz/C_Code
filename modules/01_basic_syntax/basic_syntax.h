/**
 * @file basic_syntax.h
 * @brief 基础语法模块头文件
 * @details 涵盖变量、数据类型、运算符等C语言基础知识点的演示。
 *          本模块是学习嵌入式C语言的第一步，帮助理解C语言的基本概念。
 * 
 * @section main_features 主要内容
 * - 数据类型：基本类型、限定符、固定宽度类型
 * - 变量作用域：局部变量、全局变量、块作用域
 * - 存储类：auto、static、register、extern
 * - 常量类型：字面量、const、enum、#define
 * - 运算符：算术、关系、逻辑、位运算、赋值、条件
 * - 类型转换：隐式转换、显式转换
 * - sizeof运算符：获取类型和变量大小
 * - 运算符优先级：表达式求值顺序
 * 
 * @section learning_objectives 学习目标
 * - 理解C语言基本数据类型及其取值范围
 * - 掌握变量定义、初始化和作用域规则
 * - 熟悉各类运算符的使用方法和优先级
 * - 了解类型转换规则和潜在风险
 * 
 * @author Embedded C Learning Project
 * @date 2026-03-22
 * @version 1.0.0
 * 
 * @note 嵌入式开发推荐使用固定宽度整数类型（uint8_t, int32_t等）
 * @note 注意有符号数溢出是未定义行为
 */

#ifndef BASIC_SYNTAX_H
#define BASIC_SYNTAX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common/common_types.h"

/*============================================================================*/
/*                           演示函数声明                                       */
/*============================================================================*/

/**
 * @brief 数据类型演示
 * @details 展示C语言基本数据类型：char、short、int、long、float、double，
 *          以及类型限定符signed/unsigned，固定宽度类型uint8_t等。
 *          同时演示各类型的取值范围和存储大小。
 */
void demo_data_types(void);

/**
 * @brief 变量作用域演示
 * @details 展示局部变量、全局变量、块作用域变量的定义和使用。
 *          演示变量同名遮蔽（shadowing）现象。
 *          说明自动变量和静态局部变量的区别。
 */
void demo_variable_scope(void);

/**
 * @brief 存储类演示
 * @details 展示四种存储类说明符：
 *          - auto: 自动存储期（默认）
 *          - static: 静态存储期，作用域限制
 *          - register: 寄存器存储（建议）
 *          - extern: 外部链接声明
 */
void demo_storage_classes(void);

/**
 * @brief 常量类型演示
 * @details 展示C语言中定义常量的多种方式：
 *          - 字面量常量：10, 3.14, 'A', "hello"
 *          - const关键字：const int MAX = 100;
 *          - 枚举常量：enum { RED, GREEN, BLUE };
 *          - 宏定义常量：#define PI 3.14159
 */
void demo_constant_types(void);

/**
 * @brief 算术运算符演示
 * @details 展示基本算术运算符：+、-、*、/、%
 *          说明整数除法和浮点除法的区别。
 *          演示取模运算在嵌入式中的应用（如循环计数器）。
 */
void demo_arithmetic_operators(void);

/**
 * @brief 关系运算符演示
 * @details 展示关系运算符：==、!=、<、>、<=、>=
 *          说明返回值为0（假）或1（真）。
 *          演示浮点数比较的注意事项。
 */
void demo_relational_operators(void);

/**
 * @brief 逻辑运算符演示
 * @details 展示逻辑运算符：&&、||、!
 *          说明短路求值（short-circuit evaluation）特性。
 *          演示位运算符&/|/~与逻辑运算符的区别。
 */
void demo_logical_operators(void);

/**
 * @brief 位运算符演示
 * @details 展示位运算符：&、|、^、~、<<、>>
 *          这是嵌入式开发的核心技能，用于：
 *          - 硬件寄存器操作
 *          - 标志位设置和清除
 *          - 数据打包和解包
 */
void demo_bitwise_operators(void);

/**
 * @brief 赋值运算符演示
 * @details 展示赋值运算符：=、+=、-=、*=、/=、%=、&=、|=、^=、<<=、>>=
 *          说明复合赋值运算符的效率优势。
 */
void demo_assignment_operators(void);

/**
 * @brief 条件运算符演示
 * @details 展示三目条件运算符：? :
 *          语法：condition ? expr1 : expr2
 *          演示其在嵌入式中的典型应用（如取最大值）。
 */
void demo_conditional_operator(void);

/**
 * @brief 类型转换演示
 * @details 展示隐式类型转换和显式类型转换（强制转换）：
 *          - 隐式转换：自动进行的类型提升
 *          - 显式转换：(type)value
 *          说明转换风险：精度丢失、符号扩展、溢出
 */
void demo_type_conversion(void);

/**
 * @brief sizeof运算符演示
 * @details 展示sizeof运算符获取类型和变量的大小。
 *          说明sizeof是编译时运算符，不是函数。
 *          演示结构体大小与内存对齐的关系。
 */
void demo_sizeof_operator(void);

/**
 * @brief 运算符优先级演示
 * @details 展示C语言运算符的优先级和结合性。
 *          建议使用括号明确表达式意图，避免依赖优先级。
 *          演示常见陷阱：*p++、a & b == c等。
 */
void demo_operator_precedence(void);

#ifdef __cplusplus
}
#endif

#endif
