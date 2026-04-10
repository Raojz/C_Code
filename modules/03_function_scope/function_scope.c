/**
 * @file function_scope.c
 * @brief 函数与作用域模块实现
 * @details 演示C语言函数定义、声明、参数传递、作用域规则等。
 *          本模块涵盖以下核心知识点：
 *          - 函数声明与定义
 *          - 参数传递：值传递、指针传递
 *          - 返回值：单返回值、多返回值（指针参数）
 *          - 作用域：文件作用域、块作用域、函数原型作用域
 *          - 存储类：static函数、extern声明
 *          - 递归函数
 *          - 内联函数
 *          - 函数指针
 * 
 * @section embedded_importance 嵌入式开发重要性
 * - 函数是模块化设计的基础
 * - static函数限制作用域，避免命名冲突
 * - 内联函数用于性能关键代码
 * - 函数指针实现回调机制和状态机
 * 
 * @author Embedded C Learning Project
 * @date 2026-03-22
 * @version 1.0.0
 * 
 * @note 本文件遵循 MISRA-C:2012 编码规范
 */

#include "function_scope.h"
#include "common/common_utils.h"
#include <stdio.h>
#include <string.h>

/*============================================================================*/
/*                           文件作用域变量                                     */
/*============================================================================*/

/** @brief 文件作用域静态变量（仅本文件可见） */
static int32_t s_file_scope_var = 100;

/** @brief 文件作用域静态计数器 */
static uint32_t s_call_counter = 0;

/*============================================================================*/
/*                           静态函数定义                                       */
/*============================================================================*/

/**
 * @brief 静态辅助函数（仅本文件可见）
 * @details static关键字限制函数作用域为文件内部
 * @param[in] value 输入值
 * @return 计算结果
 * @note 嵌入式开发中常用static限制函数作用域
 */
static int32_t private_helper(int32_t value)
{
    return value * 2;
}

/**
 * @brief 数值翻倍（静态函数前向声明）
 * @param value 输入值
 * @return 翻倍后的值
 */
static int32_t double_value(int32_t value);

/*============================================================================*/
/*                           基本函数演示                                       */
/*============================================================================*/

/**
 * @brief 函数声明与定义演示
 * @details 展示函数的基本概念：
 *          - 函数声明（原型）
 *          - 函数定义
 *          - 函数调用
 *          - 函数返回值
 * 
 * @note 函数声明告诉编译器函数的存在
 * @note 函数定义提供函数的实际代码
 */
void demo_function_basics(void)
{
    print_separator("函数声明与定义演示");

    printf("[函数声明(原型)]\n");
    printf("  返回类型 函数名(参数类型1 参数名1, 参数类型2 参数名2);\n");
    printf("  示例: int32_t add(int32_t a, int32_t b);\n\n");

    printf("[函数定义]\n");
    printf("  返回类型 函数名(参数类型1 参数名1, 参数类型2 参数名2) {\n");
    printf("      函数体;\n");
    printf("      return 返回值;\n");
    printf("  }\n\n");

    printf("[函数调用]\n");
    int32_t result = add_numbers(10, 20);
    printf("  add_numbers(10, 20) = %d\n", result);

    printf("\n[void函数]\n");
    print_greeting("嵌入式开发");
    printf("  void函数无返回值，用于执行操作\n");

    printf("\n[空参数列表]\n");
    printf("  void func(void) - 明确表示无参数\n");
    printf("  void func()     - C中表示未指定参数(不推荐)\n");
}

/**
 * @brief 两数相加
 * @param[in] a 第一个数
 * @param[in] b 第二个数
 * @return 两数之和
 */
int32_t add_numbers(int32_t a, int32_t b)
{
    return a + b;
}

/**
 * @brief 打印问候语
 * @param[in] name 名称字符串
 */
void print_greeting(const char *name)
{
    if (name != NULL) {
        printf("  你好, %s!\n", name);
    }
}

