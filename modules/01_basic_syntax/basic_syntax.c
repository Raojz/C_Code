/**
 * @file basic_syntax.c
 * @brief 基础语法模块实现
 * @details 演示C语言基础语法：变量、数据类型、运算符等。
 *          本模块是C语言学习的基础，涵盖以下核心知识点：
 *          - 数据类型：整数、浮点、字符、布尔类型
 *          - 变量作用域：局部、全局、静态变量
 *          - 存储类：auto、static、register、extern
 *          - 常量类型：const、宏定义、枚举
 *          - 运算符：算术、关系、逻辑、位运算、赋值、条件
 *          - 类型转换：隐式转换、显式转换
 *          - sizeof运算符：类型大小、数组大小计算
 *          - 运算符优先级：结合性与求值顺序
 * 
 * @section embedded_notes 嵌入式开发注意事项
 * - 优先使用固定宽度整数类型（int8_t, uint32_t等）
 * - 注意整数溢出和截断风险
 * - 位操作是嵌入式开发的核心技能
 * - sizeof用于计算数组元素个数
 * 
 * @author Embedded C Learning Project
 * @date 2026-03-22
 * @version 1.0.0
 * 
 * @note 本文件遵循 MISRA-C:2012 编码规范
 */

#include "basic_syntax.h"
#include "common/common_utils.h"
#include <stdio.h>
#include <limits.h>
#include <float.h>

/*============================================================================*/
/*                           全局变量定义                                       */
/*============================================================================*/

/** @brief 静态全局变量（仅本文件可见） */
static int32_t s_global_static_var = 100;

/** @brief 全局变量（外部可见） */
int32_t g_global_var = 200;

/*============================================================================*/
/*                           数据类型演示函数                                   */
/*============================================================================*/

/**
 * @brief 数据类型演示
 * @details 展示C语言中的各种数据类型及其特性：
 *          - 整数类型：char, short, int, long, long long
 *          - 固定宽度整数类型：int8_t, uint8_t, int16_t等
 *          - 浮点类型：float, double
 *          - 字符类型：char及其转义字符
 *          - 布尔类型：bool (true/false)
 * 
 * @note 嵌入式开发推荐使用固定宽度整数类型
 * @note float精度约6-7位有效数字，double约15-16位
 */
void demo_data_types(void)
{
    print_separator("数据类型演示");

    printf("\n[整数类型]\n");
    printf("  char:        %zu 字节, 范围: %d ~ %d\n", sizeof(char), CHAR_MIN, CHAR_MAX);
    printf("  short:       %zu 字节, 范围: %d ~ %d\n", sizeof(short), SHRT_MIN, SHRT_MAX);
    printf("  int:         %zu 字节, 范围: %d ~ %d\n", sizeof(int), INT_MIN, INT_MAX);
    printf("  long:        %zu 字节, 范围: %ld ~ %ld\n", sizeof(long), LONG_MIN, LONG_MAX);
    printf("  long long:   %zu 字节, 范围: %lld ~ %lld\n", sizeof(long long), LLONG_MIN, LLONG_MAX);

    printf("\n[固定宽度整数类型 (嵌入式推荐)]\n");
    printf("  int8_t:      %zu 字节, 范围: %d ~ %d\n", sizeof(int8_t), INT8_MIN, INT8_MAX);
    printf("  uint8_t:     %zu 字节, 范围: 0 ~ %u\n", sizeof(uint8_t), UINT8_MAX);
    printf("  int16_t:     %zu 字节, 范围: %d ~ %d\n", sizeof(int16_t), INT16_MIN, INT16_MAX);
    printf("  uint16_t:    %zu 字节, 范围: 0 ~ %u\n", sizeof(uint16_t), UINT16_MAX);
    printf("  int32_t:     %zu 字节, 范围: %d ~ %d\n", sizeof(int32_t), INT32_MIN, INT32_MAX);
    printf("  uint32_t:    %zu 字节, 范围: 0 ~ %u\n", sizeof(uint32_t), UINT32_MAX);

    printf("\n[浮点类型]\n");
    printf("  float:       %zu 字节, 精度: %d 位, 范围: %e ~ %e\n", 
           sizeof(float), FLT_DIG, FLT_MIN, FLT_MAX);
    printf("  double:      %zu 字节, 精度: %d 位, 范围: %e ~ %e\n", 
           sizeof(double), DBL_DIG, DBL_MIN, DBL_MAX);

    printf("\n[字符类型]\n");
    char c = 'A';
    printf("  char c = 'A': 值=%c, ASCII=%d\n", c, c);
    printf("  转义字符: \\n(换行=%d), \\t(制表=%d), \\\\(%d), \\\"(%d)\n", 
           '\n', '\t', '\\', '\"');

    printf("\n[布尔类型]\n");
    bool flag_true = true;
    bool flag_false = false;
    printf("  true = %d, false = %d\n", flag_true, flag_false);
}

