/**
 * @file error_debug.h
 * @brief 错误处理与调试模块头文件
 * @details 涵盖错误码、断言、日志、防御性编程等知识点。
 *          错误处理和调试是保证软件质量的关键。
 * 
 * @section main_features 主要内容
 * - 错误码：定义、返回、检查、转换
 * - 断言机制：编译时断言、运行时断言
 * - 日志系统：日志级别、格式化输出
 * - 防御性编程：参数验证、边界检查、资源管理
 * - 调试技巧：打印调试、内存检查、性能分析
 * - 错误处理模式：返回值、错误码、资源清理
 * 
 * @section embedded_importance 嵌入式开发重要性
 * - 嵌入式系统需要健壮的错误处理
 * - 调试手段有限，需要完善的日志系统
 * - 防御性编程提高系统可靠性
 * - 错误恢复机制保证系统稳定
 * 
 * @section best_practices 最佳实践
 * - 统一错误处理方式
 * - 详细记录错误信息
 * - 及时处理错误
 * - 完整清理资源
 * 
 * @author Embedded C Learning Project
 * @date 2026-03-22
 * @version 1.0.0
 * 
 * @note 本文件遵循 MISRA-C:2012 编码规范
 */

#ifndef ERROR_DEBUG_H
#define ERROR_DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common/common_types.h"

/*============================================================================*/
/*                           错误码定义                                         */
/*============================================================================*/

/**
 * @brief 错误码枚举
 */
typedef enum {
    ERR_OK = 0,             /**< 成功 */
    ERR_INVALID_PARAM,      /**< 无效参数 */
    ERR_NULL_POINTER,       /**< 空指针 */
    ERR_OUT_OF_MEMORY,      /**< 内存不足 */
    ERR_TIMEOUT,            /**< 超时 */
    ERR_NOT_FOUND,          /**< 未找到 */
    ERR_OUT_OF_RANGE,       /**< 超出范围 */
    ERR_BUSY,               /**< 忙碌 */
    ERR_NOT_SUPPORTED,      /**< 不支持 */
    ERR_HARDWARE            /**< 硬件错误 */
} ErrorCode_t;

/*============================================================================*/
/*                           演示函数声明                                       */
/*============================================================================*/

/**
 * @brief 错误码处理演示
 */
void demo_error_code(void);

/**
 * @brief 安全除法函数
 * @param[in] a 被除数
 * @param[in] b 除数
 * @param[out] result 结果
 * @return 错误码
 */
ErrorCode_t divide_safe(int32_t a, int32_t b, int32_t *result);

/**
 * @brief 错误码转字符串
 * @param[in] err 错误码
 * @return 错误信息字符串
 */
const char *error_to_string(ErrorCode_t err);

/**
 * @brief 断言机制演示
 */
void demo_assertion(void);

/**
 * @brief 日志系统演示
 */
void demo_logging(void);

/**
 * @brief 防御性编程演示
 */
void demo_defensive_programming(void);

/**
 * @brief 调试技巧演示
 */
void demo_debug_techniques(void);

/**
 * @brief 错误处理模式演示
 */
void demo_error_handling_patterns(void);

#ifdef __cplusplus
}
#endif

#endif
