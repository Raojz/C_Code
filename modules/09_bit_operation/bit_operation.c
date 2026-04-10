/**
 * @file bit_operation.c
 * @brief 位操作模块实现
 * @details 演示C语言位操作的核心技术。
 *          本模块涵盖以下核心知识点：
 *          - 位运算符：&、|、^、~、<<、>>
 *          - 位操作技巧
 *          - 位域操作
 *          - 硬件寄存器操作
 *          - 位掩码应用
 * 
 * @section embedded_importance 嵌入式开发重要性
 * - 硬件寄存器操作是嵌入式开发的核心
 * - 位操作用于控制外设
 * - 位掩码用于数据提取和设置
 * - 位操作是高效的数据处理方式
 * 
 * @author Embedded C Learning Project
 * @date 2026-03-22
 * @version 1.0.0
 * 
 * @note 本文件遵循 MISRA-C:2012 编码规范
 */

#include "bit_operation.h"
#include "common/common_utils.h"
#include <stdio.h>

/*============================================================================*/
/*                           位运算符演示                                       */
/*============================================================================*/

/**
 * @brief 位运算符演示
 * @details 展示位运算符的基本用法：
 *          - 按位与 &
 *          - 按位或 |
 *          - 按位异或 ^
 *          - 按位取反 ~
 *          - 左移 <<
 *          - 右移 >>
 * 
 * @note 位操作是嵌入式开发的核心技能
 * @note 注意有符号数的右移行为
 */
void demo_bit_operators(void)
{
    print_separator("位运算符演示");

    uint8_t a = 0b11001010;
    uint8_t b = 0b10110101;

    printf("[按位运算]\n");
    printf("  a = 0b" BYTE_TO_BINARY_FMT " (0x%02X)\n", BYTE_TO_BINARY(a), a);
    printf("  b = 0b" BYTE_TO_BINARY_FMT " (0x%02X)\n", BYTE_TO_BINARY(b), b);
    printf("  a & b = 0b" BYTE_TO_BINARY_FMT " (按位与)\n", BYTE_TO_BINARY(a & b));
    printf("  a | b = 0b" BYTE_TO_BINARY_FMT " (按位或)\n", BYTE_TO_BINARY(a | b));
    printf("  a ^ b = 0b" BYTE_TO_BINARY_FMT " (按位异或)\n", BYTE_TO_BINARY(a ^ b));
    printf("  ~a    = 0b" BYTE_TO_BINARY_FMT " (按位取反)\n", BYTE_TO_BINARY((uint8_t)(~a)));

    printf("\n[移位运算]\n");
    printf("  a << 2 = 0b" BYTE_TO_BINARY_FMT " (左移2位)\n", BYTE_TO_BINARY((uint8_t)(a << 2)));
    printf("  a >> 2 = 0b" BYTE_TO_BINARY_FMT " (右移2位)\n", BYTE_TO_BINARY((uint8_t)(a >> 2)));
    printf("  左移相当于乘2, 右移相当于除2\n");
}

/*============================================================================*/
/*                           位操作技巧演示                                     */
/*============================================================================*/

/**
 * @brief 位操作技巧演示
 * @details 展示常用的位操作技巧：
 *          - 设置位
 *          - 清除位
 *          - 翻转位
 *          - 检查位
 *          - 提取位域
 * 
 * @note 这些技巧在嵌入式开发中非常常用
 * @note 使用宏定义可以提高代码可读性
 */
void demo_bit_tricks(void)
{
    print_separator("位操作技巧演示");

    uint8_t reg = 0x00;

    printf("[设置位]\n");
    printf("  reg = 0x%02X\n", reg);
    BIT_SET(reg, 3);
    printf("  BIT_SET(reg, 3) -> 0x%02X\n", reg);
    BIT_SET(reg, 5);
    printf("  BIT_SET(reg, 5) -> 0x%02X\n", reg);

    printf("\n[清除位]\n");
    BIT_CLEAR(reg, 3);
    printf("  BIT_CLEAR(reg, 3) -> 0x%02X\n", reg);

    printf("\n[翻转位]\n");
    BIT_TOGGLE(reg, 5);
    printf("  BIT_TOGGLE(reg, 5) -> 0x%02X\n", reg);

    printf("\n[检查位]\n");
    printf("  BIT_CHECK(reg, 5) = %d\n", BIT_CHECK(reg, 5));

    printf("\n[提取位域]\n");
    uint32_t data = 0x12345678;
    printf("  data = 0x%08X\n", data);
    printf("  字节0 = 0x%02X\n", GET_LOW_BYTE0(data));
    printf("  字节1 = 0x%02X\n", GET_LOW_BYTE1(data));
    printf("  字节2 = 0x%02X\n", GET_LOW_BYTE2(data));
    printf("  字节3 = 0x%02X\n", GET_LOW_BYTE3(data));
}

/*============================================================================*/
/*                           硬件寄存器操作演示                                 */
/*============================================================================*/

/**
 * @brief 硬件寄存器操作演示
 * @details 展示硬件寄存器的位操作：
 *          - 寄存器读写
 *          - 位域设置
 *          - 标志位检查
 *          - 寄存器配置
 * 
 * @note 硬件寄存器通常使用volatile修饰
 * @note 注意寄存器的访问宽度
 */
