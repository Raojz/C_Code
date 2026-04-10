/**
 * @file array_string.c
 * @brief 数组与字符串模块实现
 * @details 演示C语言数组与字符串的使用方法。
 *          本模块涵盖以下核心知识点：
 *          - 一维数组：声明、初始化、访问
 *          - 多维数组：二维数组、三维数组
 *          - 数组与指针的关系
 *          - 字符数组与字符串
 *          - 字符串操作函数
 *          - 字符串处理技巧
 * 
 * @section embedded_importance 嵌入式开发重要性
 * - 数组是数据存储的基础结构
 * - 字符串处理用于通信协议解析
 * - 缓冲区管理是嵌入式开发的核心技能
 * - 数组越界是常见的内存错误来源
 * 
 * @author Embedded C Learning Project
 * @date 2026-03-22
 * @version 1.0.0
 * 
 * @note 本文件遵循 MISRA-C:2012 编码规范
 */

#include "array_string.h"
#include "common/common_utils.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*============================================================================*/
/*                           一维数组演示                                       */
/*============================================================================*/

/**
 * @brief 一维数组演示
 * @details 展示一维数组的基本操作：
 *          - 数组声明与初始化
 *          - 数组元素的访问
 *          - 数组大小计算
 *          - 数组遍历
 * 
 * @note 数组大小必须在编译时确定（C99支持变长数组）
 * @note 数组名是首元素地址的常量
 */
void demo_one_dimensional_array(void)
{
    print_separator("一维数组演示");

    printf("[数组声明与初始化]\n");
    int32_t arr1[5] = {1, 2, 3, 4, 5};
    int32_t arr2[] = {10, 20, 30, 40, 50};
    int32_t arr3[5] = {0};
    int32_t arr4[5] = {1, 2};
    
    printf("  完全初始化: arr1[] = {1, 2, 3, 4, 5}\n");
    printf("  自动大小:   arr2[] = {10, 20, 30, 40, 50}\n");
    printf("  全零初始化: arr3[5] = {0}\n");
    printf("  部分初始化: arr4[5] = {1, 2} (其余为0)\n");

    printf("\n[数组元素访问]\n");
    printf("  arr1[0] = %d (第一个元素)\n", arr1[0]);
    printf("  arr1[4] = %d (最后一个元素)\n", arr1[4]);
    printf("  数组索引范围: 0 到 %zu\n", ARRAY_SIZE(arr1) - 1);

    printf("\n[数组大小计算]\n");
    size_t arr_bytes = sizeof(arr1);
    size_t elem_bytes = sizeof(arr1[0]);
    size_t elem_count = ARRAY_SIZE(arr1);
    printf("  sizeof(arr1) = %zu 字节\n", arr_bytes);
    printf("  sizeof(arr1[0]) = %zu 字节\n", elem_bytes);
    printf("  元素个数 = %zu / %zu = %zu\n", arr_bytes, elem_bytes, elem_count);

    printf("\n[数组遍历]\n");
    printf("  arr1: ");
    for (size_t i = 0; i < ARRAY_SIZE(arr1); i++) {
        printf("%d ", arr1[i]);
    }
    printf("\n");

    printf("\n[数组与指针关系]\n");
    int32_t *ptr = arr1;
    printf("  arr1 = %p (数组首地址)\n", (void *)arr1);
    printf("  &arr1[0] = %p (首元素地址)\n", (void *)&arr1[0]);
    printf("  ptr = arr1, ptr[2] = %d\n", ptr[2]);
    printf("  *(arr1 + 2) = %d (指针算术)\n", *(arr1 + 2));
}

/*============================================================================*/
/*                           多维数组演示                                       */
/*============================================================================*/

/**
 * @brief 多维数组演示
 * @details 展示多维数组的使用：
 *          - 二维数组声明与初始化
 *          - 二维数组访问
 *          - 三维数组示例
 *          - 内存布局
 * 
 * @note 多维数组在内存中按行优先存储
 * @note 二维数组常用于矩阵运算、图像处理
 */
