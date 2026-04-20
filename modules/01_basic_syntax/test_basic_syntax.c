/**
 * @file test_basic_syntax.c
 * @brief 基础语法模块测试
 */

#include "test_framework.h"
#include "basic_syntax.h"
#include <stdio.h>

/*============================================================================*/
/*                           测试用例                                           */
/*============================================================================*/

static void test_fixed_width_types(void)
{
    printf("    [测试] 固定宽度类型大小...\n");

    bool pass = true;
    pass = pass && (sizeof(uint8_t) == 1U);
    pass = pass && (sizeof(uint16_t) == 2U);
    pass = pass && (sizeof(uint32_t) == 4U);
    pass = pass && (sizeof(int32_t) == 4U);

    if (pass) {
        printf("    [通过] 固定宽度类型大小正确\n");
        test_pass();
    } else {
        printf("    [失败] 固定宽度类型大小错误\n");
        test_fail();
    }
}

static void test_arithmetic_operators(void)
{
    printf("    [测试] 算术运算符...\n");

    int32_t a = 10, b = 3;
    bool pass = true;

    pass = pass && ((a + b) == 13);
    pass = pass && ((a - b) == 7);
    pass = pass && ((a * b) == 30);
    pass = pass && ((a / b) == 3);
    pass = pass && ((a % b) == 1);

    if (pass) {
        printf("    [通过] 算术运算符正确\n");
        test_pass();
    } else {
        printf("    [失败] 算术运算符错误\n");
        test_fail();
    }
}

static void test_bitwise_operators(void)
{
    printf("    [测试] 位运算符...\n");

    uint32_t a = 0x0F, b = 0xF0;//0000 1111,1111 0000
    bool pass = true;

    pass = pass && ((a & b) == 0x00);//同为1为1，不同为0。0000 0000
    pass = pass && ((a | b) == 0xFF);//有1为1，否则为0。1111 1111
    pass = pass && ((a ^ b) == 0xFF);//相同为0，不同为1。1111 1111
    pass = pass && ((a << 4) == 0xF0);//左移4位，右边补0。1111 0000
    pass = pass && ((b >> 4) == 0x0F);//右移4位，左边补0。0000 1111

    if (pass) {
        printf("    [通过] 位运算符正确\n");
        test_pass();
    } else {
        printf("    [失败] 位运算符错误\n");
        test_fail();
    }
}

static void test_macros(void)
{
    printf("    [测试] 公共宏定义...\n");

    bool pass = true;

    pass = pass && (MIN(10, 20) == 10);
    pass = pass && (MAX(10, 20) == 20);
    pass = pass && (ABS(-5) == 5);
    pass = pass && (ABS(5) == 5);
    pass = pass && (CLAMP(50, 0, 100) == 50);
    pass = pass && (CLAMP(-10, 0, 100) == 0);
    pass = pass && (CLAMP(150, 0, 100) == 100);

    int32_t arr[10];
    pass = pass && (ARRAY_SIZE(arr) == 10U);

    if (pass) {
        printf("    [通过] 公共宏定义正确\n");
        test_pass();
    } else {
        printf("    [失败] 公共宏定义错误\n");
        test_fail();
    }
}

static void test_byte_macros(void)
{
    printf("    [测试] 字节操作宏...\n");

    bool pass = true;

    pass = pass && (LOW_BYTE(0x1234) == 0x34);
    pass = pass && (HIGH_BYTE(0x1234) == 0x12);
    pass = pass && (MAKE_WORD(0x12, 0x34) == 0x1234);
    pass = pass && (LOW_WORD(0x12345678) == 0x5678);
    pass = pass && (HIGH_WORD(0x12345678) == 0x1234);
    pass = pass && (MAKE_DWORD(0x1234, 0x5678) == 0x12345678);

    if (pass) {
        printf("    [通过] 字节操作宏正确\n");
        test_pass();
    } else {
        printf("    [失败] 字节操作宏错误\n");
        test_fail();
    }
}

static void test_bit_macros(void)
{
    printf("    [测试] 位操作宏...\n");

    uint32_t reg = 0x00;

    BIT_SET(reg, 3);
    bool pass = (reg == 0x08);

    BIT_SET(reg, 0);
    pass = pass && (reg == 0x09);

    BIT_CLEAR(reg, 3);
    pass = pass && (reg == 0x01);

    BIT_TOGGLE(reg, 1);
    pass = pass && (reg == 0x03);

    pass = pass && BIT_CHECK(reg, 0);
    pass = pass && !BIT_CHECK(reg, 7);
    pass = pass && (BIT_MASK(4) == 0x0FU);

    if (pass) {
        printf("    [通过] 位操作宏正确\n");
        test_pass();
    } else {
        printf("    [失败] 位操作宏错误\n");
        test_fail();
    }
}

/*============================================================================*/
/*                           模块测试入口                                       */
/*============================================================================*/

int32_t test_basic_syntax(void)
{
    test_fixed_width_types();
    test_arithmetic_operators();
    test_bitwise_operators();
    test_macros();
    test_byte_macros();
    test_bit_macros();

    TestStats_t stats = test_get_stats();
    return stats.failed;
}
