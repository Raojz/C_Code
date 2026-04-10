/**
 * @file common_types.h
 * @brief 通用类型定义和宏定义
 * @details 定义嵌入式C语言开发中常用的数据类型、宏和枚举,提供跨平台兼容性支持。
 *          本文件是整个工程的基础头文件,所有模块都应包含此文件。
 * 
 * @section main_features 主要功能
 * - 编译器兼容性宏(MSVC)
 * - 常用工具宏(数组大小、最值、绝对值等)
 * - 位操作宏(置位、清零、翻转、检查)
 * - 对齐相关宏(向上对齐、向下对齐、对齐检查)
 * - 字节操作宏(高低字节提取、字拼接)
 * - 状态码枚举
 * - 日志级别枚举
 * - 回调函数类型定义
 * 
 * @section usage 使用方法
 * @code
 * #include "common/common_types.h"
 * 
 * int arr[10];
 * size_t count = ARRAY_SIZE(arr);  // 获取数组元素个数
 * 
 * uint32_t reg = 0;
 * BIT_SET(reg, 3);      // 设置第3位
 * BIT_CLEAR(reg, 1);    // 清除第1位
 * 
 * Status_t status = do_something();
 * if (SUCCESS(status)) {
 *     // 操作成功
 * }
 * @endcode
 * 
 * @author Embedded C Learning Project
 * @date 2026-03-22
 * @version 1.0.0
 * 
 * @note 遵循 MISRA-C:2012 编码规范
 * @note 所有宏定义使用大括号包围,防止展开错误
 * @note 使用固定宽度整数类型确保跨平台一致性
 */

#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/*============================================================================*/
/*                           版本信息宏                                         */
/*============================================================================*/

/** @brief 主版本号 */
#define VERSION_MAJOR    1

/** @brief 次版本号 */
#define VERSION_MINOR    0

/** @brief 修订版本号 */
#define VERSION_PATCH    0

/**
 * @brief 版本字符串
 * @details 格式: "主版本.次版本.修订版本"
 */
#define VERSION_STRING   "1.0.0"

/*============================================================================*/
/*                           编译器兼容性宏                                     */
/*============================================================================*/

/** @brief MSVC导出符号声明 */
#define API_PUBLIC  __declspec(dllexport)
/** @brief MSVC私有符号声明 */
#define API_PRIVATE
/** @brief MSVC紧凑结构体开始标记 */
#define PACKED_BEGIN __pragma(pack(push, 1))
/** @brief MSVC紧凑结构体结束标记 */
#define PACKED_END   __pragma(pack(pop))
/** @brief MSVC强制内联 */
#define INLINE       __forceinline
/** @brief MSVC不返回属性 */
#define NORETURN     __declspec(noreturn)

/** @brief 紧凑结构体定义宏,用于消除结构体填充字节 */
#define PACKED_STRUCT struct PACKED_BEGIN PACKED_END

/*============================================================================*/
/*                           通用工具宏                                         */
/*============================================================================*/

/**
 * @brief 标记未使用的函数参数
 * @param param 未使用的参数名
 * @note 用于消除编译器警告
 */
#define UNUSED_PARAM(param) ((void)(param))

/**
 * @brief 计算数组元素个数
 * @param arr 数组名
 * @return 数组元素个数(size_t类型)
 * @warning 不能用于指针!仅适用于真实数组
 * @code
 * int arr[10];
 * size_t n = ARRAY_SIZE(arr);  // n = 10
 * @endcode
 */
#define ARRAY_SIZE(arr)       (sizeof(arr) / sizeof((arr)[0]))

/**
 * @brief 字节转换为位
 * @param byte 字节数
 * @return 位数
 */
#define BYTE_TO_BIT(byte)     ((byte) * 8U)

/**
 * @brief 位转换为字节(向上取整)
 * @param bit 位数
 * @return 字节数
 */
#define BIT_TO_BYTE(bit)      ((bit) / 8U + (((bit) % 8U) ? 1U : 0U))

/*============================================================================*/
/*                           数学运算宏                                         */
/*============================================================================*/

/**
 * @brief 取两个数中的最大值
 * @param a 第一个值
 * @param b 第二个值
 * @return 较大的值
 * @warning 参数会被求值多次,避免使用有副作用的表达式
 */
