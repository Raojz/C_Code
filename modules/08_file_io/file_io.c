/**
 * @file file_io.c
 * @brief 文件输入输出模块实现
 * @details 演示C语言文件操作的标准库函数。
 *          本模块涵盖以下核心知识点：
 *          - 文件打开与关闭
 *          - 文件读写操作
 *          - 文件定位
 *          - 错误处理
 *          - 二进制文件操作
 *          - 文件系统操作
 * 
 * @section embedded_importance 嵌入式开发重要性
 * - 配置文件读写
 * - 日志文件记录
 * - 固件升级文件处理
 * - 数据持久化存储
 * 
 * @author Embedded C Learning Project
 * @date 2026-03-22
 * @version 1.0.0
 * 
 * @note 本文件遵循 MISRA-C:2012 编码规范
 */

#include "file_io.h"
#include "common/common_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*============================================================================*/
/*                           文件基础操作演示                                   */
/*============================================================================*/

/**
 * @brief 文件基础操作演示
 * @details 展示文件的基本操作：
 *          - fopen：打开文件
 *          - fclose：关闭文件
 *          - 文件打开模式
 *          - 错误检查
 * 
 * @note 文件操作必须检查返回值
 * @note 打开文件后必须关闭
 */
void demo_file_basics(void)
{
    print_separator("文件基础操作演示");

    printf("[文件打开模式]\n");
    printf("  \"r\"  - 只读, 文件必须存在\n");
    printf("  \"w\"  - 只写, 文件不存在则创建, 存在则清空\n");
    printf("  \"a\"  - 追加, 文件不存在则创建\n");
    printf("  \"r+\" - 读写, 文件必须存在\n");
    printf("  \"w+\" - 读写, 文件不存在则创建, 存在则清空\n");
    printf("  \"a+\" - 读写追加, 文件不存在则创建\n");
    printf("  \"b\"  - 二进制模式 (如 \"rb\", \"wb\")\n");

    printf("\n[文件打开与关闭]\n");
    FILE *file = fopen("test.txt", "w");
    if (file == NULL) {
        printf("  文件打开失败!\n");
        return;
    }
    
    printf("  文件打开成功\n");
    fclose(file);
    printf("  文件已关闭\n");

    printf("\n[错误检查]\n");
    printf("  if (file == NULL) { /* 错误处理 */ }\n");
    printf("  if (ferror(file)) { /* 检查错误 */ }\n");
    printf("  if (feof(file)) { /* 检查文件结束 */ }\n");
}

/*============================================================================*/
/*                           文件读写演示                                       */
/*============================================================================*/

/**
 * @brief 文件读写演示
 * @details 展示文件的读写操作：
 *          - fgetc/fputc：字符读写
 *          - fgets/fputs：字符串读写
 *          - fscanf/fprintf：格式化读写
 *          - fread/fwrite：二进制读写
 * 
 * @note 读写操作需检查返回值
 * @note 二进制读写需使用"rb"/"wb"模式
 */
void demo_file_read_write(void)
{
    print_separator("文件读写演示");

    printf("[写入文件]\n");
    FILE *file = fopen("demo.txt", "w");
    if (file != NULL) {
        fprintf(file, "Hello, File I/O!\n");
        fprintf(file, "数字: %d, 浮点: %.2f\n", 42, 3.14);
        fputs("这是最后一行\n", file);
        fclose(file);
        printf("  文件写入成功\n");
    }

    printf("\n[读取文件]\n");
    file = fopen("demo.txt", "r");
    if (file != NULL) {
        char buffer[256];
        printf("  文件内容:\n");
        while (fgets(buffer, sizeof(buffer), file) != NULL) {
            printf("    %s", buffer);
        }
        fclose(file);
    }

    printf("\n[二进制文件操作]\n");
    int32_t data[] = {1, 2, 3, 4, 5};
    file = fopen("binary.dat", "wb");
    if (file != NULL) {
        size_t written = fwrite(data, sizeof(int32_t), 5, file);
        printf("  写入 %zu 个整数\n", written);
        fclose(file);
    }

    int32_t read_data[5];
    file = fopen("binary.dat", "rb");
    if (file != NULL) {
        size_t read_count = fread(read_data, sizeof(int32_t), 5, file);
        printf("  读取 %zu 个整数: ", read_count);
        for (size_t i = 0; i < read_count; i++) {
            printf("%d ", read_data[i]);
        }
        printf("\n");
        fclose(file);
    }
}

