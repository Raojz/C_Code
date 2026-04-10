/**
 * @file pointer_memory.c
 * @brief 指针与内存管理模块实现
 * @details 演示C语言指针与内存管理的核心概念。
 *          本模块涵盖以下核心知识点:
 *          - 指针基础：声明、初始化、解引用
 *          - 指针运算：算术运算、关系运算
 *          - 指针与数组：数组名与指针的关系
 *          - 指针与函数：指针参数、返回指针
 *          - 多级指针：二级指针、指针数组
 *          - 动态内存：malloc、free、内存泄漏
 *          - 函数指针：回调函数、函数指针数组
 *          - 常见错误：野指针、悬空指针、内存泄漏
 * 
 * @section embedded_importance 嵌入式开发重要性
 * - 指针是嵌入式开发的核心技能
 * - 直接访问硬件寄存器需要指针
 * - 动态内存管理需谨慎使用
 * - 内存泄漏会导致系统崩溃
 * 
 * @author Embedded C Learning Project
 * @date 2026-03-22
 * @version 1.0.0
 * 
 * @note 本文件遵循 MISRA-C:2012 编码规范
 */

#include "pointer_memory.h"
#include "common/common_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*============================================================================*/
/*                           静态函数前向声明                                   */
/*============================================================================*/

/**
 * @brief 数值翻倍（静态函数前向声明）
 * @param value 输入值
 * @return 翻倍后的值
 */
static int32_t double_value(int32_t value);

/*============================================================================*/
/*                           指针基础演示                                       */
/*============================================================================*/

/**
 * @brief 指针基础演示
 * @details 展示指针的基本概念：
 *          - 指针声明与初始化
 *          - 取地址运算符&
 *          - 解引用运算符*
 *          - 空指针NULL
 * 
 * @note 指针必须初始化，未初始化的指针是野指针
 * @note 使用指针前必须检查是否为NULL
 */
void demo_pointer_basics(void)
{
    print_separator("指针基础演示");

    printf("[指针声明与初始化]\n");
    int32_t value = 42;
    int32_t *ptr = &value;
    
    printf("  int32_t value = 42;\n");
    printf("  int32_t *ptr = &value;\n\n");
    
    printf("  value地址: %p\n", (void *)&value);
    printf("  ptr值:     %p (存储的是value的地址)\n", (void *)ptr);
    printf("  ptr地址:   %p (ptr本身的地址)\n", (void *)&ptr);
    printf("  *ptr:      %d (解引用, 访问value的值)\n", *ptr);

    printf("\n[修改指针指向的值]\n");
    *ptr = 100;
    printf("  *ptr = 100;\n");
    printf("  value = %d (通过指针修改)\n", value);

    printf("\n[空指针NULL]\n");
    int32_t *null_ptr = NULL;
    printf("  int32_t *null_ptr = NULL;\n");
    printf("  null_ptr = %p\n", (void *)null_ptr);
    printf("  使用前检查: if (ptr != NULL) { ... }\n");

    printf("\n[指针大小]\n");
    printf("  sizeof(int32_t*) = %zu 字节\n", sizeof(int32_t*));
    printf("  sizeof(char*)    = %zu 字节\n", sizeof(char*));
    printf("  sizeof(void*)    = %zu 字节\n", sizeof(void*));
    printf("  指针大小与类型无关，取决于平台(32位/64位)\n");
}

/*============================================================================*/
/*                           指针运算演示                                       */
/*============================================================================*/

/**
 * @brief 指针运算演示
 * @details 展示指针的算术运算：
 *          - 指针加减整数
 *          - 指针自增自减
 *          - 指针减法
 *          - 指针比较
 * 
 * @note 指针运算按数据类型大小自动调整
 * @note 指针减法得到元素个数差
 */
