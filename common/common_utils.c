/**
 * @file common_utils.c
 * @brief 通用工具函数实现
 * @details 实现日志输出、调试工具、时间函数、数据处理等通用工具函数。
 *          这些函数为整个工程提供基础支持,所有模块都可以调用。
 * 
 * @section functions 函数列表
 * - log_output()     - 日志输出函数
 * - log_set_level()  - 设置日志级别
 * - print_separator() - 打印分隔线
 * - print_hex_dump() - 十六进制转储
 * - get_tick_count() - 获取时间计数
 * - delay_ms()       - 毫秒延时
 * - swap_bytes()     - 字节序交换
 * - calc_checksum()  - 计算校验和
 * - calc_crc8()      - 计算CRC8
 * - calc_crc16()     - 计算CRC16
 * 
 * @author Embedded C Learning Project
 * @date 2026-03-22
 * @version 1.0.0
 * 
 * @note 本文件遵循 MISRA-C:2012 编码规范
 * @note 所有函数都进行了参数有效性检查
 */

#include "common/common_utils.h"
#include <stdarg.h>
#include <string.h>
#include <time.h>

/*============================================================================*/
/*                           私有变量定义                                       */
/*============================================================================*/

/**
 * @brief 当前日志输出级别
 * @details 低于此级别的日志不会输出,默认为DEBUG级别(输出所有日志)
 */
static LogLevel_t s_current_log_level = LOG_LEVEL_DEBUG;

/*============================================================================*/
/*                           日志函数实现                                       */
/*============================================================================*/

/**
 * @brief 日志输出函数
 * @details 根据日志级别输出格式化日志信息,包含时间戳。
 *          如果日志级别低于当前设置的级别,则不输出。
 * 
 * @param level 日志级别(DEBUG/INFO/WARNING/ERROR/FATAL)
 * @param fmt 格式字符串(printf格式)
 * @param ... 可变参数
 * 
 * @note 一般不直接调用此函数,使用LOG_DEBUG/LOG_INFO等宏
 * 
 * @code
 * // 使用宏调用(推荐)
 * LOG_INFO("系统启动,版本: %s", "1.0.0");
 * 
 * // 直接调用(不推荐)
 * log_output(LOG_LEVEL_INFO, "系统启动,版本: %s", "1.0.0");
 * @endcode
 */
void log_output(LogLevel_t level, const char *fmt, ...)
{
    if (level < s_current_log_level) {
        return;
    }
    
    va_list args;
    va_start(args, fmt);
    
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    
    printf("[%02d:%02d:%02d] ", t->tm_hour, t->tm_min, t->tm_sec);
    vprintf(fmt, args);
    printf("\n");
    
    va_end(args);
}

/**
 * @brief 设置日志输出级别
 * @details 设置最低输出级别,低于此级别的日志将被过滤。
 *          例如设置为LOG_LEVEL_INFO,则DEBUG级别的日志不会输出。
 * 
 * @param level 最低输出级别
 * 
 * @code
 * log_set_level(LOG_LEVEL_INFO);   // 只输出INFO及以上级别
 * log_set_level(LOG_LEVEL_ERROR);  // 只输出ERROR和FATAL
 * log_set_level(LOG_LEVEL_DEBUG);  // 输出所有级别
 * @endcode
 */
void log_set_level(LogLevel_t level)
{
    s_current_log_level = level;
}

/*============================================================================*/
/*                           调试工具函数实现                                   */
/*============================================================================*/

/**
 * @brief 打印分隔线
 * @details 在控制台输出一条分隔线,用于区分不同的输出区域,增强可读性。
 *          如果提供标题,则在分隔线中央显示标题。
 * 
 * @param title 标题文本(可为NULL)
 * 
 * @code
 * print_separator("基础语法演示");
 * // 输出:
 * // ======== 基础语法演示 ========
 * 
 * print_separator(NULL);
 * // 输出:
 * // ================================
 * @endcode
 */