/*============================================================================*/
/*                           变量作用域演示函数                                 */
/*============================================================================*/

/**
 * @brief 变量作用域演示
 * @details 展示C语言中变量的作用域规则：
 *          - 局部变量：块作用域，自动存储期
 *          - 全局变量：文件作用域，静态存储期
 *          - 静态全局变量：文件作用域，内部链接
 *          - 变量遮蔽：内层块可以遮蔽外层同名变量
 * 
 * @note 避免使用同名变量遮蔽，容易引起混淆
 * @note 全局变量应尽量少用，降低耦合度
 */
void demo_variable_scope(void)
{
    print_separator("变量作用域演示");

    int32_t local_var = 10;
    printf("  局部变量 local_var = %d\n", local_var);
    printf("  全局变量 g_global_var = %d\n", g_global_var);
    printf("  静态全局变量 s_global_static_var = %d\n", s_global_static_var);

    {
        int32_t local_var = 20;
        printf("  内部块局部变量 local_var = %d (遮蔽外部同名变量)\n", local_var);
    }
    
    printf("  块外局部变量 local_var = %d (恢复原值)\n", local_var);
}

/*============================================================================*/
/*                           存储类演示函数                                     */
/*============================================================================*/

/**
 * @brief 存储类演示
 * @details 展示C语言中的四种存储类：
 *          - auto：自动存储期，块作用域（默认）
 *          - static：静态存储期，保持值持久
 *          - register：建议存储在寄存器中
 *          - extern：外部链接，跨文件访问
 * 
 * @note static局部变量只初始化一次，值在函数调用间保持
 * static的作用主要为两种：1.修饰局部变量，延长生命周期到程序结束，只初始化一次保留上一次的值；
 *                         2.修饰全局变量或函数，限制作用域，该变量及函数只能在本文件中使用，无法通过extern外部链接
 * @note register只是建议，编译器可能忽略
 */
void demo_storage_classes(void)
{
    print_separator("存储类演示");

    auto int32_t auto_var = 1;
    printf("  auto变量(默认): auto_var = %d (自动存储期, 块作用域)\n", auto_var);

    static int32_t static_local_var = 0;
    static_local_var++;
    printf("  static局部变量: static_local_var = %d (静态存储期, 只初始化一次)\n", 
           static_local_var);

    register int32_t reg_var = 100;
    printf("  register变量: reg_var = %d (建议存于寄存器, 不可取地址)\n", reg_var);

    printf("  extern变量: g_global_var = %d (外部链接, 跨文件访问)\n", g_global_var);
}

/*============================================================================*/
/*                           常量类型演示函数                                   */
/*============================================================================*/

/**
 * @brief 常量类型演示
 * @details 展示C语言中定义常量的三种方式：
 *          - const关键字：类型安全的只读变量，可能占内存
 *          - 宏定义：预处理器文本替换，不占内存
 *          - 枚举常量：整数常量集合
 * 
 * @note 推荐使用const替代宏定义常量（类型安全）
 * @note 枚举适合定义相关的一组整数常量
 */
void demo_constant_types(void)
{
    print_separator("常量类型演示");

    const int32_t const_var = 100;
    printf("  const常量: const_var = %d (只读, 编译时检查)\n", const_var);

    #define PI 3.14159265f
    printf("  宏定义常量: PI = %.8f (预处理器替换)\n", PI);

    enum {
        COLOR_RED = 0,
        COLOR_GREEN = 1,
        COLOR_BLUE = 2,
        COLOR_MAX
    };
    printf("  枚举常量: RED=%d, GREEN=%d, BLUE=%d, MAX=%d\n", 
           COLOR_RED, COLOR_GREEN, COLOR_BLUE, COLOR_MAX);

    enum {
        BIT_0 = (1U << 0),
        BIT_1 = (1U << 1),
        BIT_2 = (1U << 2),
        BIT_3 = (1U << 3)
    };
    printf("  位掩码枚举: BIT_0=%u, BIT_1=%u, BIT_2=%u, BIT_3=%u\n", 
           BIT_0, BIT_1, BIT_2, BIT_3);
}

