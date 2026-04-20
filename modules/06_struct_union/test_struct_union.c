/**
 * @file test_struct_union.c
 * @brief 结构体与联合体模块测试
 */

#include "test_framework.h"
#include "struct_union.h"
#include <stdio.h>
#include <string.h>

typedef struct { int32_t x, y; } TestPoint_t;
typedef union { uint32_t value; uint8_t bytes[4]; } TestWordUnion_t;

static void test_struct_basics(void)
{
    printf("    [测试] 结构体基础...\n");

    TestPoint_t p = {10, 20};
    TestPoint_t *ptr = &p;
    bool pass = (p.x == 10 && p.y == 20) && (ptr->x == 10);

    if (pass) {
        printf("    [通过] 结构体基础正确\n");
        test_pass();
    } else {
        printf("    [失败] 结构体成员访问错误\n");
        test_fail();
    }
}

static void test_struct_copy(void)
{
    printf("    [测试] 结构体拷贝...\n");

    TestPoint_t p1 = {100, 200}, p2;
    p2 = p1;

    if (p2.x == 100 && p2.y == 200) {
        printf("    [通过] 结构体拷贝正确\n");
        test_pass();
    } else {
        printf("    [失败] 结构体拷贝错误\n");
        test_fail();
    }
}

static void test_union_basics(void)
{
    printf("    [测试] 联合体基础...\n");

    TestWordUnion_t u;
    u.value = 0x12345678;

    if (u.bytes[0] == 0x78 || u.bytes[0] == 0x12) {
        printf("    [通过] 联合体基础正确\n");
        test_pass();
    } else {
        printf("    [失败] 联合体字节访问错误\n");
        test_fail();
    }
}

static void test_enum_basics(void)
{
    printf("    [测试] 枚举基础...\n");

    typedef enum { COLOR_RED = 0, COLOR_GREEN = 1, COLOR_BLUE = 2 } Color_t;
    Color_t c = COLOR_GREEN;

    if (c == 1) {
        printf("    [通过] 枚举基础正确\n");
        test_pass();
    } else {
        printf("    [失败] 枚举值错误\n");
        test_fail();
    }
}

static void test_struct_array(void)
{
    printf("    [测试] 结构体数组...\n");

    TestPoint_t points[3] = {{0, 0}, {1, 1}, {2, 2}};
    int32_t sum_x = 0;
    for (int32_t i = 0; i < 3; i++) sum_x += points[i].x;

    if (sum_x == 3) {
        printf("    [通过] 结构体数组正确\n");
        test_pass();
    } else {
        printf("    [失败] 结构体数组访问错误\n");
        test_fail();
    }
}

static void test_nested_struct(void)
{
    printf("    [测试] 嵌套结构体...\n");

    typedef struct { TestPoint_t start, end; } TestLine_t;
    TestLine_t line = {{0, 0}, {10, 10}};

    if (line.end.x == 10) {
        printf("    [通过] 嵌套结构体正确\n");
        test_pass();
    } else {
        printf("    [失败] 嵌套结构体访问错误\n");
        test_fail();
    }
}

int32_t test_struct_union(void)
{
    test_struct_basics();
    test_struct_copy();
    test_union_basics();
    test_enum_basics();
    test_struct_array();
    test_nested_struct();

    TestStats_t stats = test_get_stats();
    return stats.failed;
}