void demo_multi_dimensional_array(void)
{
    print_separator("多维数组演示");

    printf("[二维数组声明与初始化]\n");
    int32_t matrix[3][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };
    printf("  int32_t matrix[3][4] = {{1,2,3,4}, {5,6,7,8}, {9,10,11,12}}\n");

    printf("\n[二维数组访问]\n");
    printf("  matrix[1][2] = %d (第2行第3列)\n", matrix[1][2]);
    printf("  行数: %zu, 列数: %zu\n", 
           sizeof(matrix) / sizeof(matrix[0]),
           sizeof(matrix[0]) / sizeof(matrix[0][0]));

    printf("\n[二维数组遍历]\n");
    for (size_t i = 0; i < 3; i++) {
        printf("  第%zu行: ", i);
        for (size_t j = 0; j < 4; j++) {
            printf("%2d ", matrix[i][j]);
        }
        printf("\n");
    }

    printf("\n[内存布局]\n");
    printf("  二维数组在内存中按行优先存储:\n");
    printf("  地址: %p -> %p -> ... -> %p\n", 
           (void *)&matrix[0][0], 
           (void *)&matrix[0][1],
           (void *)&matrix[2][3]);

    printf("\n[三维数组示例]\n");
    int32_t cube[2][2][3] = {
        {{1, 2, 3}, {4, 5, 6}},
        {{7, 8, 9}, {10, 11, 12}}
    };
    printf("  int32_t cube[2][2][3]\n");
    printf("  cube[0][1][2] = %d\n", cube[0][1][2]);

    printf("\n[嵌入式应用]\n");
    printf("  - 图像缓冲区: uint8_t image[HEIGHT][WIDTH]\n");
    printf("  - 矩阵运算: float matrix[N][M]\n");
    printf("  - 查找表: const uint16_t lut[256]\n");
}

/*============================================================================*/
/*                           字符数组与字符串演示                               */
/*============================================================================*/

/**
 * @brief 字符数组与字符串演示
 * @details 展示字符数组与字符串的关系：
 *          - 字符数组声明与初始化
 *          - 字符串的本质（以'\0'结尾的字符数组）
 *          - 字符串字面量
 *          - 字符数组与字符串的区别
 * 
 * @note 字符串必须以'\0'结尾
 * @note 字符串字面量存储在只读区
 */
void demo_character_array_string(void)
{
    print_separator("字符数组与字符串演示");

    printf("[字符数组初始化]\n");
    char char_arr1[5] = {'H', 'e', 'l', 'l', 'o'};
    char char_arr2[6] = {'H', 'e', 'l', 'l', 'o', '\0'};
    char char_arr3[] = "Hello";
    const char *str_literal = "Hello";
    
    printf("  char char_arr1[5] = {'H','e','l','l','o'} (字符数组, 非字符串)\n");
    printf("  char char_arr2[6] = {'H','e','l','l','o','\\0'} (字符串)\n");
    printf("  char char_arr3[] = \"Hello\" (字符串, 自动添加'\\0')\n");
    printf("  const char *str = \"Hello\" (字符串字面量, 只读)\n");

    printf("\n[字符串的本质]\n");
    printf("  字符串 = 以'\\0'结尾的字符数组\n");
    printf("  char_arr3 大小: %zu 字节 (包含'\\0')\n", sizeof(char_arr3));
    printf("  strlen(char_arr3): %zu (不含'\\0')\n", strlen(char_arr3));

    printf("\n[字符数组与字符串区别]\n");
    printf("  字符数组: 可以没有'\\0'结尾\n");
    printf("  字符串: 必须以'\\0'结尾\n");
    printf("  字符串字面量: 存储在只读区, 不可修改\n");

    printf("\n[字符串输出]\n");
    printf("  char_arr2: %s\n", char_arr2);
    printf("  char_arr3: %s\n", char_arr3);
    printf("  str_literal: %s\n", str_literal);

    printf("\n[注意事项]\n");
    printf("  - 字符数组大小要足够容纳'\\0'\n");
    printf("  - 字符串字面量不可修改\n");
    printf("  - 使用const char*指向字符串字面量\n");
}

/*============================================================================*/
/*                           字符串操作函数演示                                 */
/*============================================================================*/

/**
 * @brief 字符串操作函数演示
 * @details 展示常用字符串操作函数：
 *          - strlen: 字符串长度
 *          - strcpy/strncpy: 字符串复制
 *          - strcat/strncat: 字符串连接
 *          - strcmp/strncmp: 字符串比较
 *          - strchr/strrchr: 字符查找
 *          - strstr: 子串查找
 * 
 * @note 字符串操作函数需确保目标缓冲区足够大
 * @note 优先使用带n的版本（strncpy等）防止缓冲区溢出
 */
