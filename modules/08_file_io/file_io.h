/**
 * @file file_io.h
 * @brief 文件输入输出模块头文件
 * @details 涵盖文件打开、读写、定位、错误处理等知识点。
 *          文件操作是数据持久化和系统配置的基础。
 * 
 * @section main_features 主要内容
 * - 文件基础：打开、关闭、模式
 * - 文件读写：字符、字符串、格式化、二进制
 * - 文件定位：fseek、ftell、rewind
 * - 错误处理：ferror、feof、perror
 * - 配置文件：读写配置参数
 * 
 * @section embedded_importance 嵌入式开发重要性
 * - 配置文件读写
 * - 日志文件记录
 * - 固件升级文件处理
 * - 数据持久化存储
 * 
 * @section best_practices 最佳实践
 * - 检查所有文件操作返回值
 * - 打开文件后必须关闭
 * - 使用二进制模式处理二进制文件
 * - 注意文件路径和权限
 * 
 * @author Embedded C Learning Project
 * @date 2026-03-22
 * @version 1.0.0
 * 
 * @note 本文件遵循 MISRA-C:2012 编码规范
 */

#ifndef FILE_IO_H
#define FILE_IO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common/common_types.h"

/*============================================================================*/
/*                           演示函数声明                                       */
/*============================================================================*/

/**
 * @brief 文件基础操作演示
 */
void demo_file_basics(void);

/**
 * @brief 文件读写演示
 */
void demo_file_read_write(void);

/**
 * @brief 文件定位演示
 */
void demo_file_positioning(void);

/**
 * @brief 文件错误处理演示
 */
void demo_file_error_handling(void);

/**
 * @brief 配置文件处理演示
 */
void demo_config_file(void);

#ifdef __cplusplus
}
#endif

#endif