void demo_pointer_arithmetic(void)
{
    print_separator("指针运算演示");

    printf("[指针加减整数]\n");
    int32_t arr[] = {10, 20, 30, 40, 50};
    int32_t *ptr = arr;
    
    printf("  int32_t arr[] = {10, 20, 30, 40, 50};\n");
    printf("  int32_t *ptr = arr;\n\n");
    
    printf("  ptr      = %p, *ptr = %d\n", (void *)ptr, *ptr);
    printf("  ptr + 1  = %p, *(ptr+1) = %d (偏移4字节)\n", (void *)(ptr + 1), *(ptr + 1));
    printf("  ptr + 2  = %p, *(ptr+2) = %d (偏移8字节)\n", (void *)(ptr + 2), *(ptr + 2));

    printf("\n[指针自增自减]\n");
    ptr = arr;
    printf("  初始: ptr = %p, *ptr = %d\n", (void *)ptr, *ptr);
    ptr++;
    printf("  ptr++: ptr = %p, *ptr = %d\n", (void *)ptr, *ptr);
    ptr--;
    printf("  ptr--: ptr = %p, *ptr = %d\n", (void *)ptr, *ptr);

    printf("\n[指针减法]\n");
    int32_t *ptr1 = &arr[1];
    int32_t *ptr2 = &arr[4];
    printf("  ptr1 = &arr[1] = %p\n", (void *)ptr1);
    printf("  ptr2 = &arr[4] = %p\n", (void *)ptr2);
    printf("  ptr2 - ptr1 = %td (元素个数差)\n", ptr2 - ptr1);

    printf("\n[指针比较]\n");
    if (ptr1 < ptr2) {
        printf("  ptr1 < ptr2: ptr1在ptr2之前\n");
    }

    printf("\n[不同类型的指针运算]\n");
    char char_arr[] = {'A', 'B', 'C', 'D', 'E'};
    char *char_ptr = char_arr;
    printf("  char *ptr: ptr+1偏移 %zu 字节\n", sizeof(char));
    printf("  int *ptr:  ptr+1偏移 %zu 字节\n", sizeof(int));
}

/*============================================================================*/
/*                           指针与数组演示                                     */
/*============================================================================*/

/**
 * @brief 指针与数组演示
 * @details 展示指针与数组的密切关系：
 *          - 数组名是首元素地址
 *          - 指针访问数组元素
 *          - 数组作为函数参数
 *          - 指针与数组的区别
 * 
 * @note 数组名是常量指针，不可修改
 * @note sizeof(数组名)得到整个数组大小
 * @note sizeof(指针)得到指针本身大小
 */
void demo_pointer_array(void)
{
    print_separator("指针与数组演示");

    printf("[数组名与指针]\n");
    int32_t arr[] = {1, 2, 3, 4, 5};
    int32_t *ptr = arr;
    
    printf("  arr = %p (数组首地址)\n", (void *)arr);
    printf("  &arr[0] = %p (首元素地址)\n", (void *)&arr[0]);
    printf("  ptr = %p (指针值)\n\n", (void *)ptr);
    
    printf("  arr[2] = %d\n", arr[2]);
    printf("  *(arr + 2) = %d\n", *(arr + 2));
    printf("  ptr[2] = %d\n", ptr[2]);
    printf("  *(ptr + 2) = %d\n", *(ptr + 2));

    printf("\n[sizeof的区别]\n");
    printf("  sizeof(arr) = %zu (整个数组大小)\n", sizeof(arr));
    printf("  sizeof(ptr) = %zu (指针大小)\n", sizeof(ptr));
    printf("  数组元素个数 = %zu\n", sizeof(arr) / sizeof(arr[0]));

    printf("\n[数组名是常量指针]\n");
    printf("  arr++ // 错误! 数组名不可修改\n");
    printf("  ptr++ // 正确! 指针可以修改\n");

    printf("\n[指针遍历数组]\n");
    printf("  使用指针遍历: ");
    for (int32_t *p = arr; p < arr + 5; p++) {
        printf("%d ", *p);
    }
    printf("\n");

    printf("\n[嵌入式应用]\n");
    printf("  - 数据缓冲区: uint8_t *buffer_ptr\n");
    printf("  - 硬件寄存器: volatile uint32_t *reg\n");
    printf("  - 字符串处理: char *str\n");
}

