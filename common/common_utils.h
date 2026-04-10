/**
 * @file common_utils.h
 * @brief 通用工具函数声明
 * @details 提供常用的工具函数,包括日志输出、断言、测试框架、延时、校验和计算等。
 *          这些工具函数为整个工程提供基础支持,所有模块都可以使用。
 * 
 * @section main_features 主要功能
 * - 日志系统:支持多级别日志输出(DEBUG/INFO/WARNING/ERROR/FATAL)
 * - 断言宏:运行时条件检查,用于调试和错误处理
 * - 测试框架:简化测试用例编写,自动输出测试结果
 * - 调试工具:分隔线打印、十六进制转储
 * - 时间函数:获取系统时间、延时
 * - 数据处理:字节交换、校验和、CRC计算
 * 
 * @section usage 使用方法
 * @code
 * #include "common/common_utils.h"
 * 
 * // 日志输出
 * LOG_INFO("系统启动,版本: %s", "1.0.0");
 * LOG_ERROR("打开文件失败: %s", filename);
 * 
 * // 测试用例
 * int32_t test_my_function(void) {
 *     TEST_ASSERT(my_function() == 0, "my_function应返回0");
 *     TEST_ASSERT_EQUAL(42, calculate(), "计算结果应为42");
 *     return 0;
 * }
 * 
 * // 十六进制转储
 * uint8_t data[] = {0x01, 0x02, 0x03};
 * print_hex_dump(data, sizeof(data), "数据内容");
 * @endcode
 * 
 * @author Embedded C Learning Project
 * @date 2026-03-22
 * @version 1.0.0
 * 
 * @note 日志级别可通过 log_set_level() 动态调整
 * @note 测试函数应返回0表示成功,非0表示失败
 */

#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common/common_types.h"
#include <stdio.h>

/*============================================================================*/
/*                           日志宏定义                                         */
/*============================================================================*/

/**
 * @brief 调试级别日志输出宏
 * @param fmt 格式字符串(printf格式)
 * @param ... 可变参数
 * @note 仅在DEBUG模式下输出,用于开发调试
 * @code
 * LOG_DEBUG("变量值: x=%d, y=%d", x, y);
 * @endcode
 */
#define LOG_DEBUG(fmt, ...)   log_output(LOG_LEVEL_DEBUG,   "[DEBUG] " fmt, ##__VA_ARGS__)

/**
 * @brief 信息级别日志输出宏
 * @param fmt 格式字符串
 * @param ... 可变参数
 * @note 用于输出一般运行信息
 */
#define LOG_INFO(fmt, ...)    log_output(LOG_LEVEL_INFO,    "[INFO] "  fmt, ##__VA_ARGS__)

/**
 * @brief 警告级别日志输出宏
 * @param fmt 格式字符串
 * @param ... 可变参数
 * @note 用于输出警告信息,表示可能存在问题
 */
#define LOG_WARNING(fmt, ...) log_output(LOG_LEVEL_WARNING, "[WARN] "  fmt, ##__VA_ARGS__)

/** @brief LOG_WARN 是 LOG_WARNING 的别名 */
#define LOG_WARN(fmt, ...)    LOG_WARNING(fmt, ##__VA_ARGS__)

/**
 * @brief 错误级别日志输出宏
 * @param fmt 格式字符串
 * @param ... 可变参数
 * @note 用于输出错误信息,表示发生了错误但程序可继续运行
 */
#define LOG_ERROR(fmt, ...)   log_output(LOG_LEVEL_ERROR,   "[ERROR] " fmt, ##__VA_ARGS__)

/**
 * @brief 致命错误日志输出宏
 * @param fmt 格式字符串
 * @param ... 可变参数
 * @note 用于输出致命错误信息,通常表示程序无法继续运行
 */
#define LOG_FATAL(fmt, ...)   log_output(LOG_LEVEL_FATAL,   "[FATAL] " fmt, ##__VA_ARGS__)

/*============================================================================*/
/*                           断言宏定义                                         */
/*============================================================================*/

/**
 * @brief 带消息的断言宏
 * @param cond 条件表达式
 * @param msg 断言失败时的消息
 * @note 条件为假时输出致命错误日志,包含文件名和行号
 * @code
 * ASSERT_MSG(ptr != NULL, "指针不能为空");
 * @endcode
 */
#define ASSERT_MSG(cond, msg) do { \
    if (!(cond)) { \
        LOG_FATAL("Assertion failed: %s, file: %s, line: %d", msg, __FILE__, __LINE__); \
    } \
} while(0)

/*============================================================================*/
/*                           测试框架宏                                         */
/*============================================================================*/

/**
 * @brief 运行测试函数宏
 * @param test_func 测试函数名
 * @note 自动输出测试开始和结果信息
 * @code
 * RUN_TEST(test_array_operations);
 * @endcode
 */