/*============================================================================*/
/*                           算术运算符演示函数                                 */
/*============================================================================*/

/**
 * @brief 算术运算符演示
 * @details 展示C语言中的算术运算符：
 *          - 基本运算：+、-、*、/、%
 *          - 自增自减：++、--（前缀和后缀形式）
 * 
 * @note 整数除法会截断小数部分
 * @note 取模运算符%只能用于整数
 * @note 前缀++先加后用，后缀++先用后加
 */
void demo_arithmetic_operators(void)
{
    print_separator("算术运算符演示");

    int32_t a = 17;
    int32_t b = 5;

    printf("  a = %d, b = %d\n", a, b);//17,5
    printf("  加法 (+):  a + b = %d\n", a + b);//17+5
    printf("  减法 (-):  a - b = %d\n", a - b);//17-5
    printf("  乘法 (*):  a * b = %d\n", a * b);//17*5
    printf("  除法 (/):  a / b = %d (整数除法截断)\n", a / b);//17/5=3
    printf("  取模 (%%): a %% b = %d\n", a % b);//17%5=2

    printf("\n[自增/自减运算符]\n");
    int32_t c = 10;
    printf("  c = %d\n", c);
    printf("  ++c (前缀): %d, c = %d\n", ++c, c);//先自增再赋值，先加1，然后用这个新值去参与运算。11,11
    c = 10;
    printf("  c++ (后缀): %d, c = %d\n", c++, c);//先用原值计算再加一。10,11
    c = 10;
    printf("  --c (前缀): %d, c = %d\n", --c, c);//先减一再用新值去参与计算。9,9
    c = 10;
    printf("  c-- (后缀): %d, c = %d\n", c--, c);//先用原值再减一。10,9
}

/*============================================================================*/
/*                           关系运算符演示函数                                 */
/*============================================================================*/

/**
 * @brief 关系运算符演示
 * @details 展示C语言中的关系运算符：
 *          - 相等性：==、!=
 *          - 大小比较：>、<、>=、<=
 * 
 * @note 关系运算符返回整数0（假）或1（真）
 * @note 注意区分==和=，避免赋值误用
 */
void demo_relational_operators(void)
{
    print_separator("关系运算符演示");

    int32_t a = 10;
    int32_t b = 20;

    printf("  a = %d, b = %d\n", a, b);
    printf("  相等 (==): a == b = %s\n", (a == b) ? "true" : "false");
    printf("  不等 (!=): a != b = %s\n", (a != b) ? "true" : "false");
    printf("  大于 (>):  a > b  = %s\n", (a > b) ? "true" : "false");
    printf("  小于 (<):  a < b  = %s\n", (a < b) ? "true" : "false");
    printf("  大等于(>=): a >= b = %s\n", (a >= b) ? "true" : "false");
    printf("  小等于(<=): a <= b = %s\n", (a <= b) ? "true" : "false");
}

/*============================================================================*/
/*                           逻辑运算符演示函数                                 */
/*============================================================================*/

/**
 * @brief 逻辑运算符演示
 * @details 展示C语言中的逻辑运算符：
 *          - 逻辑与：&&
 *          - 逻辑或：||
 *          - 逻辑非：!
 *          - 短路求值特性
 * 
 * @note 短路求值：&&遇假即停，||遇真即停
 * @note 利用短路求值可以避免潜在错误
 */
void demo_logical_operators(void)
{
    print_separator("逻辑运算符演示");

    bool a = true;
    bool b = false;

    printf("  a = %s, b = %s\n", a ? "true" : "false", b ? "true" : "false");
    printf("  逻辑与 (&&): a && b = %s\n", (a && b) ? "true" : "false");
    printf("  逻辑或 (||): a || b = %s\n", (a || b) ? "true" : "false");
    printf("  逻辑非 (!):  !a = %s, !b = %s\n", (!a) ? "true" : "false", (!b) ? "true" : "false");

    printf("\n[短路求值演示]\n");
    int32_t x = 0;
    bool result = (x != 0) && (10 / x > 1);
    printf("  (x != 0) && (10/x > 1): 由于短路求值, 避免除零错误, result = %s\n", 
           result ? "true" : "false");
}