/*============================================================================*/
/*                           指针与函数演示                                     */
/*============================================================================*/

/**
 * @brief 指针与函数演示
 * @details 展示指针在函数中的应用：
 *          - 指针参数（修改实参）
 *          - 指针作为返回值
 *          - const指针参数
 *          - 函数指针
 * 
 * @note 指针参数可以修改实参的值
 * @note 返回指针需注意生命周期
 */
void demo_pointer_function(void)
{
    print_separator("指针与函数演示");

    printf("[指针参数 - 修改实参]\n");
    int32_t a = 10, b = 20;
    printf("  交换前: a = %d, b = %d\n", a, b);
    swap_values(&a, &b);
    printf("  交换后: a = %d, b = %d\n", a, b);

    printf("\n[指针参数 - 多返回值]\n");
    int32_t quotient, remainder;
    divide_with_remainder(17, 5, &quotient, &remainder);
    printf("  17 / 5 = %d 余 %d\n", quotient, remainder);

    printf("\n[const指针参数]\n");
    const char *str = "Hello";
    size_t len = safe_strlen(str);
    printf("  safe_strlen(\"%s\") = %zu\n", str, len);
    printf("  const char* 防止函数内部修改字符串\n");

    printf("\n[返回指针]\n");
    int32_t arr[] = {5, 2, 8, 1, 9};
    int32_t *max_ptr = find_max(arr, 5);
    if (max_ptr != NULL) {
        printf("  最大值: %d, 地址: %p\n", *max_ptr, (void *)max_ptr);
    }

    printf("\n[嵌入式应用]\n");
    printf("  - 修改硬件寄存器: void write_reg(uint32_t *reg, uint32_t value)\n");
    printf("  - 读取传感器数据: void read_sensor(SensorData_t *data)\n");
}

/**
 * @brief 交换两个变量的值
 * @param[in,out] a 第一个变量指针
 * @param[in,out] b 第二个变量指针
 */
void swap_values(int32_t *a, int32_t *b)
{
    if (a != NULL && b != NULL) {
        int32_t temp = *a;
        *a = *b;
        *b = temp;
    }
}

/**
 * @brief 除法运算，返回商和余数
 * @param[in] dividend 被除数
 * @param[in] divisor 除数
 * @param[out] quotient 商
 * @param[out] remainder 余数
 */
void divide_with_remainder(int32_t dividend, int32_t divisor, int32_t *quotient, int32_t *remainder)
{
    if (quotient != NULL && remainder != NULL && divisor != 0) {
        *quotient = dividend / divisor;
        *remainder = dividend % divisor;
    }
}

/**
 * @brief 安全的字符串长度计算
 * @param[in] str 字符串指针
 * @return 字符串长度
 */
size_t safe_strlen(const char *str)
{
    size_t len = 0;
    if (str != NULL) {
        while (str[len] != '\0') {
            len++;
        }
    }
    return len;
}

/**
 * @brief 查找数组中的最大值
 * @param[in] arr 数组指针
 * @param[in] size 数组大小
 * @return 最大值的指针，失败返回NULL
 */
int32_t *find_max(int32_t *arr, size_t size)
{
    if (arr == NULL || size == 0) {
        return NULL;
    }
    
    int32_t *max_ptr = arr;
    for (size_t i = 1; i < size; i++) {
        if (arr[i] > *max_ptr) {
            max_ptr = &arr[i];
        }
    }
    return max_ptr;
}

/*============================================================================*/
/*                           多级指针演示                                       */
/*============================================================================*/

