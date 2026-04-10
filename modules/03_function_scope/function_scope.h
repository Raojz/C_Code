/**
 * @file function_scope.h
 * @brief 函数与作用域模块头文件
 * @details 涵盖函数定义、声明、参数传递、作用域规则等知识点。
 *          函数是C语言模块化编程的基础，理解作用域规则对编写可维护代码至关重要。
 * 
 * @section main_features 主要内容
 * - 函数基础：声明、定义、调用、返回值
 * - 参数传递：值传递、指针传递、数组传递
 * - 多返回值：指针参数、结构体返回
 * - 作用域规则：文件作用域、块作用域、函数原型作用域
 * - 存储类：static函数、extern声明、inline函数
 * - 递归函数：递归定义、终止条件、栈空间
 * - 函数指针：回调函数、函数指针数组
 * - 可变参数：va_list宏的使用
 * 
 * @section embedded_importance 嵌入式开发重要性
 * - 函数是模块化设计的基础
 * - static函数限制作用域，避免命名冲突
 * - 函数指针实现回调机制和状态机
 * - 内联函数用于性能关键代码
 * 
 * @section best_practices 最佳实践
 * - 函数应单一职责，长度适中
 * - 避免过多参数（建议不超过4个）
 * - 使用const修饰只读参数
 * - 指针参数必须检查NULL
 * - 优先使用迭代而非递归
 * 
 * @author Embedded C Learning Project
 * @date 2026-03-22
 * @version 1.0.0
 * 
 * @note 本文件遵循 MISRA-C:2012 编码规范
 */

#ifndef FUNCTION_SCOPE_H
#define FUNCTION_SCOPE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common/common_types.h"
#include <stdarg.h>

/*============================================================================*/
/*                           类型定义                                           */
/*============================================================================*/

/**
 * @brief 最小最大值结构体
 */
typedef struct {
    int32_t min;  /**< 最小值 */
    int32_t max;  /**< 最大值 */
} MinMax_t;

/**
 * @brief 运算函数指针类型
 */
typedef int32_t (*OperationFunc_t)(int32_t, int32_t);

/**
 * @brief 处理函数指针类型
 */
typedef int32_t (*ProcessFunc_t)(int32_t);

/*============================================================================*/
/*                           基本函数演示                                       */
/*============================================================================*/

/**
 * @brief 函数声明与定义演示
 * @details 展示函数的基本概念：
 *          - 函数声明（原型）
 *          - 函数定义
 *          - 函数调用
 *          - 函数返回值
 */
void demo_function_basics(void);

/**
 * @brief 两数相加
 * @param[in] a 第一个数
 * @param[in] b 第二个数
 * @return 两数之和
 */
int32_t add_numbers(int32_t a, int32_t b);

/**
 * @brief 打印问候语
 * @param[in] name 名称字符串
 */
void print_greeting(const char *name);

/*============================================================================*/
/*                           参数传递演示                                       */
/*============================================================================*/

/**
 * @brief 参数传递演示
 * @details 展示C语言的参数传递方式：
 *          - 值传递：复制实参的值
 *          - 指针传递：传递地址，可修改实参
 *          - 数组传递：退化为指针
 *          - const参数：防止修改
 */
void demo_parameter_passing(void);

/**
 * @brief 值传递示例（尝试修改值）
 * @param[in] value 输入值
 * @return 修改后的值
 */
int32_t modify_value(int32_t value);

/**
 * @brief 指针传递示例（修改原值）
 * @param[in,out] ptr 指向要修改的变量
 */
void modify_by_pointer(int32_t *ptr);

/**
 * @brief 计算数组元素之和
 * @param[in] arr 数组指针
 * @param[in] size 数组大小
 * @return 数组元素之和
 */
int32_t sum_array(const int32_t *arr, size_t size);

/**
 * @brief 获取字符串长度
 * @param[in] str 字符串指针
 * @return 字符串长度
 */
size_t get_string_length(const char *str);

/*============================================================================*/
/*                           多返回值演示                                       */
/*============================================================================*/

/**
 * @brief 多返回值演示
 * @details 展示C语言实现多返回值的方法：
 *          - 指针参数：通过指针参数返回多个值
 *          - 结构体：返回包含多个值的结构体
 *          - 数组：返回数组指针
 */
void demo_multiple_return_values(void);

