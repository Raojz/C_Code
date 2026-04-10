/**
 * @file error_debug.c
 * @brief 错误处理与调试模块实现
 * @details 演示C语言的错误处理和调试技术。
 *          本模块涵盖以下核心知识点：
 *          - 错误码定义与返回
 *          - 错误处理策略
 *          - 断言机制
 *          - 日志系统
 *          - 调试技巧
 *          - 防御性编程
 * 
 * @section embedded_importance 嵌入式开发重要性
 * - 嵌入式系统需要健壮的错误处理
 * - 调试手段有限，需要完善的日志系统
 * - 防御性编程提高系统可靠性
 * - 错误恢复机制保证系统稳定
 * 
 * @author Embedded C Learning Project
 * @date 2026-03-22
 * @version 1.0.0
 * 
 * @note 本文件遵循 MISRA-C:2012 编码规范
 */

#include "error_debug.h"
#include "common/common_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*============================================================================*/
/*                           错误码处理演示                                     */
/*============================================================================*/

/**
 * @brief 错误码处理演示
 * @details 展示错误码的定义和使用：
 *          - 错误码枚举定义
 *          - 错误码返回
 *          - 错误码检查
 *          - 错误信息获取
 * 
 * @note 错误码应覆盖所有可能的错误情况
 * @note 错误码应有清晰的文档说明
 */
void demo_error_code(void)
{
    print_separator("错误码处理演示");

    printf("[错误码定义]\n");
    printf("  typedef enum {\n");
    printf("    ERR_OK = 0,\n");
    printf("    ERR_INVALID_PARAM,\n");
    printf("    ERR_NULL_POINTER,\n");
    printf("    ERR_OUT_OF_MEMORY,\n");
    printf("    ERR_TIMEOUT,\n");
    printf("    ERR_NOT_FOUND\n");
    printf("  } ErrorCode_t;\n");

    printf("\n[错误码使用]\n");
    int32_t div_result = 0;
    ErrorCode_t result = divide_safe(10, 2, &div_result);
    if (result == ERR_OK) {
        printf("  操作成功: 10 / 2 = %d\n", div_result);
    }

    result = divide_safe(10, 0, &div_result);
    if (result != ERR_OK) {
        printf("  操作失败: %s\n", error_to_string(result));
    }

    printf("\n[错误处理模式]\n");
    printf("  1. 返回值检查: if (func() != ERR_OK) { ... }\n");
    printf("  2. 错误传播: return error_code;\n");
    printf("  3. 错误恢复: 重试、回滚、降级\n");
}

/**
 * @brief 安全除法函数
 */
ErrorCode_t divide_safe(int32_t a, int32_t b, int32_t *result)
{
    if (result == NULL) {
        return ERR_NULL_POINTER;
    }
    if (b == 0) {
        return ERR_INVALID_PARAM;
    }
    *result = a / b;
    return ERR_OK;
}

/**
 * @brief 错误码转字符串
 */
const char *error_to_string(ErrorCode_t err)
{
    switch (err) {
        case ERR_OK:           return "成功";
        case ERR_INVALID_PARAM: return "无效参数";
        case ERR_NULL_POINTER:  return "空指针";
        case ERR_OUT_OF_MEMORY: return "内存不足";
        case ERR_TIMEOUT:       return "超时";
        case ERR_NOT_FOUND:     return "未找到";
        default:               return "未知错误";
    }
}

/*============================================================================*/
/*                           断言机制演示                                       */
/*============================================================================*/

/**
 * @brief 断言机制演示
 * @details 展示断言的使用：
 *          - assert宏
 *          - 编译时断言
 *          - 运行时断言
 *          - 断言与错误处理的区别
 * 
 * @note 断言用于调试阶段发现逻辑错误
 * @note 生产代码中可能禁用断言
 */
void demo_assertion(void)
{
    print_separator("断言机制演示");

    printf("[assert宏]\n");
    printf("  #include <assert.h>\n");
    printf("  assert(expression);\n");
    printf("  如果expression为假, 程序终止并打印错误信息\n");

    printf("\n[断言示例]\n");
    int32_t *ptr = (int32_t *)malloc(sizeof(int32_t));
    assert(ptr != NULL && "内存分配失败");
    printf("  断言通过: ptr != NULL\n");
    free(ptr);

    printf("\n[编译时断言]\n");
    printf("  STATIC_ASSERT(sizeof(int) == 4, int_must_be_4_bytes);\n");
    STATIC_ASSERT(sizeof(int32_t) == 4, int32_must_be_4_bytes);
    printf("  编译时断言通过\n");

    printf("\n[断言与错误处理]\n");
    printf("  断言: 用于发现编程错误, 调试阶段\n");
    printf("  错误处理: 用于处理运行时错误, 生产环境\n");
    printf("  断言不应替代错误处理\n");
}

/*============================================================================*/
/*                           日志系统演示                                       */
/*============================================================================*/

/**
 * @brief 日志系统演示
 * @details 展示日志系统的设计和使用：
 *          - 日志级别
 *          - 日志格式
 *          - 日志输出
 *          - 条件编译
 * 
 * @note 日志是嵌入式调试的重要手段
 * @note 生产环境应控制日志输出
 */