/**
 * @brief 多级指针演示
 * @details 展示多级指针的使用：
 *          - 二级指针
 *          - 指针数组
 *          - 数组指针
 *          - 实际应用场景
 * 
 * @note 二级指针常用于动态分配二维数组
 * @note 指针数组常用于字符串数组
 */
void demo_multi_level_pointer(void)
{
    print_separator("多级指针演示");

    printf("[二级指针]\n");
    int32_t value = 42;
    int32_t *ptr = &value;
    int32_t **pptr = &ptr;
    
    printf("  int32_t value = 42;\n");
    printf("  int32_t *ptr = &value;\n");
    printf("  int32_t **pptr = &ptr;\n\n");
    
    printf("  value  = %d\n", value);
    printf("  *ptr   = %d\n", *ptr);
    printf("  **pptr = %d\n", **pptr);
    printf("  pptr指向ptr, ptr指向value\n");

    printf("\n[指针数组]\n");
    int32_t a = 10, b = 20, c = 30;
    int32_t *ptr_arr[3] = {&a, &b, &c};
    
    printf("  int32_t *ptr_arr[3] = {&a, &b, &c};\n");
    printf("  ptr_arr[0] = %p, *ptr_arr[0] = %d\n", (void *)ptr_arr[0], *ptr_arr[0]);
    printf("  ptr_arr[1] = %p, *ptr_arr[1] = %d\n", (void *)ptr_arr[1], *ptr_arr[1]);
    printf("  ptr_arr[2] = %p, *ptr_arr[2] = %d\n", (void *)ptr_arr[2], *ptr_arr[2]);

    printf("\n[字符串指针数组]\n");
    const char *names[] = {"Alice", "Bob", "Charlie", "David"};
    printf("  const char *names[] = {\"Alice\", \"Bob\", \"Charlie\", \"David\"};\n");
    for (size_t i = 0; i < 4; i++) {
        printf("  names[%zu] = \"%s\"\n", i, names[i]);
    }

    printf("\n[数组指针]\n");
    int32_t matrix[2][3] = {{1, 2, 3}, {4, 5, 6}};
    int32_t (*row_ptr)[3] = matrix;
    
    printf("  int32_t matrix[2][3] = {{1,2,3}, {4,5,6}};\n");
    printf("  int32_t (*row_ptr)[3] = matrix;\n");
    printf("  row_ptr[0][1] = %d\n", row_ptr[0][1]);
    printf("  row_ptr[1][2] = %d\n", row_ptr[1][2]);

    printf("\n[嵌入式应用]\n");
    printf("  - 命令表: const Command_t *cmd_table[]\n");
    printf("  - 字符串数组: const char *error_msgs[]\n");
    printf("  - 动态二维数组: int **matrix\n");
}

/*============================================================================*/
/*                           动态内存管理演示                                   */
/*============================================================================*/

/**
 * @brief 动态内存管理演示
 * @details 展示动态内存分配与释放：
 *          - malloc：分配内存
 *          - free：释放内存
 *          - calloc：分配并清零
 *          - realloc：重新分配
 *          - 内存泄漏检测
 * 
 * @note 动态内存必须成对使用malloc和free
 * @note 嵌入式开发中应谨慎使用动态内存
 * @note 内存泄漏是严重的资源泄漏问题
 */