#define RUN_TEST(test_func) do { \
    printf("\n========== Running: %s ==========\n", #test_func); \
    int32_t result = test_func(); \
    if (result == 0) { \
        printf("[PASS] %s\n", #test_func); \
    } else { \
        printf("[FAIL] %s (error code: %d)\n", #test_func, result); \
    } \
} while(0)

/**
 * @brief 测试断言宏(条件检查)
 * @param cond 条件表达式
 * @param msg 断言失败时的消息
 * @note 条件为假时返回-1表示测试失败
 * @code
 * TEST_ASSERT(result > 0, "结果应大于0");
 * @endcode
 */
#define TEST_ASSERT(cond, msg) do { \
    if (!(cond)) { \
        LOG_ERROR("Test assertion failed: %s", msg); \
        return -1; \
    } \
} while(0)

/**
 * @brief 测试断言宏(相等检查)
 * @param expected 期望值
 * @param actual 实际值
 * @param msg 断言失败时的消息
 * @note 值不相等时返回-1表示测试失败,并输出期望值和实际值
 * @code
 * TEST_ASSERT_EQUAL(42, calculate(), "计算结果应为42");
 * @endcode
 */
#define TEST_ASSERT_EQUAL(expected, actual, msg) do { \
    if ((expected) != (actual)) { \
        LOG_ERROR("Test assertion failed: %s (expected: %d, actual: %d)", msg, (int)(expected), (int)(actual)); \
        return -1; \
    } \
} while(0)

/*============================================================================*/
/*                           日志函数                                           */
/*============================================================================*/

/**
 * @brief 日志输出函数
 * @param level 日志级别
 * @param fmt 格式字符串
 * @param ... 可变参数
 * @note 一般不直接调用,使用LOG_xxx宏
 */
void log_output(LogLevel_t level, const char *fmt, ...);

/**
 * @brief 设置日志输出级别
 * @param level 最低输出级别(低于此级别的日志不输出)
 * @code
 * log_set_level(LOG_LEVEL_INFO);  // 只输出INFO及以上级别的日志
 * @endcode
 */
void log_set_level(LogLevel_t level);

/*============================================================================*/
/*                           调试工具函数                                       */
/*============================================================================*/

/**
 * @brief 打印分隔线
 * @param title 标题文本(可为NULL)
 * @note 用于在控制台输出分隔线,增强可读性
 * @code
 * print_separator("基础语法演示");
 * // 输出: ======== 基础语法演示 ========
 * @endcode
 */
void print_separator(const char *title);

/**
 * @brief 打印十六进制转储
 * @param data 数据指针
 * @param len 数据长度
 * @param title 标题(可为NULL)
 * @note 以十六进制格式输出数据内容,每行16字节,附带ASCII显示
 * @code
 * uint8_t data[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F};
 * print_hex_dump(data, sizeof(data), "数据内容");
 * // 输出:
 * // 数据内容:
 * // 0000: 48 65 6C 6C 6F                                   Hello
 * @endcode
 */
void print_hex_dump(const uint8_t *data, size_t len, const char *title);

/*============================================================================*/
/*                           时间函数                                           */
/*============================================================================*/

/**
 * @brief 获取系统时间计数
 * @return 当前时间计数(毫秒)
 * @note 返回从系统启动到当前的毫秒数,用于计时
 * @code
 * uint32_t start = get_tick_count();
 * do_something();
 * uint32_t elapsed = get_tick_count() - start;
 * printf("耗时: %u ms\n", elapsed);
 * @endcode
 */
uint32_t get_tick_count(void);

/**
 * @brief 毫秒级延时
 * @param ms 延时毫秒数
 * @note 阻塞式延时,精度取决于系统
 * @warning 在嵌入式系统中,注意看门狗喂狗
 */
void delay_ms(uint32_t ms);

/*============================================================================*/
/*                           数据处理函数                                       */
/*============================================================================*/

/**
 * @brief 字节序交换
 * @param data 数据指针
 * @param size 数据大小(字节)
 * @note 原地交换数据的字节序,用于大小端转换
 * @code
 * uint32_t value = 0x12345678;
 * swap_bytes(&value, sizeof(value));
 * // value = 0x78563412
 * @endcode
 */
void swap_bytes(void *data, size_t size);

/**
 * @brief 计算累加和校验
 * @param data 数据指针
 * @param len 数据长度
 * @return 校验和(16位)
 * @note 简单的累加和校验,用于数据完整性检查
 */
uint16_t calc_checksum(const uint8_t *data, size_t len);

/**
 * @brief 计算CRC8校验值
 * @param data 数据指针
 * @param len 数据长度
 * @return CRC8校验值
 * @note 使用多项式 0x07(CRC-8-CCITT)
 */
uint8_t calc_crc8(const uint8_t *data, size_t len);

/**
 * @brief 计算CRC16校验值
 * @param data 数据指针
 * @param len 数据长度
 * @return CRC16校验值
 * @note 使用多项式 0x1021(CRC-16-CCITT)
 */
uint16_t calc_crc16(const uint8_t *data, size_t len);

#ifdef __cplusplus
}
#endif

#endif