/*============================================================================*/
/*                           参数传递演示                                       */
/*============================================================================*/

/**
 * @brief 参数传递演示
 * @details 展示C语言的参数传递方式：
 *          - 值传递：复制实参的值
 *          - 指针传递：传递地址，可修改实参
 *          - 数组传递：退化为指针
 *          - const参数：防止修改
 * 
 * @note C语言只有值传递，指针传递本质也是值传递（传递地址值）
 * @note 嵌入式开发中常用指针传递避免大结构体复制
 */
void demo_parameter_passing(void)
{
    print_separator("参数传递演示");

    printf("[值传递]\n");
    int32_t x = 10;
    printf("  调用前: x = %d\n", x);
    int32_t result = modify_value(x);
    printf("  调用后: x = %d (未改变)\n", x);
    printf("  返回值: %d\n", result);

    printf("\n[指针传递]\n");
    int32_t y = 10;
    printf("  调用前: y = %d\n", y);
    modify_by_pointer(&y);
    printf("  调用后: y = %d (已改变)\n", y);

    printf("\n[数组传递]\n");
    int32_t arr[] = {1, 2, 3, 4, 5};
    printf("  数组: ");
    for (size_t i = 0; i < ARRAY_SIZE(arr); i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    int32_t sum = sum_array(arr, ARRAY_SIZE(arr));
    printf("  数组元素之和: %d\n", sum);

    printf("\n[const参数]\n");
    const char *str = "Hello";
    size_t len = get_string_length(str);
    printf("  字符串 \"%s\" 长度: %zu\n", str, len);
    printf("  const参数防止函数内部修改数据\n");
}

/**
 * @brief 值传递示例（尝试修改值）
 * @param[in] value 输入值
 * @return 修改后的值
 */
int32_t modify_value(int32_t value)
{
    value = value * 2;
    return value;
}

/**
 * @brief 指针传递示例（修改原值）
 * @param[in,out] ptr 指向要修改的变量
 */
void modify_by_pointer(int32_t *ptr)
{
    if (ptr != NULL) {
        *ptr = *ptr * 2;
    }
}

/**
 * @brief 计算数组元素之和
 * @param[in] arr 数组指针
 * @param[in] size 数组大小
 * @return 数组元素之和
 */
int32_t sum_array(const int32_t *arr, size_t size)
{
    int32_t sum = 0;
    if (arr != NULL) {
        for (size_t i = 0; i < size; i++) {
            sum += arr[i];
        }
    }
    return sum;
}

/**
 * @brief 获取字符串长度
 * @param[in] str 字符串指针
 * @return 字符串长度
 */
size_t get_string_length(const char *str)
{
    size_t len = 0;
    if (str != NULL) {
        while (str[len] != '\0') {
            len++;
        }
    }
    return len;
}

/*============================================================================*/
/*                           多返回值演示                                       */
/*============================================================================*/

/**
 * @brief 多返回值演示
 * @details 展示C语言实现多返回值的方法：
 *          - 指针参数：通过指针参数返回多个值
 *          - 结构体：返回包含多个值的结构体
 *          - 数组：返回数组指针
 * 
 * @note C语言函数只能返回一个值
 * @note 指针参数是嵌入式开发中常用的多返回值方式
 */
void demo_multiple_return_values(void)
{
    print_separator("多返回值演示");

    printf("[指针参数返回多个值]\n");
    int32_t quotient = 0;
    int32_t remainder = 0;
    divide_numbers(17, 5, &quotient, &remainder);
    printf("  17 / 5 = %d 余 %d\n", quotient, remainder);

    printf("\n[结构体返回多个值]\n");
    MinMax_t result = find_min_max((int32_t[]){5, 2, 8, 1, 9, 3}, 6);
    printf("  最小值: %d, 最大值: %d\n", result.min, result.max);

    printf("\n[嵌入式典型应用]\n");
    printf("  状态码 + 数据: Status_t func(DataType *out_data);\n");
    printf("  错误码 + 结果: ErrorCode_t parse(InputType in, ResultType *out);\n");
}

/**
 * @brief 除法运算（返回商和余数）
 * @param[in] dividend 被除数
 * @param[in] divisor 除数
 * @param[out] quotient 商
 * @param[out] remainder 余数
 * @return 成功返回true，除数为0返回false
 */
bool divide_numbers(int32_t dividend, int32_t divisor, int32_t *quotient, int32_t *remainder)
{
    if (divisor == 0 || quotient == NULL || remainder == NULL) {
        return false;
    }
    *quotient = dividend / divisor;
    *remainder = dividend % divisor;
    return true;
}

/**
 * @brief 查找数组最小值和最大值
 * @param[in] arr 数组指针
 * @param[in] size 数组大小
 * @return 包含最小值和最大值的结构体
 */
MinMax_t find_min_max(const int32_t *arr, size_t size)
{
    MinMax_t result = {0, 0};
    if (arr != NULL && size > 0) {
        result.min = arr[0];
        result.max = arr[0];
        for (size_t i = 1; i < size; i++) {
            if (arr[i] < result.min) {
                result.min = arr[i];
            }
            if (arr[i] > result.max) {
                result.max = arr[i];
            }
        }
    }
    return result;
}

/*============================================================================*/
/*                           作用域规则演示                                     */
/*============================================================================*/

/**
 * @brief 作用域规则演示
 * @details 展示C语言的作用域规则：
 *          - 文件作用域：全局变量、函数
 *          - 块作用域：局部变量
 *          - 函数原型作用域：参数名
 *          - 函数作用域：标签名
 * 
 * @note 变量遮蔽：内层块可以定义同名变量遮蔽外层变量
 * @note 嵌入式开发应避免全局变量，降低耦合
 */
void demo_scope_rules(void)
{
    print_separator("作用域规则演示");

    printf("[文件作用域]\n");
    printf("  全局变量: 整个程序可见\n");
    printf("  static全局变量: 仅本文件可见\n");
    printf("  s_file_scope_var = %d (文件作用域静态变量)\n", s_file_scope_var);

    printf("\n[块作用域]\n");
    int32_t block_var = 10;
    printf("  外层块变量 block_var = %d\n", block_var);
    {
        int32_t block_var = 20;
        printf("  内层块变量 block_var = %d (遮蔽外层同名变量)\n", block_var);
    }
    printf("  外层块变量 block_var = %d (恢复原值)\n", block_var);

    printf("\n[函数原型作用域]\n");
    printf("  函数声明中的参数名作用域到声明结束\n");
    printf("  示例: void func(int x); // x的作用域仅在此声明中\n");

    printf("\n[最佳实践]\n");
    printf("  - 避免使用全局变量\n");
    printf("  - 变量定义在最接近使用的位置\n");
    printf("  - 避免同名变量遮蔽\n");
    printf("  - 使用static限制变量和函数的作用域\n");
}

/*============================================================================*/
/*                           存储类演示                                         */
/*============================================================================*/

/**
 * @brief 存储类演示
 * @details 展示函数相关的存储类：
 *          - static函数：文件作用域，内部链接
 *          - extern声明：引用外部函数
 *          - inline函数：建议内联展开
 * 
 * @note static函数避免命名冲突，嵌入式开发常用
 * @note inline函数用于性能关键代码
 */
void demo_storage_class(void)
{
    print_separator("存储类演示");

    printf("[static函数]\n");
    printf("  static关键字限制函数作用域为文件内部\n");
    printf("  避免与其他文件的同名函数冲突\n");
    int32_t result = private_helper(5);
    printf("  private_helper(5) = %d (static函数)\n", result);

    printf("\n[extern声明]\n");
    printf("  extern关键字声明外部定义的函数\n");
    printf("  extern void external_func(void);\n");
    printf("  用于引用其他文件中定义的函数\n");

    printf("\n[inline函数]\n");
    printf("  inline关键字建议编译器内联展开\n");
    printf("  用于频繁调用的小函数，减少函数调用开销\n");
    int32_t max_val = inline_max(10, 20);
    printf("  inline_max(10, 20) = %d\n", max_val);

    printf("\n[static局部变量]\n");
    increment_counter();
    increment_counter();
    increment_counter();
    printf("  static局部变量在函数调用间保持值\n");
}

/**
 * @brief 递增计数器（演示static局部变量）
 */
void increment_counter(void)
{
    static uint32_t counter = 0;
    counter++;
    printf("  计数器: %d (static局部变量)\n", counter);
}

/*============================================================================*/
/*                           递归函数演示                                       */
/*============================================================================*/

/**
 * @brief 递归函数演示
 * @details 展示递归函数的使用：
 *          - 递归定义：函数调用自身
 *          - 递归终止条件
 *          - 递归调用栈
 *          - 递归与迭代的选择
 * 
 * @note 递归可能导致栈溢出，嵌入式开发需谨慎使用
 * @note 性能关键代码优先使用迭代
 */
void demo_recursion(void)
{
    print_separator("递归函数演示");

    printf("[递归计算阶乘]\n");
    for (int32_t i = 0; i <= 10; i++) {
        printf("  %d! = %lld\n", i, factorial(i));
    }

    printf("\n[递归计算斐波那契数列]\n");
    printf("  斐波那契数列前15项: ");
    for (int32_t i = 0; i < 15; i++) {
        printf("%lld ", fibonacci(i));
    }
    printf("\n");

    printf("\n[递归与迭代对比]\n");
    printf("  递归优点: 代码简洁，逻辑清晰\n");
    printf("  递归缺点: 占用栈空间，可能栈溢出，效率较低\n");
    printf("  嵌入式开发建议: 优先使用迭代，避免递归\n");

    printf("\n[递归调用栈分析]\n");
    printf("  每次递归调用都会在栈上分配新的局部变量\n");
    printf("  factorial(5) 调用栈深度: 6层 (包括初始调用)\n");
}

/**
 * @brief 计算阶乘（递归实现）
 * @param[in] n 非负整数
 * @return n的阶乘
 */
int64_t factorial(int32_t n)
{
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

/**
 * @brief 计算斐波那契数（递归实现）
 * @param[in] n 项数（从0开始）
 * @return 第n项斐波那契数
 */
int64_t fibonacci(int32_t n)
{
    if (n <= 1) {
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

/*============================================================================*/
/*                           函数指针演示                                       */
/*============================================================================*/

/**
 * @brief 函数指针演示
 * @details 展示函数指针的使用：
 *          - 函数指针声明
 *          - 函数指针赋值
 *          - 通过函数指针调用函数
 *          - 函数指针数组
 *          - 回调函数
 * 
 * @note 函数指针是嵌入式开发的核心技术
 * @note 用于实现回调机制、状态机、策略模式
 */
void demo_function_pointer(void)
{
    print_separator("函数指针演示");

    printf("[函数指针声明]\n");
    printf("  返回类型 (*指针名)(参数类型1, 参数类型2);\n");
    printf("  示例: int32_t (*op_func)(int32_t, int32_t);\n\n");

    printf("[函数指针使用]\n");
    OperationFunc_t op = add_numbers;
    int32_t result = op(10, 20);
    printf("  通过函数指针调用: op(10, 20) = %d\n", result);

    printf("\n[函数指针数组]\n");
    OperationFunc_t operations[] = {add_numbers, subtract_numbers, multiply_numbers};
    const char *op_names[] = {"加法", "减法", "乘法"};
    int32_t a = 15, b = 5;
    for (size_t i = 0; i < ARRAY_SIZE(operations); i++) {
        printf("  %s: %d %s %d = %d\n", op_names[i], a, 
               (i == 0) ? "+" : (i == 1) ? "-" : "*", b, operations[i](a, b));
    }

    printf("\n[回调函数]\n");
    int32_t arr[] = {5, 2, 8, 1, 9, 3};
    printf("  原数组: ");
    for (size_t i = 0; i < ARRAY_SIZE(arr); i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    process_array(arr, ARRAY_SIZE(arr), double_value);
    printf("  处理后: ");
    for (size_t i = 0; i < ARRAY_SIZE(arr); i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    printf("\n[嵌入式应用]\n");
    printf("  - 回调函数: 事件处理、中断处理\n");
    printf("  - 状态机: 状态转换表\n");
    printf("  - 策略模式: 算法选择\n");
    printf("  - 驱动框架: 设备操作函数表\n");
}

/**
 * @brief 减法运算
 * @param[in] a 被减数
 * @param[in] b 减数
 * @return 差
 */
int32_t subtract_numbers(int32_t a, int32_t b)
{
    return a - b;
}

/**
 * @brief 乘法运算
 * @param[in] a 第一个数
 * @param[in] b 第二个数
 * @return 积
 */
int32_t multiply_numbers(int32_t a, int32_t b)
{
    return a * b;
}

/**
 * @brief 数值翻倍
 * @param value 输入值
 * @return 翻倍后的值
 */
static int32_t double_value(int32_t value)
{
    return value * 2;
}

/**
 * @brief 处理数组元素
 * @param[in,out] arr 数组指针
 * @param[in] size 数组大小
 * @param[in] processor 处理函数指针
 */
void process_array(int32_t *arr, size_t size, ProcessFunc_t processor)
{
    if (arr != NULL && processor != NULL) {
        for (size_t i = 0; i < size; i++) {
            arr[i] = processor(arr[i]);
        }
    }
}

/*============================================================================*/
/*                           可变参数演示                                       */
/*============================================================================*/

/**
 * @brief 可变参数函数演示
 * @details 展示可变参数函数的使用：
 *          - va_list、va_start、va_arg、va_end宏
 *          - 可变参数函数的声明和定义
 *          - 可变参数的安全使用
 * 
 * @note 可变参数函数缺乏类型安全
 * @note 嵌入式开发中较少使用，建议优先考虑其他方案
 */
void demo_variadic_function(void)
{
    print_separator("可变参数函数演示");

    printf("[可变参数函数声明]\n");
    printf("  返回类型 函数名(固定参数, ...);\n");
    printf("  示例: int32_t sum_integers(int32_t count, ...);\n\n");

    printf("[可变参数函数调用]\n");
    int32_t sum1 = sum_integers(3, 10, 20, 30);
    printf("  sum_integers(3, 10, 20, 30) = %d\n", sum1);
    
    int32_t sum2 = sum_integers(5, 1, 2, 3, 4, 5);
    printf("  sum_integers(5, 1, 2, 3, 4, 5) = %d\n", sum2);

    printf("\n[可变参数宏]\n");
    printf("  va_list: 可变参数列表类型\n");
    printf("  va_start: 初始化可变参数列表\n");
    printf("  va_arg: 获取下一个参数\n");
    printf("  va_end: 清理可变参数列表\n");

    printf("\n[注意事项]\n");
    printf("  - 需要某种方式确定参数个数和类型\n");
    printf("  - 缺乏类型安全，容易出错\n");
    printf("  - 嵌入式开发中较少使用\n");
    printf("  - 常见例子: printf, scanf\n");
}

/**
 * @brief 计算多个整数的和
 * @param[in] count 整数个数
 * @param[in] ... 可变参数列表
 * @return 整数之和
 */
int32_t sum_integers(int32_t count, ...)
{
    va_list args;
    int32_t sum = 0;
    
    va_start(args, count);
    for (int32_t i = 0; i < count; i++) {
        sum += va_arg(args, int32_t);
    }
    va_end(args);
    
    return sum;
}