#define MAX(a, b)             (((a) > (b)) ? (a) : (b))

/**
 * @brief 取两个数中的最小值
 * @param a 第一个值
 * @param b 第二个值
 * @return 较小的值
 */
#define MIN(a, b)             (((a) < (b)) ? (a) : (b))

/**
 * @brief 取绝对值
 * @param x 输入值
 * @return 绝对值
 */
#define ABS(x)                (((x) < 0) ? (-(x)) : (x))

/**
 * @brief 将值限制在指定范围内
 * @param val 输入值
 * @param min 最小值
 * @param max 最大值
 * @return 限制后的值
 */
#define CLAMP(val, min, max)  (MIN(MAX(val, min), max))

/*============================================================================*/
/*                           对齐相关宏                                         */
/*============================================================================*/

/**
 * @brief 向上对齐到指定边界
 * @param x 输入值
 * @param align 对齐边界(必须是2的幂)
 * @return 对齐后的值
 * @code
 * ALIGN_UP(5, 4)   // 返回 8
 * ALIGN_UP(8, 4)   // 返回 8
 * @endcode
 */
#define ALIGN_UP(x, align)    (((x) + ((align) - 1)) & ~((align) - 1))

/**
 * @brief 向下对齐到指定边界
 * @param x 输入值
 * @param align 对齐边界(必须是2的幂)
 * @return 对齐后的值
 */
#define ALIGN_DOWN(x, align)  ((x) & ~((align) - 1))

/**
 * @brief 检查值是否对齐到指定边界
 * @param x 输入值(可以是整数或指针)
 * @param align 对齐边界(必须是2的幂)
 * @return true表示已对齐,false表示未对齐
 * @code
 * int arr[10];
 * IS_ALIGNED(arr, 4)      // 检查数组是否4字节对齐
 * IS_ALIGNED(100, 4)      // true,100是4的倍数
 * @endcode
 */
#define IS_ALIGNED(x, align)  (((uintptr_t)(x) & ((align) - 1)) == 0U)

/*============================================================================*/
/*                           位操作宏                                           */
/*============================================================================*/

/**
 * @brief 设置寄存器中的指定位
 * @param reg 寄存器变量
 * @param bit 位号(0-based)
 * @code
 * uint32_t reg = 0x00;
 * BIT_SET(reg, 3);    // reg = 0x08
 * @endcode
 */
#define BIT_SET(reg, bit)     ((reg) |= (1U << (bit)))

/**
 * @brief 清除寄存器中的指定位
 * @param reg 寄存器变量
 * @param bit 位号(0-based)
 */
#define BIT_CLEAR(reg, bit)   ((reg) &= ~(1U << (bit)))

/**
 * @brief 翻转寄存器中的指定位
 * @param reg 寄存器变量
 * @param bit 位号(0-based)
 */
#define BIT_TOGGLE(reg, bit)  ((reg) ^= (1U << (bit)))

/**
 * @brief 检查寄存器中的指定位是否为1
 * @param reg 寄存器变量
 * @param bit 位号(0-based)
 * @return 1表示该位为1,0表示该位为0
 */
#define BIT_CHECK(reg, bit)   (((reg) >> (bit)) & 1U)

/**
 * @brief 生成指定长度的位掩码
 * @param len 掩码长度
 * @return 位掩码
 * @code
 * BIT_MASK(4)   // 返回 0x0F
 * BIT_MASK(8)   // 返回 0xFF
 * @endcode
 */
#define BIT_MASK(len)         ((1U << (len)) - 1U)

/**
 * @brief 提取寄存器中的位域
 * @param reg 寄存器变量
 * @param start 起始位号
 * @param len 位域长度
 * @return 提取的位域值
 * @code
 * uint32_t reg = 0xABCD;
 * BIT_FIELD(reg, 4, 8)   // 提取bit4~bit11,返回0xBC
 * @endcode
 */
#define BIT_FIELD(reg, start, len) (((reg) >> (start)) & BIT_MASK(len))

/*============================================================================*/
/*                           字节操作宏                                         */
/*============================================================================*/

/**
 * @brief 提取16位值的低字节
 * @param x 16位输入值
 * @return 低字节(bit0~bit7)
 */
#define LOW_BYTE(x)           ((uint8_t)((x) & 0xFFU))