/*============================================================================*/
/*                           文件定位演示                                       */
/*============================================================================*/

/**
 * @brief 文件定位演示
 * @details 展示文件定位操作：
 *          - fseek：移动文件指针
 *          - ftell：获取当前位置
 *          - rewind：回到文件开头
 *          - 文件大小获取
 * 
 * @note 二进制文件定位需注意字节对齐
 * @note 文本文件定位可能因换行符转换而不准确
 */
void demo_file_positioning(void)
{
    print_separator("文件定位演示");

    printf("[文件定位函数]\n");
    printf("  fseek(file, offset, origin):\n");
    printf("    SEEK_SET: 文件开头\n");
    printf("    SEEK_CUR: 当前位置\n");
    printf("    SEEK_END: 文件末尾\n");
    printf("  ftell(file): 返回当前位置\n");
    printf("  rewind(file): 回到文件开头\n");

    printf("\n[获取文件大小]\n");
    FILE *file = fopen("demo.txt", "rb");
    if (file != NULL) {
        fseek(file, 0, SEEK_END);
        long size = ftell(file);
        printf("  文件大小: %ld 字节\n", size);
        fclose(file);
    }

    printf("\n[随机访问]\n");
    file = fopen("binary.dat", "rb");
    if (file != NULL) {
        fseek(file, 2 * sizeof(int32_t), SEEK_SET);
        int32_t value;
        fread(&value, sizeof(int32_t), 1, file);
        printf("  第3个整数: %d\n", value);
        fclose(file);
    }
}

/*============================================================================*/
/*                           错误处理演示                                       */
/*============================================================================*/

/**
 * @brief 文件错误处理演示
 * @details 展示文件操作的错误处理：
 *          - ferror：检查错误标志
 *          - feof：检查文件结束
 *          - perror：打印错误信息
 *          - 错误恢复
 * 
 * @note 文件操作必须进行错误检查
 * @note 错误处理后应清除错误标志
 */
void demo_file_error_handling(void)
{
    print_separator("文件错误处理演示");

    printf("[错误检查函数]\n");
    printf("  ferror(file): 检查是否发生错误\n");
    printf("  feof(file): 检查是否到达文件末尾\n");
    printf("  clearerr(file): 清除错误和EOF标志\n");
    printf("  perror(msg): 打印错误信息\n");

    printf("\n[错误处理示例]\n");
    FILE *file = fopen("nonexistent.txt", "r");
    if (file == NULL) {
        perror("  打开文件失败");
        printf("  错误码: %d\n", errno);
    }

    printf("\n[最佳实践]\n");
    printf("  1. 检查fopen返回值\n");
    printf("  2. 检查读写函数返回值\n");
    printf("  3. 使用ferror检查错误\n");
    printf("  4. 使用feof判断文件结束\n");
    printf("  5. 错误处理后关闭文件\n");
}

/*============================================================================*/
/*                           配置文件处理演示                                   */
/*============================================================================*/

/**
 * @brief 配置文件处理演示
 * @details 展示配置文件的读写：
 *          - 配置文件格式
 *          - 键值对解析
 *          - 配置文件写入
 * 
 * @note 配置文件常用于嵌入式系统参数存储
 * @note 注意配置文件的格式和解析
 */
void demo_config_file(void)
{
    print_separator("配置文件处理演示");

    printf("[写入配置文件]\n");
    FILE *file = fopen("config.ini", "w");
    if (file != NULL) {
        fprintf(file, "# 系统配置文件\n");
        fprintf(file, "[System]\n");
        fprintf(file, "version=1.0.0\n");
        fprintf(file, "debug=1\n");
        fprintf(file, "\n[Network]\n");
        fprintf(file, "ip=192.168.1.100\n");
        fprintf(file, "port=8080\n");
        fclose(file);
        printf("  配置文件写入成功\n");
    }

    printf("\n[读取配置文件]\n");
    file = fopen("config.ini", "r");
    if (file != NULL) {
        char line[256];
        printf("  配置内容:\n");
        while (fgets(line, sizeof(line), file) != NULL) {
            if (line[0] != '#' && line[0] != '\n') {
                printf("    %s", line);
            }
        }
        fclose(file);
    }

    printf("\n[嵌入式应用]\n");
    printf("  - 系统参数配置\n");
    printf("  - 网络配置\n");
    printf("  - 传感器校准参数\n");
    printf("  - 用户设置\n");
}