void demo_dynamic_memory(void)
{
    print_separator("动态内存管理演示");

    printf("[malloc - 分配内存]\n");
    int32_t *ptr = (int32_t *)malloc(5 * sizeof(int32_t));
    if (ptr == NULL) {
        printf("  内存分配失败!\n");
        return;
    }
    
    printf("  int32_t *ptr = (int32_t*)malloc(5 * sizeof(int32_t));\n");
    printf("  分配地址: %p\n", (void *)ptr);
    
    for (int32_t i = 0; i < 5; i++) {
        ptr[i] = i * 10;
    }
    
    printf("  赋值后: ");
    for (int32_t i = 0; i < 5; i++) {
        printf("%d ", ptr[i]);
    }
    printf("\n");

    printf("\n[free - 释放内存]\n");
    free(ptr);
    ptr = NULL;
    printf("  free(ptr);\n");
    printf("  ptr = NULL; // 防止悬空指针\n");

    printf("\n[calloc - 分配并清零]\n");
    int32_t *cptr = (int32_t *)calloc(5, sizeof(int32_t));
    if (cptr != NULL) {
        printf("  int32_t *cptr = (int32_t*)calloc(5, sizeof(int32_t));\n");
        printf("  自动清零: ");
        for (int32_t i = 0; i < 5; i++) {
            printf("%d ", cptr[i]);
        }
        printf("\n");
        free(cptr);
        cptr = NULL;
    }

    printf("\n[realloc - 重新分配]\n");
    int32_t *rptr = (int32_t *)malloc(3 * sizeof(int32_t));
    if (rptr != NULL) {
        printf("  初始分配3个元素\n");
        
        rptr = (int32_t *)realloc(rptr, 6 * sizeof(int32_t));
        if (rptr != NULL) {
            printf("  重新分配为6个元素\n");
            free(rptr);
            rptr = NULL;
        }
    }

    printf("\n[内存泄漏示例]\n");
    printf("  错误: ptr = malloc(...); // 忘记free\n");
    printf("  正确: ptr = malloc(...); ... free(ptr); ptr = NULL;\n");

    printf("\n[嵌入式注意事项]\n");
    printf("  - 谨慎使用动态内存\n");
    printf("  - 使用内存池替代频繁malloc/free\n");
    printf("  - 检查返回值是否为NULL\n");
    printf("  - 成对使用malloc和free\n");
    printf("  - free后置指针为NULL\n");
}

/*============================================================================*/
/*                           常见指针错误演示                                   */
/*============================================================================*/

/**
 * @brief 常见指针错误演示
 * @details 展示指针使用中的常见错误：
 *          - 野指针：未初始化的指针
 *          - 悬空指针：指向已释放的内存
 *          - 内存泄漏：忘记释放内存
 *          - 越界访问：指针超出有效范围
 *          - 重复释放：多次free同一指针
 * 
 * @note 这些错误会导致未定义行为或程序崩溃
 * @note 必须养成良好的指针使用习惯
 */
void demo_pointer_errors(void)
{
    print_separator("常见指针错误演示");

    printf("[野指针 - 未初始化]\n");
    printf("  错误: int32_t *ptr; // 未初始化\n");
    printf("        *ptr = 10;   // 未定义行为!\n");
    printf("  正确: int32_t *ptr = NULL;\n");
    printf("        if (ptr != NULL) { *ptr = 10; }\n");

    printf("\n[悬空指针 - 指向已释放内存]\n");
    printf("  错误: int32_t *ptr = malloc(...);\n");
    printf("        free(ptr);\n");
    printf("        *ptr = 10; // ptr仍指向已释放的内存!\n");
    printf("  正确: free(ptr); ptr = NULL;\n");

    printf("\n[内存泄漏 - 忘记释放]\n");
    printf("  错误: void func() {\n");
    printf("          int32_t *ptr = malloc(...);\n");
    printf("          // 使用ptr\n");
    printf("          // 忘记free(ptr)\n");
    printf("        } // ptr丢失，内存泄漏!\n");
    printf("  正确: free(ptr); ptr = NULL;\n");

    printf("\n[越界访问]\n");
    printf("  错误: int32_t arr[5];\n");
    printf("        int32_t *ptr = arr;\n");
    printf("        ptr[10] = 100; // 越界!\n");
    printf("  正确: 检查索引范围\n");

    printf("\n[重复释放]\n");
    printf("  错误: int32_t *ptr = malloc(...);\n");
    printf("        free(ptr);\n");
    printf("        free(ptr); // 重复释放!\n");
    printf("  正确: free(ptr); ptr = NULL;\n");
    printf("        if (ptr != NULL) { free(ptr); } // 不会执行\n");

    printf("\n[最佳实践]\n");
    printf("  1. 指针初始化为NULL\n");
    printf("  2. 使用前检查NULL\n");
    printf("  3. free后立即置NULL\n");
    printf("  4. 成对使用malloc和free\n");
    printf("  5. 避免返回局部变量的指针\n");
    printf("  6. 使用const保护只读数据\n");
}