void demo_string_operations(void)
{
    print_separator("字符串操作函数演示");

    char buffer[100];

    printf("[strlen - 字符串长度]\n");
    const char *str1 = "Hello";
    printf("  strlen(\"%s\") = %zu\n", str1, strlen(str1));

    printf("\n[strcpy/strncpy - 字符串复制]\n");
    strcpy(buffer, "Hello");
    printf("  strcpy: buffer = \"%s\"\n", buffer);
    strncpy(buffer, "World", sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    printf("  strncpy: buffer = \"%s\"\n", buffer);

    printf("\n[strcat/strncat - 字符串连接]\n");
    strcpy(buffer, "Hello");
    strcat(buffer, " ");
    strcat(buffer, "World");
    printf("  strcat: buffer = \"%s\"\n", buffer);

    printf("\n[strcmp/strncmp - 字符串比较]\n");
    const char *s1 = "Apple";
    const char *s2 = "Banana";
    const char *s3 = "Apple";
    printf("  strcmp(\"%s\", \"%s\") = %d\n", s1, s2, strcmp(s1, s2));
    printf("  strcmp(\"%s\", \"%s\") = %d\n", s1, s3, strcmp(s1, s3));
    printf("  strncmp(\"%s\", \"%s\", 3) = %d\n", s1, s2, strncmp(s1, s2, 3));

    printf("\n[strchr/strrchr - 字符查找]\n");
    const char *text = "Hello World";
    char *pos = strchr(text, 'o');
    printf("  strchr(\"%s\", 'o') = %s\n", text, pos ? pos : "NULL");
    pos = strrchr(text, 'o');
    printf("  strrchr(\"%s\", 'o') = %s\n", text, pos ? pos : "NULL");

    printf("\n[strstr - 子串查找]\n");
    pos = strstr(text, "World");
    printf("  strstr(\"%s\", \"World\") = %s\n", text, pos ? pos : "NULL");

    printf("\n[安全建议]\n");
    printf("  - 使用strncpy替代strcpy\n");
    printf("  - 使用strncat替代strcat\n");
    printf("  - 确保目标缓冲区足够大\n");
    printf("  - 手动添加字符串结束符'\\0'\n");
}

/*============================================================================*/
/*                           字符串处理技巧演示                                 */
/*============================================================================*/

/**
 * @brief 字符串处理技巧演示
 * @details 展示实用的字符串处理技巧：
 *          - 字符串分割
 *          - 字符串转换
 *          - 字符串修剪
 *          - 字符串格式化
 * 
 * @note 字符串处理是嵌入式通信协议的基础
 * @note 注意缓冲区边界检查
 */
void demo_string_techniques(void)
{
    print_separator("字符串处理技巧演示");

    printf("[字符串分割]\n");
    char str[] = "Hello,World,Embedded,C";
    printf("  原字符串: \"%s\"\n", str);
    printf("  分割结果:\n");
    
    char *token = strtok(str, ",");
    while (token != NULL) {
        printf("    \"%s\"\n", token);
        token = strtok(NULL, ",");
    }

    printf("\n[字符串转换]\n");
    char lower[] = "hello world";
    char upper[20];
    for (size_t i = 0; i < strlen(lower); i++) {
        upper[i] = (char)toupper(lower[i]);
    }
    upper[strlen(lower)] = '\0';
    printf("  小写: \"%s\"\n", lower);
    printf("  大写: \"%s\"\n", upper);

    printf("\n[字符串修剪]\n");
    char trim_str[] = "  Hello World  ";
    printf("  原字符串: \"%s\"\n", trim_str);
    
    char *start = trim_str;
    while (isspace((unsigned char)*start)) {
        start++;
    }
    
    char *end = trim_str + strlen(trim_str) - 1;
    while (end > start && isspace((unsigned char)*end)) {
        end--;
    }
    *(end + 1) = '\0';
    
    printf("  修剪后: \"%s\"\n", start);

    printf("\n[字符串格式化]\n");
    char formatted[100];
    int32_t value = 42;
    float voltage = 3.3f;
    snprintf(formatted, sizeof(formatted), "Value=%d, Voltage=%.2fV", value, voltage);
    printf("  格式化字符串: \"%s\"\n", formatted);

    printf("\n[嵌入式应用]\n");
    printf("  - 协议解析: 分割命令和参数\n");
    printf("  - 数据转换: 字符串转数字\n");
    printf("  - 日志输出: 格式化日志信息\n");
}

/*============================================================================*/
/*                           数组排序演示                                       */
/*============================================================================*/

/**
 * @brief 数组排序演示
 * @details 展示常用的数组排序算法：
 *          - 冒泡排序
 *          - 选择排序
 *          - 插入排序
 *          - 快速排序
 * 
 * @note 排序算法选择需考虑数据规模和性能要求
 * @note 嵌入式开发中常用简单排序算法
 */
void demo_array_sorting(void)
{
    print_separator("数组排序演示");

    printf("[冒泡排序]\n");
    int32_t arr1[] = {64, 34, 25, 12, 22, 11, 90};
    size_t n1 = ARRAY_SIZE(arr1);
    
    printf("  排序前: ");
    print_array(arr1, n1);
    
    bubble_sort(arr1, n1);
    printf("  排序后: ");
    print_array(arr1, n1);

    printf("\n[选择排序]\n");
    int32_t arr2[] = {64, 34, 25, 12, 22, 11, 90};
    size_t n2 = ARRAY_SIZE(arr2);
    
    printf("  排序前: ");
    print_array(arr2, n2);
    
    selection_sort(arr2, n2);
    printf("  排序后: ");
    print_array(arr2, n2);

    printf("\n[插入排序]\n");
    int32_t arr3[] = {64, 34, 25, 12, 22, 11, 90};
    size_t n3 = ARRAY_SIZE(arr3);
    
    printf("  排序前: ");
    print_array(arr3, n3);
    
    insertion_sort(arr3, n3);
    printf("  排序后: ");
    print_array(arr3, n3);

    printf("\n[排序算法比较]\n");
    printf("  冒泡排序: O(n²), 稳定, 简单\n");
    printf("  选择排序: O(n²), 不稳定, 简单\n");
    printf("  插入排序: O(n²), 稳定, 适合小数据\n");
    printf("  快速排序: O(n log n), 不稳定, 高效\n");
}

/**
 * @brief 冒泡排序
 * @param[in,out] arr 待排序数组
 * @param[in] size 数组大小
 */
void bubble_sort(int32_t arr[], size_t size)
{
    if (arr == NULL || size == 0) {
        return;
    }
    
    for (size_t i = 0; i < size - 1; i++) {
        bool swapped = false;
        for (size_t j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int32_t temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = true;
            }
        }
        if (!swapped) {
            break;
        }
    }
}