/**
 * @brief 提取16位值的高字节
 * @param x 16位输入值
 * @return 高字节(bit8~bit15)
 */
#define HIGH_BYTE(x)          ((uint8_t)(((x) >> 8) & 0xFFU))

/**
 * @brief 将两个字节拼接为16位字
 * @param h 高字节
 * @param l 低字节
 * @return 16位字
 */
#define MAKE_WORD(h, l)       ((uint16_t)(((uint16_t)(h) << 8) | (l)))

/**
 * @brief 提取32位值的低16位字
 * @param x 32位输入值
 * @return 低16位字
 */
#define LOW_WORD(x)           ((uint16_t)((x) & 0xFFFFU))

/**
 * @brief 提取32位值的高16位字
 * @param x 32位输入值
 * @return 高16位字
 */
#define HIGH_WORD(x)          ((uint16_t)(((x) >> 16) & 0xFFFFU))

/**
 * @brief 提取32位值的字节0(最低字节)
 * @param x 32位输入值
 * @return 字节0(bit0~bit7)
 */
#define GET_LOW_BYTE0(x)      ((uint8_t)((x) & 0xFFU))

/**
 * @brief 提取32位值的字节1
 * @param x 32位输入值
 * @return 字节1(bit8~bit15)
 */
#define GET_LOW_BYTE1(x)      ((uint8_t)(((x) >> 8) & 0xFFU))

/**
 * @brief 提取32位值的字节2
 * @param x 32位输入值
 * @return 字节2(bit16~bit23)
 */
#define GET_LOW_BYTE2(x)      ((uint8_t)(((x) >> 16) & 0xFFU))

/**
 * @brief 提取32位值的字节3(最高字节)
 * @param x 32位输入值
 * @return 字节3(bit24~bit31)
 */
#define GET_LOW_BYTE3(x)      ((uint8_t)(((x) >> 24) & 0xFFU))

/**
 * @brief 将两个16位字拼接为32位双字
 * @param h 高16位字
 * @param l 低16位字
 * @return 32位双字
 */
#define MAKE_DWORD(h, l)      ((uint32_t)(((uint32_t)(h) << 16) | (l)))

/*============================================================================*/
/*                           二进制格式化宏                                     */
/*============================================================================*/

/**
 * @brief 二进制输出格式字符串
 * @code
 * uint8_t val = 0xA5;
 * printf("val = " BYTE_TO_BINARY_FMT, BYTE_TO_BINARY(val));
 * // 输出: val = 10100101
 * @endcode
 */
#define BYTE_TO_BINARY_FMT "%c%c%c%c%c%c%c%c"

/**
 * @brief 将字节转换为8个字符参数(用于printf)
 * @param byte 要转换的字节
 * @return 8个字符参数,配合BYTE_TO_BINARY_FMT使用
 */
#define BYTE_TO_BINARY(byte) \
    (((byte) & 0x80U) ? '1' : '0'), \
    (((byte) & 0x40U) ? '1' : '0'), \
    (((byte) & 0x20U) ? '1' : '0'), \
    (((byte) & 0x10U) ? '1' : '0'), \
    (((byte) & 0x08U) ? '1' : '0'), \
    (((byte) & 0x04U) ? '1' : '0'), \
    (((byte) & 0x02U) ? '1' : '0'), \
    (((byte) & 0x01U) ? '1' : '0')

/*============================================================================*/
/*                           状态码枚举                                         */
/*============================================================================*/

/**
 * @brief 函数返回状态码枚举
 * @details 用于函数返回值,表示操作结果
 *          正值表示警告,0表示成功,负值表示错误
 */
typedef enum {
    STATUS_OK           = 0,    /**< 操作成功 */
    STATUS_ERROR        = -1,   /**< 一般错误 */
    STATUS_NULL_PTR     = -2,   /**< 空指针错误 */
    STATUS_INVALID_PARAM = -3,  /**< 无效参数错误 */
    STATUS_TIMEOUT      = -4,   /**< 超时错误 */
    STATUS_BUSY         = -5,   /**< 忙状态 */
    STATUS_NO_MEM       = -6,   /**< 内存不足错误 */
    STATUS_OVERFLOW     = -7,   /**< 溢出错误 */
    STATUS_UNDERFLOW    = -8,   /**< 下溢错误 */
    STATUS_NOT_FOUND    = -9    /**< 未找到错误 */
} Status_t;

