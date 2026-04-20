/**
 * @file test_file_io.c
 * @brief 文件I/O模块测试
 */

#include "test_framework.h"
#include "file_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEST_FILE_NAME "test_temp.txt"

static void test_file_write_read(void)
{
    printf("    [测试] 文件写入和读取...\n");

    FILE *fp = fopen(TEST_FILE_NAME, "w");
    if (fp == NULL) {
        printf("    [失败] 无法创建测试文件\n");
        test_fail();
        return;
    }
    fprintf(fp, "Hello, World!");
    fclose(fp);

    fp = fopen(TEST_FILE_NAME, "r");
    if (fp == NULL) {
        printf("    [失败] 无法打开测试文件\n");
        test_fail();
        return;
    }

    char buffer[32] = {0};
    if (fgets(buffer, sizeof(buffer), fp) == NULL) {
        printf("    [失败] 读取文件失败\n");
        fclose(fp);
        test_fail();
        return;
    }
    fclose(fp);

    if (strncmp(buffer, "Hello, World!", 13) == 0) {
        printf("    [通过] 文件写入和读取正确\n");
        test_pass();
    } else {
        printf("    [失败] 文件内容不匹配\n");
        test_fail();
    }
    remove(TEST_FILE_NAME);
}

static void test_file_binary(void)
{
    printf("    [测试] 二进制文件操作...\n");

    int32_t data[5] = {1, 2, 3, 4, 5};

    FILE *fp = fopen(TEST_FILE_NAME, "wb");
    if (fp == NULL) {
        printf("    [失败] 无法创建二进制文件\n");
        test_fail();
        return;
    }
    size_t written = fwrite(data, sizeof(int32_t), 5, fp);
    fclose(fp);

    if (written != 5) {
        printf("    [失败] 写入数量错误\n");
        test_fail();
        return;
    }

    int32_t read_data[5] = {0};
    fp = fopen(TEST_FILE_NAME, "rb");
    if (fp == NULL) {
        printf("    [失败] 无法打开二进制文件\n");
        test_fail();
        return;
    }
    size_t read_count = fread(read_data, sizeof(int32_t), 5, fp);
    fclose(fp);

    bool pass = (read_count == 5);
    for (int32_t i = 0; i < 5 && pass; i++) {
        pass = pass && (read_data[i] == data[i]);
    }

    if (pass) {
        printf("    [通过] 二进制文件操作正确\n");
        test_pass();
    } else {
        printf("    [失败] 二进制数据不匹配\n");
        test_fail();
    }
    remove(TEST_FILE_NAME);
}

static void test_file_seek(void)
{
    printf("    [测试] 文件定位...\n");

    FILE *fp = fopen(TEST_FILE_NAME, "w+b");
    if (fp == NULL) {
        printf("    [失败] 无法创建文件\n");
        test_fail();
        return;
    }

    const char *text = "ABCDEFGHIJ";
    fwrite(text, 1, 10, fp);

    fseek(fp, 5, SEEK_SET);
    char ch = (char)fgetc(fp);
    bool pass = (ch == 'F');

    fseek(fp, -3, SEEK_END);
    ch = (char)fgetc(fp);
    pass = pass && (ch == 'H');

    fclose(fp);
    remove(TEST_FILE_NAME);

    if (pass) {
        printf("    [通过] 文件定位正确\n");
        test_pass();
    } else {
        printf("    [失败] 文件定位错误\n");
        test_fail();
    }
}

static void test_file_size(void)
{
    printf("    [测试] 获取文件大小...\n");

    FILE *fp = fopen(TEST_FILE_NAME, "wb");
    if (fp == NULL) {
        printf("    [失败] 无法创建文件\n");
        test_fail();
        return;
    }
    const char *text = "1234567890";
    fwrite(text, 1, 10, fp);
    fclose(fp);

    fp = fopen(TEST_FILE_NAME, "rb");
    if (fp == NULL) {
        printf("    [失败] 无法打开文件\n");
        test_fail();
        return;
    }
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fclose(fp);

    if (size == 10) {
        printf("    [通过] 获取文件大小正确\n");
        test_pass();
    } else {
        printf("    [失败] 文件大小错误\n");
        test_fail();
    }
    remove(TEST_FILE_NAME);
}

int32_t test_file_io(void)
{
    test_file_write_read();
    test_file_binary();
    test_file_seek();
    test_file_size();

    TestStats_t stats = test_get_stats();
    return stats.failed;
}