void demo_hardware_register(void)
{
    print_separator("硬件寄存器操作演示");

    printf("[模拟硬件寄存器]\n");
    volatile uint32_t GPIOA_MODER = 0x00;
    volatile uint32_t GPIOA_ODR = 0x00;
    volatile uint32_t GPIOA_IDR = 0x00;

    printf("  配置GPIO模式寄存器\n");
    GPIOA_MODER |= (0x01 << 0);  // PA0输出模式
    GPIOA_MODER |= (0x01 << 2);  // PA1输出模式
    printf("  GPIOA_MODER = 0x%08X\n", GPIOA_MODER);

    printf("\n  设置输出数据寄存器\n");
    GPIOA_ODR |= (1 << 0);  // PA0输出高
    GPIOA_ODR |= (1 << 1);  // PA1输出高
    printf("  GPIOA_ODR = 0x%08X\n", GPIOA_ODR);

    printf("\n  清除输出\n");
    GPIOA_ODR &= ~(1 << 0);  // PA0输出低
    printf("  GPIOA_ODR = 0x%08X\n", GPIOA_ODR);

    printf("\n[嵌入式应用]\n");
    printf("  - GPIO配置: 方向、上下拉、输出模式\n");
    printf("  - 外设配置: 时钟使能、中断使能\n");
    printf("  - 状态检查: 标志位、就绪位\n");
}

/*============================================================================*/
/*                           位掩码应用演示                                     */
/*============================================================================*/

/**
 * @brief 位掩码应用演示
 * @details 展示位掩码的实际应用：
 *          - 数据提取
 *          - 数据打包
 *          - 标志位管理
 *          - 权限控制
 * 
 * @note 位掩码是数据处理的高效方式
 * @note 注意掩码的定义和使用
 */
void demo_bitmask_application(void)
{
    print_separator("位掩码应用演示");

    printf("[数据提取]\n");
    uint32_t packet = 0x12345678;
    uint8_t header = (packet >> 24) & 0xFF;
    uint16_t data = (packet >> 8) & 0xFFFF;
    uint8_t checksum = packet & 0xFF;
    
    printf("  数据包: 0x%08X\n", packet);
    printf("  头部: 0x%02X\n", header);
    printf("  数据: 0x%04X\n", data);
    printf("  校验: 0x%02X\n", checksum);

    printf("\n[数据打包]\n");
    uint32_t packed = ((uint32_t)0x12 << 24) | 
                      ((uint32_t)0x3456 << 8) | 
                      0x78;
    printf("  打包结果: 0x%08X\n", packed);

    printf("\n[标志位管理]\n");
    uint8_t flags = 0;
    flags |= (1 << 0);  // 设置标志0
    flags |= (1 << 2);  // 设置标志2
    flags |= (1 << 4);  // 设置标志4
    printf("  标志位: 0x%02X = 0b" BYTE_TO_BINARY_FMT "\n", flags, BYTE_TO_BINARY(flags));
    
    if (flags & (1 << 2)) {
        printf("  标志2已设置\n");
    }

    printf("\n[权限控制]\n");
    uint8_t permissions = 0;
    permissions |= PERM_READ;
    permissions |= PERM_WRITE;
    printf("  权限: 0x%02X\n", permissions);
    if (permissions & PERM_READ) {
        printf("  具有读权限\n");
    }
    if (permissions & PERM_WRITE) {
        printf("  具有写权限\n");
    }
}

/*============================================================================*/
/*                           位操作算法演示                                     */
/*============================================================================*/

/**
 * @brief 位操作算法演示
 * @details 展示位操作相关算法：
 *          - 位计数
 *          - 位反转
 *          - 判断2的幂
 *          - 交换变量
 * 
 * @note 位操作算法通常具有高效性
 * @note 注意算法的正确性和边界条件
 */
void demo_bit_algorithms(void)
{
    print_separator("位操作算法演示");

    printf("[位计数 - 计算二进制中1的个数]\n");
    uint32_t num = 0x12345678;
    int count = 0;
    uint32_t temp = num;
    while (temp) {
        count += temp & 1;
        temp >>= 1;
    }
    printf("  0x%08X 中有 %d 个1\n", num, count);

    printf("\n[判断2的幂]\n");
    uint32_t test_num = 16;
    if ((test_num & (test_num - 1)) == 0 && test_num != 0) {
        printf("  %u 是2的幂\n", test_num);
    }

    printf("\n[不使用临时变量交换]\n");
    int32_t x = 10, y = 20;
    printf("  交换前: x=%d, y=%d\n", x, y);
    x ^= y;
    y ^= x;
    x ^= y;
    printf("  交换后: x=%d, y=%d\n", x, y);

    printf("\n[位反转]\n");
    uint8_t byte = 0b11001010;
    uint8_t reversed = 0;
    for (int i = 0; i < 8; i++) {
        reversed = (reversed << 1) | ((byte >> i) & 1);
    }
    printf("  原始: 0b" BYTE_TO_BINARY_FMT "\n", BYTE_TO_BINARY(byte));
    printf("  反转: 0b" BYTE_TO_BINARY_FMT "\n", BYTE_TO_BINARY(reversed));
}