/*============================================================================*/
/*                           日志级别枚举                                       */
/*============================================================================*/

/**
 * @brief 日志级别枚举
 * @details 用于日志系统,控制输出级别
 */
typedef enum {
    LOG_LEVEL_DEBUG   = 0,  /**< 调试信息,仅DEBUG模式输出 */
    LOG_LEVEL_INFO    = 1,  /**< 普通信息 */
    LOG_LEVEL_WARNING = 2,  /**< 警告信息 */
    LOG_LEVEL_ERROR   = 3,  /**< 错误信息 */
    LOG_LEVEL_FATAL   = 4   /**< 致命错误 */
} LogLevel_t;

/*============================================================================*/
/*                           回调函数类型定义                                   */
/*============================================================================*/

/**
 * @brief 通用回调函数类型
 * @param param 回调参数指针
 */
typedef void (*Callback_t)(void *param);

/**
 * @brief 比较函数类型
 * @param a 第一个元素指针
 * @param b 第二一个元素指针
 * @return <0表示a<b,=0表示a=b,>0表示a>b
 */
typedef int32_t (*CompareFunc_t)(const void *a, const void *b);

/**
 * @brief 谓词函数类型(用于条件判断)
 * @param item 待判断元素指针
 * @return true表示满足条件,false表示不满足
 */
typedef bool (*Predicate_t)(const void *item);

/*============================================================================*/
/*                           错误处理宏                                         */
/*============================================================================*/

/**
 * @brief 检查状态是否为成功
 * @param status 状态码
 * @return true表示成功
 */
#define SUCCESS(status)    ((status) == STATUS_OK)

/**
 * @brief 检查状态是否为失败
 * @param status 状态码
 * @return true表示失败
 */
#define FAILED(status)     ((status) != STATUS_OK)

/**
 * @brief 安全释放内存
 * @param ptr 指针变量(注意:必须是变量名,不是表达式)
 * @details 释放内存后将指针置为NULL,防止悬空指针
 * @code
 * int *p = malloc(sizeof(int));
 * SAFE_FREE(p);  // 释放后p = NULL
 * @endcode
 */
#define SAFE_FREE(ptr)     do { if ((ptr) != NULL) { free(ptr); (ptr) = NULL; } } while(0)

/**
 * @brief 空指针检查并返回错误
 * @param ptr 要检查的指针
 * @note 如果指针为NULL,返回STATUS_NULL_PTR
 */
#define RETURN_IF_NULL(ptr)       do { if ((ptr) == NULL) { return STATUS_NULL_PTR; } } while(0)

/**
 * @brief 失败状态检查并返回
 * @param status 状态码
 * @note 如果状态为失败,直接返回该状态码
 */
#define RETURN_IF_FAILED(status)  do { if (FAILED(status)) { return (status); } } while(0)

/**
 * @brief 空指针检查并返回指定值
 * @param ptr 要检查的指针
 * @param val 指针为NULL时的返回值
 */
#define RETURN_VAL_IF_NULL(ptr, val) do { if ((ptr) == NULL) { return (val); } } while(0)

/*============================================================================*/
/*                           静态断言                                          */
/*============================================================================*/

/**
 * @brief 编译时静态断言
 * @param cond 条件表达式
 * @param msg 断言失败时的错误消息
 * @note C11标准特性,编译时检查条件,失败则编译报错
 */
#if defined(_MSC_VER)
    #define STATIC_ASSERT_IMPL(cond, line) typedef char static_assert_##line[(cond) ? 1 : -1]
    #define STATIC_ASSERT(cond, msg) STATIC_ASSERT_IMPL(cond, __LINE__)
#else
    #define STATIC_ASSERT(cond, msg) _Static_assert(cond, msg)
#endif

/** @brief 静态断言:uint8_t必须为1字节 */
STATIC_ASSERT(sizeof(uint8_t) == 1, "uint8_t must be 1 byte");
/** @brief 静态断言:uint16_t必须为2字节 */
STATIC_ASSERT(sizeof(uint16_t) == 2, "uint16_t must be 2 bytes");
/** @brief 静态断言:uint32_t必须为4字节 */
STATIC_ASSERT(sizeof(uint32_t) == 4, "uint32_t must be 4 bytes");

#ifdef __cplusplus
}
#endif

#endif