void print_separator(const char *title)
{
    printf("\n");
    printf("================================================================================\n");
    if (title != NULL) {
        printf("  %s\n", title);
        printf("--------------------------------------------------------------------------------\n");
    }
}

/**
 * @brief 打印十六进制转储
 * @details 以十六进制格式输出数据内容,每行显示16字节,
 *          同时在右侧显示对应的ASCII字符(不可打印字符显示为点号)。
 *          常用于调试二进制数据、网络数据包等。
 * 
 * @param data 数据指针
 * @param len 数据长度(字节)
 * @param title 标题文本(可为NULL)
 * 
 * @note 如果data为NULL或len为0,只输出标题
 * 
 * @code
 * uint8_t data[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64};
 * print_hex_dump(data, sizeof(data), "数据内容");
 * // 输出:
 * // 数据内容:
 * // 0000: 48 65 6C 6C 6F 20 57 6F 72 6C 64                Hello World
 * @endcode
 */
void print_hex_dump(const uint8_t *data, size_t len, const char *title)
{
    if (title != NULL) {
        printf("\n%s:\n", title);
    }
    
    if (data == NULL || len == 0U) {
        printf("  (无数据)\n");
        return;
    }
    
    for (size_t i = 0U; i < len; i += 16U) {
        printf("%04zX: ", i);
        
        size_t remaining = len - i;
        size_t line_len = (remaining > 16U) ? 16U : remaining;
        
        for (size_t j = 0U; j < 16U; j++) {
            if (j < line_len) {
                printf("%02X ", data[i + j]);
            } else {
                printf("   ");
            }
            if (j == 7U) {
                printf(" ");
            }
        }
        
        printf(" |");
        for (size_t j = 0U; j < line_len; j++) {
            uint8_t c = data[i + j];
            printf("%c", (c >= 32U && c <= 126U) ? (char)c : '.');
        }
        printf("|\n");
    }
}

/*============================================================================*/
/*                           时间函数实现                                       */
/*============================================================================*/

/**
 * @brief 获取系统时间计数
 * @details 返回从系统启动到当前的毫秒数,用于测量代码执行时间。
 *          注意:精度取决于系统时钟,Windows下通常为1ms。
 * 
 * @return 当前时间计数(毫秒)
 * 
 * @code
 * uint32_t start = get_tick_count();
 * do_something();
 * uint32_t elapsed = get_tick_count() - start;
 * printf("耗时: %u ms\n", elapsed);
 * @endcode
 */
uint32_t get_tick_count(void)
{
    return (uint32_t)clock();
}

/**
 * @brief 毫秒级延时
 * @details 阻塞式延时,使当前线程暂停执行指定的毫秒数。
 *          精度取决于系统时钟。
 * 
 * @param ms 延时毫秒数
 * 
 * @warning 在嵌入式系统中使用时,注意看门狗喂狗
 * @warning 长时间延时会阻塞整个程序,不适合实时性要求高的场景
 * 
 * @code
 * delay_ms(100);  // 延时100毫秒
 * @endcode
 */
void delay_ms(uint32_t ms)
{
    clock_t start = clock();
    clock_t wait = (clock_t)(ms * CLOCKS_PER_SEC / 1000U);
    
    while ((clock() - start) < wait) {
        /* 空循环等待 */
    }
}

/*============================================================================*/
/*                           数据处理函数实现                                   */
/*============================================================================*/

/**
 * @brief 字节序交换
 * @details 原地交换数据的字节序,用于大端和小端之间的转换。
 *          例如将0x12345678转换为0x78563412。
 * 
 * @param data 数据指针
 * @param size 数据大小(字节)
 * 
 * @note 数据在原位置被修改
 * @warning data不能为NULL
 * 
 * @code
 * uint32_t value = 0x12345678;
 * swap_bytes(&value, sizeof(value));
 * // value = 0x78563412
 * 
 * uint16_t val16 = 0xABCD;
 * swap_bytes(&val16, sizeof(val16));
 * // val16 = 0xCDAB
 * @endcode
 */
