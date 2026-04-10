/**
 * @file preprocessor.h
 * @brief 预处理器模块头文件
 * @details 涵盖宏定义、条件编译、文件包含、常用预处理指令等知识点。
 *          预处理器是C语言编译过程的第一步,掌握预处理器对理解编译原理和编写可移植代码至关重要。
 * 
 * @section main_features 主要内容
 * - 宏定义：#define、宏函数、宏参数
 * - 条件编译：#ifdef、#ifndef、#if、#elif、#else、#endif
 * - 文件包含：#include、头文件保护
 * - 预定义宏：__FILE__、__LINE__、__DATE__、__TIME__等
 * - 常用技巧：字符串化、连接、可变参数宏
 * - 编译控制：调试开关、平台适配
 * 
 * @section embedded_importance 嵌入式开发重要性
 * - 宏定义用于硬件寄存器映射
 * - 条件编译用于平台适配和功能裁剪
 * - 调试宏用于日志输出和断言
 * - 配置宏用于功能开关
 * 
 * @section best_practices 最佳实践
 * - 宏定义使用大括号包围
 * - 多表达式宏使用do-while(0)
 * - 避免宏的副作用
 * - 使用const替代宏常量
 * - 使用inline函数替代宏函数
 * 
 * @author Embedded C Learning Project
 * @date 2026-03-22
 * @version 1.0.0
 * 
 * @note 本文件遵循 MISRA-C:2012 编码规范
 * @note MISRA对宏的使用有严格限制
 */

#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common/common_types.h"

/*============================================================================*/
/*                           演示函数声明                                       */
/*============================================================================*/

/**
 * @brief 宏定义基础演示
 * @details 展示宏定义的基本用法:
 *          - 对象宏:#define PI 3.14159
 *          - 函数宏:#define MAX(a,b) ((a)>(b)?(a):(b))
 *          - #undef取消定义
 *          - 宏的作用域
 */
void demo_macro_basics(void);

/**
 * @brief 高级宏技巧演示
 * @details 展示宏的高级用法:
 *          - 字符串化运算符 #
 *          - 连接运算符 ##
 *          - 可变参数宏 __VA_ARGS__
 *          - 宏的嵌套使用
 */
void demo_macro_advanced(void);

/**
 * @brief 条件编译演示
 * @details 展示条件编译的使用:
 *          - #ifdef、#ifndef
 *          - #if、#elif、#else、#endif
 *          - defined运算符
 *          - 条件编译的实际应用
 */
void demo_conditional_compile(void);

/**
 * @brief 文件包含演示
 * @details 展示文件包含的使用:
 *          - #include <file> 和 #include "file"的区别
 *          - 头文件保护
 *          - 头文件组织原则
 *          - 避免重复包含
 */
void demo_file_include(void);

/**
 * @brief 预定义宏演示
 * @details 展示编译器预定义宏:
 *          - __FILE__:当前文件名
 *          - __LINE__:当前行号
 *          - __DATE__:编译日期
 *          - __TIME__:编译时间
 *          - __func__:当前函数名
 */
void demo_predefined_macros(void);

/**
 * @brief #pragma指令演示
 * @details 展示#pragma指令的使用:
 *          - #pragma once
 *          - #pragma pack
 *          - #pragma message
 *          - #pragma warning
 */
void demo_pragma_directive(void);

/**
 * @brief 编译时检查演示
 * @details 展示编译时检查技术:
 *          - 静态断言 _Static_assert
 *          - 编译时计算
 *          - 类型大小检查
 *          - 数组大小检查
 */
void demo_compile_time_checks(void);

/**
 * @brief 代码生成演示
 * @details 展示代码生成技术:
 *          - X-Macro模式
 *          - 自动生成枚举
 *          - 自动生成字符串数组
 *          - 减少重复代码
 */
void demo_code_generation(void);

/*============================================================================*/
/*                           预处理器特有宏定义                                 */
/*============================================================================*/

/** @brief 字符串化宏 */
#define STRINGIFY(x) #x

/** @brief 连接宏 */
#define CONCAT(a, b) a##b

/**
 * @brief MIN函数模板宏
 * @details 用于生成特定类型的MIN函数
 * @param type 数据类型
 * @code
 * MIN_IMPL(int)    // 生成 MIN_int(int, int)
 * MIN_IMPL(float)  // 生成 MIN_float(float, float)
 * @endcode
 */
#define MIN_IMPL(type) \
    static inline type MIN_##type(type a, type b) { return ((a) < (b)) ? (a) : (b); }

/**
 * @brief 参数计数宏
 * @details 用于获取可变参数的个数
 * @note 支持1-3个参数
 */
#define COUNT_ARGS(...) COUNT_ARGS_IMPL(__VA_ARGS__, 3, 2, 1)
#define COUNT_ARGS_IMPL(_1, _2, _3, n, ...) n

/** @brief 调试打印宏 */
#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...) printf("[DEBUG] " fmt "\n", ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...) ((void)0)
#endif

/** @brief 断言宏 */
#ifdef DEBUG
#define ASSERT(expr) \
    do { \
        if (!(expr)) { \
            printf("Assertion failed: %s, file %s, line %d\n", \
                   #expr, __FILE__, __LINE__); \
        } \
    } while (0)
#else
#define ASSERT(expr) ((void)0)
#endif

#ifdef __cplusplus
}
#endif

#endif