/*============================================================================*/
/*                           位运算符演示函数                                   */
/*============================================================================*/

/**
 * @brief 位运算符演示
 * @details 展示C语言中的位运算符：
 *          - 按位与：&
 *          - 按位或：|
 *          - 按位异或：^
 *          - 按位取反：~
 *          - 左移：<<
 *          - 右移：>>
 * 
 * @note 位操作是嵌入式开发的核心技能
 * @note 左移相当于乘2，右移相当于除2
 * @note 无符号数右移是逻辑右移，有符号数右移取决于编译器
 *
 * 置一：reg |= (1<<bit) //将变量第bit位的数设置为1
 * 清零：reg &= ~(1<<bit)//将变量第bit位的数设置为0
 * 翻转：reg ^= (1<<bit) //将变量第bit位的数翻转，0置一，1置零
 * 读值：if((reg >> bit) & 1) == 1)//判断第bit位的数值是否为1
 */
void demo_bitwise_operators(void)
{
    print_separator("位运算符演示");

    uint8_t a = 0b11001010;
    uint8_t b = 0b10110101;

    printf("  a = 0b" BYTE_TO_BINARY_FMT " (0x%02X)\n", BYTE_TO_BINARY(a), a);
    printf("  b = 0b" BYTE_TO_BINARY_FMT " (0x%02X)\n", BYTE_TO_BINARY(b), b);
    printf("  按位与 (&):  a & b = 0b" BYTE_TO_BINARY_FMT " (0x%02X)\n", 
           BYTE_TO_BINARY(a & b), a & b);
    printf("  按位或 (|):  a | b = 0b" BYTE_TO_BINARY_FMT " (0x%02X)\n", 
           BYTE_TO_BINARY(a | b), a | b);
    printf("  按位异或(^): a ^ b = 0b" BYTE_TO_BINARY_FMT " (0x%02X)\n", 
           BYTE_TO_BINARY(a ^ b), a ^ b);
    printf("  按位取反(~): ~a    = 0b" BYTE_TO_BINARY_FMT " (0x%02X)\n", 
           BYTE_TO_BINARY((uint8_t)(~a)), (uint8_t)(~a));
    printf("  左移 (<<):   a << 2 = 0b" BYTE_TO_BINARY_FMT " (0x%02X)\n", 
           BYTE_TO_BINARY((uint8_t)(a << 2)), (uint8_t)(a << 2));
    printf("  右移 (>>):   a >> 2 = 0b" BYTE_TO_BINARY_FMT " (0x%02X)\n", 
           BYTE_TO_BINARY((uint8_t)(a >> 2)), (uint8_t)(a >> 2));

    printf("\n[位操作技巧]\n");
    uint8_t reg = 0x00;//->0000 0000
    BIT_SET(reg, 3);//->00001000
    printf("  设置位3: reg = 0x%02X\n", reg);//0x08
    BIT_SET(reg, 5);//->00101000
    printf("  设置位5: reg = 0x%02X\n", reg);//0x28
    BIT_CLEAR(reg, 3);//00100000
    printf("  清除位3: reg = 0x%02X\n", reg);//0x20
    BIT_TOGGLE(reg, 5);//00000000
    printf("  翻转位5: reg = 0x%02X\n", reg);//0x00
    printf("  检查位5: bit5 = %d\n", BIT_CHECK(reg, 5));//0
}

/*============================================================================*/
/*                           赋值运算符演示函数                                 */
/*============================================================================*/

/**
 * @brief 赋值运算符演示
 * @details 展示C语言中的赋值运算符：
 *          - 基本赋值：=
 *          - 复合赋值：+=、-=、*=、/=、%=
 *          - 位运算复合赋值：&=、|=、^=、<<=、>>=
 * 
 * @note 复合赋值使代码更简洁
 * @note 嵌入式开发中常用位运算复合赋值操作寄存器
 */