/*============================================================================*/
/*                           指针高级应用演示                                   */
/*============================================================================*/

/**
 * @brief 指针高级应用演示
 * @details 展示指针的高级应用：
 *          - 函数指针数组
 *          - 回调函数
 *          - 指针与结构体
 *          - 指针与const
 * 
 * @note 函数指针是嵌入式开发的核心技术
 * @note const指针提供更好的类型安全
 */
void demo_pointer_advanced(void)
{
    print_separator("指针高级应用演示");

    printf("[函数指针数组]\n");
    OperationFunc_t operations[] = {add_op, sub_op, mul_op, div_op};
    const char *op_names[] = {"加法", "减法", "乘法", "除法"};
    
    int32_t a = 20, b = 5;
    for (size_t i = 0; i < 4; i++) {
        printf("  %s: %d %s %d = %d\n", op_names[i], a, 
               (i == 0) ? "+" : (i == 1) ? "-" : (i == 2) ? "*" : "/", 
               b, operations[i](a, b));
    }

    printf("\n[回调函数]\n");
    int32_t arr[] = {5, 2, 8, 1, 9, 3};
    printf("  原数组: ");
    for (size_t i = 0; i < ARRAY_SIZE(arr); i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    process_with_callback(arr, ARRAY_SIZE(arr), double_value);
    printf("  处理后: ");
    for (size_t i = 0; i < ARRAY_SIZE(arr); i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    printf("\n[指针与const]\n");
    int32_t value = 10;
    int32_t other = 20;
    
    const int32_t *ptr_to_const = &value;
    printf("  const int32_t *ptr_to_const: 指向常量的指针\n");
    printf("    *ptr_to_const = 20; // 错误! 不能修改指向的值\n");
    printf("    ptr_to_const = &other; // 正确! 可以修改指针\n");
    
    int32_t *const const_ptr = &value;
    printf("  int32_t *const const_ptr: 常量指针\n");
    printf("    *const_ptr = 20; // 正确! 可以修改指向的值\n");
    printf("    const_ptr = &other; // 错误! 不能修改指针\n");
    
    const int32_t *const const_ptr_to_const = &value;
    printf("  const int32_t *const: 指向常量的常量指针\n");
    printf("    都不能修改\n");

    printf("\n[嵌入式应用]\n");
    printf("  - 硬件寄存器访问: volatile uint32_t *reg = (uint32_t*)0x40000000;\n");
    printf("  - 中断向量表: void (*isr_table[])(void)\n");
    printf("  - 设备驱动: typedef struct { void (*init)(void); ... } Driver_t;\n");
}

/**
 * @brief 加法运算
 */
int32_t add_op(int32_t a, int32_t b) { return a + b; }

/**
 * @brief 减法运算
 */
int32_t sub_op(int32_t a, int32_t b) { return a - b; }

/**
 * @brief 乘法运算
 */
int32_t mul_op(int32_t a, int32_t b) { return a * b; }

/**
 * @brief 除法运算
 */
int32_t div_op(int32_t a, int32_t b) { return (b != 0) ? a / b : 0; }

/**
 * @brief 数值翻倍
 */
static int32_t double_value(int32_t value) { return value * 2; }

/**
 * @brief 使用回调函数处理数组
 */
void process_with_callback(int32_t *arr, size_t size, ProcessCallback_t callback)
{
    if (arr != NULL && callback != NULL) {
        for (size_t i = 0; i < size; i++) {
            arr[i] = callback(arr[i]);
        }
    }
}