/**
 * @brief 选择排序
 * @param[in,out] arr 待排序数组
 * @param[in] size 数组大小
 */
void selection_sort(int32_t arr[], size_t size)
{
    if (arr == NULL || size == 0) {
        return;
    }
    
    for (size_t i = 0; i < size - 1; i++) {
        size_t min_idx = i;
        for (size_t j = i + 1; j < size; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            int32_t temp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = temp;
        }
    }
}

/**
 * @brief 插入排序
 * @param[in,out] arr 待排序数组
 * @param[in] size 数组大小
 */
void insertion_sort(int32_t arr[], size_t size)
{
    if (arr == NULL || size == 0) {
        return;
    }
    
    for (size_t i = 1; i < size; i++) {
        int32_t key = arr[i];
        int32_t j = (int32_t)i - 1;
        
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

/**
 * @brief 打印数组
 * @param[in] arr 数组指针
 * @param[in] size 数组大小
 */
void print_array(const int32_t arr[], size_t size)
{
    if (arr != NULL) {
        for (size_t i = 0; i < size; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
    }
}

/*============================================================================*/
/*                           数组查找演示                                       */
/*============================================================================*/

/**
 * @brief 数组查找演示
 * @details 展示常用的数组查找算法：
 *          - 线性查找
 *          - 二分查找
 *          - 查找算法比较
 * 
 * @note 二分查找要求数组已排序
 * @note 线性查找适用于未排序数组
 */
void demo_array_searching(void)
{
    print_separator("数组查找演示");

    printf("[线性查找]\n");
    int32_t arr[] = {64, 34, 25, 12, 22, 11, 90};
    size_t size = ARRAY_SIZE(arr);
    int32_t target = 25;
    
    printf("  数组: ");
    print_array(arr, size);
    printf("  查找 %d: ", target);
    
    int32_t index = linear_search(arr, size, target);
    if (index >= 0) {
        printf("找到, 索引 %d\n", index);
    } else {
        printf("未找到\n");
    }

    printf("\n[二分查找]\n");
    int32_t sorted_arr[] = {11, 12, 22, 25, 34, 64, 90};
    size_t sorted_size = ARRAY_SIZE(sorted_arr);
    
    printf("  已排序数组: ");
    print_array(sorted_arr, sorted_size);
    printf("  查找 %d: ", target);
    
    index = binary_search(sorted_arr, sorted_size, target);
    if (index >= 0) {
        printf("找到, 索引 %d\n", index);
    } else {
        printf("未找到\n");
    }

    printf("\n[查找算法比较]\n");
    printf("  线性查找: O(n), 无需排序\n");
    printf("  二分查找: O(log n), 需要排序\n");
}

/**
 * @brief 线性查找
 * @param[in] arr 数组指针
 * @param[in] size 数组大小
 * @param[in] target 目标值
 * @return 找到返回索引，未找到返回-1
 */
int32_t linear_search(const int32_t arr[], size_t size, int32_t target)
{
    if (arr == NULL) {
        return -1;
    }
    
    for (size_t i = 0; i < size; i++) {
        if (arr[i] == target) {
            return (int32_t)i;
        }
    }
    return -1;
}

/**
 * @brief 二分查找
 * @param[in] arr 已排序数组指针
 * @param[in] size 数组大小
 * @param[in] target 目标值
 * @return 找到返回索引，未找到返回-1
 */
int32_t binary_search(const int32_t arr[], size_t size, int32_t target)
{
    if (arr == NULL || size == 0) {
        return -1;
    }
    
    int32_t left = 0;
    int32_t right = (int32_t)size - 1;
    
    while (left <= right) {
        int32_t mid = left + (right - left) / 2;
        
        if (arr[mid] == target) {
            return mid;
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    return -1;
}

/*============================================================================*/
/*                           数组常见错误演示                                   */
/*============================================================================*/

/**
 * @brief 数组常见错误演示
 * @details 展示数组使用中的常见错误：
 *          - 数组越界
 *          - 缓冲区溢出
 *          - 未初始化数组
 *          - 字符串未终止
 * 
 * @note 数组越界是严重的内存错误
 * @note 缓冲区溢出可能导致安全漏洞
 */
void demo_array_common_errors(void)
{
    print_separator("数组常见错误演示");

    printf("[数组越界]\n");
    int32_t arr[5] = {1, 2, 3, 4, 5};
    printf("  int32_t arr[5] = {1, 2, 3, 4, 5}\n");
    printf("  正确访问: arr[0] 到 arr[4]\n");
    printf("  错误访问: arr[5], arr[-1] (越界)\n");
    printf("  越界访问导致未定义行为\n");

    printf("\n[缓冲区溢出]\n");
    char buffer[5];
    printf("  char buffer[5]\n");
    printf("  错误: strcpy(buffer, \"Hello World\"); // 溢出\n");
    printf("  正确: strncpy(buffer, \"Hi\", 4); buffer[4] = '\\0';\n");

    printf("\n[未初始化数组]\n");
    int32_t uninit_arr[5];
    printf("  int32_t uninit_arr[5]; // 未初始化\n");
    printf("  内容不确定, 可能包含垃圾值\n");
    printf("  建议: int32_t arr[5] = {0}; // 全零初始化\n");

    printf("\n[字符串未终止]\n");
    char str[5] = {'H', 'e', 'l', 'l', 'o'};
    printf("  char str[5] = {'H','e','l','l','o'}; // 无'\\0'\n");
    printf("  strlen(str) 结果不确定\n");
    printf("  printf(\"%%s\", str) 可能输出乱码\n");

    printf("\n[最佳实践]\n");
    printf("  - 始终检查数组边界\n");
    printf("  - 使用带n的字符串函数\n");
    printf("  - 初始化所有数组\n");
    printf("  - 确保字符串以'\\0'结尾\n");
}