void demo_assignment_operators(void)
{
    print_separator("赋值运算符演示");

    int32_t a = 10;
    printf("  初始值: a = %d\n", a);

    a += 5;
    printf("  a += 5:  a = %d\n", a);//a=a+5;
    
    a -= 3;
    printf("  a -= 3:  a = %d\n", a);//a=a-3;
    
    a *= 2;
    printf("  a *= 2:  a = %d\n", a);//a=a*2
    
    a /= 4;
    printf("  a /= 4:  a = %d\n", a);//a=a/4
    
    a %= 3;
    printf("  a %%= 3:  a = %d\n", a);//a=a%3

    a = 0x0F;//-->00001111
    a &= 0x05;//->00000101
    printf("  a &= 0x05: a = 0x%02X\n", a);//a=a&0x05---->00000101---->0x05
    
    a |= 0x80;//10000000
    printf("  a |= 0x80: a = 0x%02X\n", a);//a=a|0x80---->10000101---->0x85
    
    a ^= 0xFF;//11111111
    printf("  a ^= 0xFF: a = 0x%02X\n", a);//a=a^0xFF---->01111010---->0x7A
    
    a <<= 2;//左移两位
    printf("  a <<= 2:   a = 0x%02X\n", a);//01 1110 1000----------------->0X1E8
    
    a >>= 1;
    printf("  a >>= 1:   a = 0x%02X\n", a);//1111 0100----------------->0XF4
}

/*============================================================================*/
/*                           条件运算符演示函数                                 */
/*============================================================================*/

/**
 * @brief 条件运算符（三元运算符）演示
 * @details 展示C语言中唯一的三元运算符：
 *          - 语法：条件 ? 表达式1 : 表达式2
 *          - 用途：简化简单的if-else语句
 * 
 * @note 条件运算符可嵌套，但会降低可读性
 * @note 嵌入式开发中常用于条件赋值
 */
void demo_conditional_operator(void)
{
    print_separator("条件运算符(三元运算符)演示");

    int32_t a = 10;
    int32_t b = 20;
    int32_t max = (a > b) ? a : b;
    
    printf("  a = %d, b = %d\n", a, b);
    printf("  max = (a > b) ? a : b = %d\n", max);

    int32_t score = 75;
    const char *result = (score >= 60) ? "及格" : "不及格";
    printf("  分数 = %d, 结果: %s\n", score, result);

    uint8_t value = 0x85;//10000101
    uint8_t nibble_high = (value >> 4) & 0x0F;//0000 1000 & 0000 1111----->0000 1000
    uint8_t nibble_low = value & 0x0F;//1000 0101 & 0000 1111-------->0000 0101
    uint8_t max_nibble = (nibble_high > nibble_low) ? nibble_high : nibble_low;
    printf("  value = 0x%02X, 高半字节=0x%X, 低半字节=0x%X, 较大者=0x%X\n", 
           value, nibble_high, nibble_low, max_nibble);
}

/*============================================================================*/
/*                           类型转换演示函数                                   */
/*============================================================================*/

/**
 * @brief 类型转换演示
 * @details 展示C语言中的类型转换：
 *          - 隐式类型转换：自动进行，可能丢失精度
 *          - 显式类型转换：强制转换，明确意图
 *          - 整数截断风险
 *          - 溢出风险
 * 
 * @note 嵌入式开发中常进行ADC值到物理量的转换
 * @note 强制转换时要注意数据范围
 */
void demo_type_conversion(void)
{
    print_separator("类型转换演示");

    printf("[隐式类型转换(自动转换)]\n");
    int32_t i = 10;
    float f = 3.5f;
    double result = i + f;
    printf("  int(%d) + float(%.1f) = double(%.6f) (自动提升)\n", i, f, result);

    printf("\n[显式类型转换(强制转换)]\n");
    int32_t a = 5;
    int32_t b = 3;
    float div_result = (float)a / (float)b;
    printf("  %d / %d = %d (整数除法)\n", a, b, a / b);
    printf("  (float)%d / (float)%d = %.4f (浮点除法)\n", a, b, div_result);

    printf("\n[整数截断风险]\n");
    double large_val = 123456.789;
    int32_t truncated = (int32_t)large_val;
    printf("  double %.3f -> int %d (小数部分丢失)\n", large_val, truncated);

    printf("\n[溢出风险]\n");
    int32_t big_int = 300;
    int8_t small_int = (int8_t)big_int;
    printf("  int32 %d -> int8 %d (溢出, 范围-128~127)\n", big_int, small_int);

    printf("\n[嵌入式常用转换]\n");
    uint32_t adc_value = 2048;
    float voltage = (float)adc_value * 3.3f / 4095.0f;
    printf("  ADC值 %u -> 电压 %.3fV (12位ADC, 参考电压3.3V)\n", adc_value, voltage);
}

