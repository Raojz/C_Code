/**
 * @file test_bit_operation.c
 * @brief 位运算模块测试
 */

#include "test_framework.h"
#include "bit_operation.h"
#include <stdio.h>

static void test_bitwise_and_or(void)
{
    printf("    [测试] 位与和位或...\n");

    uint32_t a = 0xAA, b = 0x55;
    bool pass = ((a & b) == 0x00) && ((a | b) == 0xFF);

    if (pass) {
        printf("    [通过] 位与和位或正确\n");
        test_pass();
    } else {
        printf("    [失败] 位与或运算错误\n");
        test_fail();
    }
}

static void test_bitwise_xor(void)
{
    printf("    [测试] 位异或...\n");

    uint32_t a = 0xAA, b = 0x55;
    bool pass = ((a ^ b) == 0xFF) && ((a ^ a) == 0x00);

    if (pass) {
        printf("    [通过] 位异或正确\n");
        test_pass();
    } else {
        printf("    [失败] 位异或运算错误\n");
        test_fail();
    }
}

static void test_bit_shift(void)
{
    printf("    [测试] 位移操作...\n");

    uint32_t value = 0x01;
    bool pass = ((value << 4) == 0x10);
    value = 0x80;
    pass = pass && ((value >> 4) == 0x08);

    if (pass) {
        printf("    [通过] 位移操作正确\n");
        test_pass();
    } else {
        printf("    [失败] 位移运算错误\n");
        test_fail();
    }
}

static void test_bit_count(void)
{
    printf("    [测试] 位计数...\n");

    uint32_t value = 0b1111000011110000;
    int32_t count = 0;
    while (value != 0U) {
        count += (int32_t)(value & 1U);
        value >>= 1;
    }

    if (count == 8) {
        printf("    [通过] 位计数正确\n");
        test_pass();
    } else {
        printf("    [失败] 位计数错误\n");
        test_fail();
    }
}

static void test_bit_tricks(void)
{
    printf("    [测试] 位操作技巧...\n");

    bool pass = ((7 & 1) == 1) && ((8 & 1) == 0);

    int32_t a = 10, b = 20;
    a ^= b; b ^= a; a ^= b;
    pass = pass && (a == 20 && b == 10);

    int32_t n = 16;
    bool is_power = (n > 0) && ((n & (n - 1)) == 0);
    pass = pass && is_power;

    if (pass) {
        printf("    [通过] 位操作技巧正确\n");
        test_pass();
    } else {
        printf("    [失败] 位操作技巧错误\n");
        test_fail();
    }
}

static void test_bitmask_operations(void)
{
    printf("    [测试] 位掩码操作...\n");

    uint32_t reg = 0x00;
    BIT_SET(reg, 3);
    bool pass = (reg == 0x08);

    reg |= 0xF0;
    pass = pass && (reg == 0xF8);

    BIT_CLEAR(reg, 7);
    pass = pass && (reg == 0x78);

    reg = 0xABCD;
    uint32_t field = BIT_FIELD(reg, 4, 8);
    pass = pass && (field == 0xBC);

    if (pass) {
        printf("    [通过] 位掩码操作正确\n");
        test_pass();
    } else {
        printf("    [失败] 位掩码操作错误\n");
        test_fail();
    }
}

int32_t test_bit_operation(void)
{
    test_bitwise_and_or();
    test_bitwise_xor();
    test_bit_shift();
    test_bit_count();
    test_bit_tricks();
    test_bitmask_operations();

    TestStats_t stats = test_get_stats();
    return stats.failed;
}