void demo_logging(void)
{
    print_separator("日志系统演示");

    printf("[日志级别]\n");
    printf("  LOG_ERROR: 错误信息, 必须输出\n");
    printf("  LOG_WARN:  警告信息, 建议输出\n");
    printf("  LOG_INFO:  一般信息, 可选输出\n");
    printf("  LOG_DEBUG: 调试信息, 调试时输出\n");

    printf("\n[日志输出]\n");
    LOG_ERROR("这是一个错误信息");
    LOG_WARN("这是一个警告信息");
    LOG_INFO("这是一个一般信息");
    LOG_DEBUG("这是一个调试信息");

    printf("\n[日志格式]\n");
    printf("  [时间] [级别] [文件:行号] 消息\n");
    printf("  [2026-03-22 10:30:45] [ERROR] [main.c:123] 系统初始化失败\n");

    printf("\n[嵌入式日志]\n");
    printf("  - 串口输出\n");
    printf("  - 文件记录\n");
    printf("  - 环形缓冲区\n");
    printf("  - 远程日志\n");
}

/*============================================================================*/
/*                           防御性编程演示                                     */
/*============================================================================*/

/**
 * @brief 防御性编程演示
 * @details 展示防御性编程技巧：
 *          - 参数验证
 *          - 边界检查
 *          - 资源管理
 *          - 错误恢复
 * 
 * @note 防御性编程提高代码健壮性
 * @note 嵌入式系统尤其需要防御性编程
 */
void demo_defensive_programming(void)
{
    print_separator("防御性编程演示");

    printf("[参数验证]\n");
    printf("  if (ptr == NULL) { return ERR_NULL_POINTER; }\n");
    printf("  if (size == 0) { return ERR_INVALID_PARAM; }\n");
    printf("  if (index >= max) { return ERR_OUT_OF_RANGE; }\n");

    printf("\n[边界检查]\n");
    printf("  if (index < ARRAY_SIZE(arr)) { arr[index] = value; }\n");
    printf("  if (len < sizeof(buffer)) { memcpy(buffer, src, len); }\n");

    printf("\n[资源管理]\n");
    printf("  - 成对使用malloc/free\n");
    printf("  - 使用后立即置NULL\n");
    printf("  - 检查返回值\n");
    printf("  - 超时机制\n");

    printf("\n[错误恢复]\n");
    printf("  - 重试机制\n");
    printf("  - 回滚操作\n");
    printf("  - 降级服务\n");
    printf("  - 安全模式\n");

    printf("\n[防御性编程原则]\n");
    printf("  1. 不信任外部输入\n");
    printf("  2. 检查所有返回值\n");
    printf("  3. 验证所有参数\n");
    printf("  4. 处理所有可能的错误\n");
    printf("  5. 资源必须释放\n");
}

/*============================================================================*/
/*                           调试技巧演示                                       */
/*============================================================================*/

/**
 * @brief 调试技巧演示
 * @details 展示常用的调试技巧：
 *          - 打印调试
 *          - 条件断点
 *          - 内存检查
 *          - 性能分析
 * 
 * @note 调试是开发过程的重要环节
 * @note 掌握多种调试技巧提高开发效率
 */
void demo_debug_techniques(void)
{
    print_separator("调试技巧演示");

    printf("[打印调试]\n");
    printf("  printf(\"变量值: %d\\n\", value);\n");
    printf("  printf(\"指针地址: %p\\n\", (void*)ptr);\n");
    printf("  printf(\"函数进入: %s\\n\", __func__);\n");

    printf("\n[调试宏]\n");
    printf("  DEBUG_PRINT(\"value = %d\", value);\n");
    printf("  TRACE_ENTER();\n");
    printf("  TRACE_EXIT();\n");

    printf("\n[内存检查]\n");
    printf("  - 检查空指针\n");
    printf("  - 检查数组越界\n");
    printf("  - 检查内存泄漏\n");
    printf("  - 检查野指针\n");

    printf("\n[性能分析]\n");
    printf("  - 时间测量\n");
    printf("  - 函数调用次数\n");
    printf("  - 内存使用量\n");
    printf("  - CPU占用率\n");

    printf("\n[嵌入式调试]\n");
    printf("  - JTAG/SWD调试\n");
    printf("  - 串口调试\n");
    printf("  - LED指示\n");
    printf("  - 示波器/逻辑分析仪\n");
}

/*============================================================================*/
/*                           错误处理模式演示                                   */
/*============================================================================*/

/**
 * @brief 错误处理模式演示
 * @details 展示常见的错误处理模式：
 *          - 返回值模式
 *          - 错误码模式
 *          - 异常处理模式（C语言模拟）
 *          - 资源清理模式
 * 
 * @note 选择合适的错误处理模式
 * @note 保持错误处理的一致性
 */
void demo_error_handling_patterns(void)
{
    print_separator("错误处理模式演示");

    printf("[返回值模式]\n");
    printf("  int32_t result = func();\n");
    printf("  if (result < 0) { /* 错误处理 */ }\n");

    printf("\n[错误码模式]\n");
    printf("  ErrorCode_t err = func();\n");
    printf("  if (err != ERR_OK) { /* 错误处理 */ }\n");

    printf("\n[资源清理模式]\n");
    printf("  Resource_t *r1 = NULL, *r2 = NULL;\n");
    printf("  r1 = allocate_resource1();\n");
    printf("  if (r1 == NULL) goto cleanup;\n");
    printf("  r2 = allocate_resource2();\n");
    printf("  if (r2 == NULL) goto cleanup;\n");
    printf("  // 使用资源\n");
    printf("cleanup:\n");
    printf("  if (r2) free_resource2(r2);\n");
    printf("  if (r1) free_resource1(r1);\n");

    printf("\n[最佳实践]\n");
    printf("  1. 统一错误处理方式\n");
    printf("  2. 错误信息要详细\n");
    printf("  3. 错误处理要及时\n");
    printf("  4. 资源清理要完整\n");
}