/**
 * @brief 除法运算（返回商和余数）
 * @param[in] dividend 被除数
 * @param[in] divisor 除数
 * @param[out] quotient 商
 * @param[out] remainder 余数
 * @return 成功返回true，除数为0返回false
 */
bool divide_numbers(int32_t dividend, int32_t divisor, int32_t *quotient, int32_t *remainder);

/**
 * @brief 查找数组最小值和最大值
 * @param[in] arr 数组指针
 * @param[in] size 数组大小
 * @return 包含最小值和最大值的结构体
 */
MinMax_t find_min_max(const int32_t *arr, size_t size);

/*============================================================================*/
/*                           作用域规则演示                                     */
/*============================================================================*/

/**
 * @brief 作用域规则演示
 * @details 展示C语言的作用域规则：
 *          - 文件作用域：全局变量、函数
 *          - 块作用域：局部变量
 *          - 函数原型作用域：参数名
 *          - 函数作用域：标签名
 */
void demo_scope_rules(void);

/*============================================================================*/
/*                           存储类演示                                         */
/*============================================================================*/

/**
 * @brief 存储类演示
 * @details 展示函数相关的存储类：
 *          - static函数：文件作用域，内部链接
 *          - extern声明：引用外部函数
 *          - inline函数：建议内联展开
 */
void demo_storage_class(void);

/**
 * @brief 递增计数器（演示static局部变量）
 */
void increment_counter(void);

/*============================================================================*/
/*                           递归函数演示                                       */
/*============================================================================*/

/**
 * @brief 递归函数演示
 * @details 展示递归函数的使用：
 *          - 递归定义：函数调用自身
 *          - 递归终止条件
 *          - 递归调用栈
 *          - 递归与迭代的选择
 */
void demo_recursion(void);

/**
 * @brief 计算阶乘（递归实现）
 * @param[in] n 非负整数
 * @return n的阶乘
 */
int64_t factorial(int32_t n);

/**
 * @brief 计算斐波那契数（递归实现）
 * @param[in] n 项数（从0开始）
 * @return 第n项斐波那契数
 */
int64_t fibonacci(int32_t n);

/*============================================================================*/
/*                           函数指针演示                                       */
/*============================================================================*/

/**
 * @brief 函数指针演示
 * @details 展示函数指针的使用：
 *          - 函数指针声明
 *          - 函数指针赋值
 *          - 通过函数指针调用函数
 *          - 函数指针数组
 *          - 回调函数
 */
void demo_function_pointer(void);

/**
 * @brief 减法运算
 * @param[in] a 被减数
 * @param[in] b 减数
 * @return 差
 */
int32_t subtract_numbers(int32_t a, int32_t b);

/**
 * @brief 乘法运算
 * @param[in] a 第一个数
 * @param[in] b 第二个数
 * @return 积
 */
int32_t multiply_numbers(int32_t a, int32_t b);

/**
 * @brief 处理数组元素
 * @param[in,out] arr 数组指针
 * @param[in] size 数组大小
 * @param[in] processor 处理函数指针
 */
void process_array(int32_t *arr, size_t size, ProcessFunc_t processor);

/*============================================================================*/
/*                           可变参数演示                                       */
/*============================================================================*/

/**
 * @brief 可变参数函数演示
 * @details 展示可变参数函数的使用：
 *          - va_list、va_start、va_arg、va_end宏
 *          - 可变参数函数的声明和定义
 *          - 可变参数的安全使用
 */
void demo_variadic_function(void);

/**
 * @brief 计算多个整数的和
 * @param[in] count 整数个数
 * @param[in] ... 可变参数列表
 * @return 整数之和
 */
int32_t sum_integers(int32_t count, ...);

/*============================================================================*/
/*                           内联函数定义                                       */
/*============================================================================*/

/**
 * @brief 内联函数：返回两数中的较大值
 * @param[in] a 第一个数
 * @param[in] b 第二个数
 * @return 较大值
 */
static inline int32_t inline_max(int32_t a, int32_t b)
{
    return ((a) > (b)) ? (a) : (b);
}

/**
 * @brief 内联函数：返回两数中的较小值
 * @param[in] a 第一个数
 * @param[in] b 第二个数
 * @return 较小值
 */
static inline int32_t inline_min(int32_t a, int32_t b)
{
    return ((a) < (b)) ? (a) : (b);
}

#ifdef __cplusplus
}
#endif

#endif