/*============================================================================*/
/*                           sizeof运算符演示函数                               */
/*============================================================================*/

/**
 * @brief sizeof运算符演示
 * @details 展示sizeof运算符的使用：
 *          - 基本类型大小
 *          - 数组大小计算
 *          - 结构体大小与内存对齐
 * 
 * @note sizeof是编译时运算符，不是函数
 * @note 常用技巧：数组元素个数 = sizeof(arr) / sizeof(arr[0])
 * @note 结构体大小可能因对齐而大于成员大小之和
 * 结构体对齐：1.成员地址大小必定是成员类型大小的整数倍；
 *             2.结构体大小必是最大成员大小的整数倍。
 */
void demo_sizeof_operator(void)
{
    print_separator("sizeof运算符演示");

    printf("[基本类型大小]\n");
    printf("  sizeof(char)      = %zu 字节\n", sizeof(char));//1
    printf("  sizeof(short)     = %zu 字节\n", sizeof(short));//2
    printf("  sizeof(int)       = %zu 字节\n", sizeof(int));//4
    printf("  sizeof(long)      = %zu 字节\n", sizeof(long));//long:4,long long:8
    printf("  sizeof(float)     = %zu 字节\n", sizeof(float));//4
    printf("  sizeof(double)    = %zu 字节\n", sizeof(double));//8
    printf("  sizeof(void*)     = %zu 字节 (指针大小)\n", sizeof(void*));//32位:4,64位:8

    printf("\n[数组大小计算]\n");
    int32_t arr[] = {1, 2, 3, 4, 5};
    size_t arr_bytes = sizeof(arr);//整个数组字节大小 4*5=20
    size_t elem_bytes = sizeof(arr[0]);//首个元素的字节大小，int类型:4字节
    size_t elem_count = arr_bytes / elem_bytes;//数组元素个数
    printf("  数组总大小: %zu 字节\n", arr_bytes);
    printf("  单元素大小: %zu 字节\n", elem_bytes);
    printf("  元素个数:   %zu = %zu / %zu\n", elem_count, arr_bytes, elem_bytes);

    printf("\n[结构体大小与对齐]\n");
    struct Unaligned {
        uint8_t a;
        uint32_t b;
        uint8_t c;
    };
#pragma pack(push, 1)
    struct Packed {
        uint8_t a;
        uint32_t b;
        uint8_t c;
    };
#pragma pack(pop)
    printf("  普通结构体大小: %zu 字节 (含填充)\n", sizeof(struct Unaligned));
    printf("  紧凑结构体大小: %zu 字节 (无填充)\n", sizeof(struct Packed));
}

/*============================================================================*/
/*                           运算符优先级演示函数                               */
/*============================================================================*/

/**
 * @brief 运算符优先级演示
 * @details 展示C语言运算符的优先级和结合性：
 *          - 括号 () 最高
 *          - 单目运算符次之：++，--，~，-，*，&，+，！。
 *          - 算术 > 关系 > 逻辑 > 条件 > 赋值
 * 
 * @note 建议使用括号明确优先级，提高可读性
 * @note 不要依赖优先级记忆，复杂表达式用括号
 */
void demo_operator_precedence(void)
{
    print_separator("运算符优先级演示");

    int32_t a = 2;
    int32_t b = 3;
    int32_t c = 4;

    printf("[优先级从高到低]\n");
    printf("  括号 () > 单目 > 算术 > 关系 > 逻辑 > 条件 > 赋值\n\n");

    int32_t r1 = a + b * c;
    int32_t r2 = (a + b) * c;
    printf("  a + b * c = %d (先乘后加)\n", r1);
    printf("  (a + b) * c = %d (括号改变优先级)\n", r2);

    int32_t r3 = a < b && b < c;
    int32_t r4 = a < b + c;
    printf("  a < b && b < c = %d (关系高于逻辑)\n", r3);
    printf("  a < b + c = %d (算术高于关系)\n", r4);

    int32_t x = 1;
    int32_t r5 = x = a + b;
    printf("  x = a + b = %d (算术高于赋值)\n", r5);

    printf("\n[建议: 使用括号明确优先级, 提高可读性]\n");
}