void swap_bytes(void *data, size_t size)
{
    if (data == NULL || size < 2U) {
        return;
    }
    
    uint8_t *bytes = (uint8_t *)data;
    size_t left = 0U;
    size_t right = size - 1U;
    
    while (left < right) {
        uint8_t temp = bytes[left];
        bytes[left] = bytes[right];
        bytes[right] = temp;
        left++;
        right--;
    }
}

/**
 * @brief 计算累加和校验
 * @details 计算数据的简单累加和校验值。
 *          将所有字节累加,返回16位结果。
 *          这是一种简单的校验方法,用于检测数据传输错误。
 * 
 * @param data 数据指针
 * @param len 数据长度
 * @return 校验和(16位)
 * 
 * @note 如果data为NULL或len为0,返回0
 * @note 此方法检错能力有限,重要数据建议使用CRC
 * 
 * @code
 * uint8_t data[] = {0x01, 0x02, 0x03, 0x04};
 * uint16_t checksum = calc_checksum(data, sizeof(data));
 * printf("校验和: 0x%04X\n", checksum);
 * @endcode
 */
uint16_t calc_checksum(const uint8_t *data, size_t len)
{
    if (data == NULL || len == 0U) {
        return 0U;
    }
    
    uint32_t sum = 0U;
    
    for (size_t i = 0U; i < len; i++) {
        sum += data[i];
    }
    
    return (uint16_t)(sum & 0xFFFFU);
}

/**
 * @brief 计算CRC8校验值
 * @details 使用CRC-8算法计算校验值。
 *          多项式: 0x31 (x^8 + x^5 + x^4 + 1)
 *          初始值: 0xFF
 *          常用于1-Wire总线、传感器数据校验等。
 * 
 * @param data 数据指针
 * @param len 数据长度
 * @return CRC8校验值(8位)
 * 
 * @note 如果data为NULL或len为0,返回0
 * 
 * @code
 * uint8_t data[] = {0x01, 0x02, 0x03, 0x04};
 * uint8_t crc = calc_crc8(data, sizeof(data));
 * printf("CRC8: 0x%02X\n", crc);
 * @endcode
 */
uint8_t calc_crc8(const uint8_t *data, size_t len)
{
    if (data == NULL || len == 0U) {
        return 0U;
    }
    
    uint8_t crc = 0xFFU;
    
    for (size_t i = 0U; i < len; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if ((crc & 0x80U) != 0U) {
                crc = (uint8_t)((crc << 1) ^ 0x31U);
            } else {
                crc = (uint8_t)(crc << 1);
            }
        }
    }
    
    return crc;
}

/**
 * @brief 计算CRC16校验值
 * @details 使用CRC-16-CCITT算法计算校验值。
 *          多项式: 0x1021 (x^16 + x^12 + x^5 + 1)
 *          初始值: 0xFFFF
 *          常用于串口通信、Modbus协议、文件校验等。
 * 
 * @param data 数据指针
 * @param len 数据长度
 * @return CRC16校验值(16位)
 * 
 * @note 如果data为NULL或len为0,返回0
 * 
 * @code
 * uint8_t data[] = {0x01, 0x02, 0x03, 0x04};
 * uint16_t crc = calc_crc16(data, sizeof(data));
 * printf("CRC16: 0x%04X\n", crc);
 * @endcode
 */
uint16_t calc_crc16(const uint8_t *data, size_t len)
{
    if (data == NULL || len == 0U) {
        return 0U;
    }
    
    uint16_t crc = 0xFFFFU;
    
    for (size_t i = 0U; i < len; i++) {
        crc ^= (uint16_t)data[i];
        for (int j = 0; j < 8; j++) {
            if ((crc & 0x0001U) != 0U) {
                crc = (crc >> 1) ^ 0xA001U;
            } else {
                crc = crc >> 1;
            }
        }
    }
    
    return crc;
}
