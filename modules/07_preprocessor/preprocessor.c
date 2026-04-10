/**
 * @file preprocessor.c
 * @brief 预处理指令模块实现
 * @details 演示C语言预处理指令的使用,涵盖以下核心内容:
 *          - 宏定义基础:#define、#undef、对象宏、函数宏
 *          - 宏高级技巧:字符串化#、连接##、可变参数
 *          - 条件编译:#if、#ifdef、#ifndef、#elif、#endif
 *          - 文件包含:#include、重复包含防护
 *          - 预定义宏:__FILE__、__LINE__、__DATE__等
 *          - #pragma指令:编译器特定指令
 *          - 编译时检查:静态断言、编译时计算
 *          - 代码生成:X-Macro技术
 * 
 * @section embedded_importance 嵌入式开发重要性
 * - 硬件抽象:通过宏定义寄存器地址
 * - 平台适配:条件编译适配不同芯片
 * - 调试开关:DEBUG宏控制调试代码
 * - 代码优化:内联函数、常量折叠
 * - 配置管理:通过宏配置功能开关
 * 
 * @section best_practices 最佳实践
 * - 宏定义使用大写命名
 * - 函数宏参数必须加括号
 * - 使用do-while(0)包裹多语句宏
 * - 避免有副作用的宏参数
 * - 复杂逻辑优先使用内联函数
 * 
 * @author Embedded C Learning Project
 * @date 2026-03-22
 * @version 1.0.0
 * 
 * @warning 宏定义不进行类型检查,需谨慎使用
 * @note 宏展开是文本替换,注意副作用
 * @note 本文件遵循 MISRA-C:2012 编码规范
 */

#include "preprocessor.h"
#include "common/common_utils.h"
#include <stdio.h>
#include <string.h>

/*============================================================================*/
/*                           宏模板实例化                                       */
/*============================================================================*/

/** @brief 实例化int类型的MIN函数 */
MIN_IMPL(int)

/** @brief 实例化float类型的MIN函数 */
MIN_IMPL(float)

/*============================================================================*/
/*                           X-Macro定义                                        */
/*============================================================================*/

/**
 * @def FRUIT_CONCAT(a, b)
 * @brief 内部连接宏
 */
#define FRUIT_CONCAT(a, b) a##b

/**
 * @def FRUIT_NAME(name)
 * @brief 生成水果枚举名称
 */
#define FRUIT_NAME(name) FRUIT_CONCAT(FRUIT_, name)

/**
 * @def FRUIT_LIST
 * @brief X-Macro水果列表定义
 * @details 使用X-Macro技术定义水果列表,避免重复代码。
 *          X()宏可以多次定义,生成不同的代码。
 */
#define FRUIT_LIST \
    X(APPLE, "苹果", 1) \
    X(ORANGE, "橙子", 2) \
    X(BANANA, "香蕉", 3)

/**
 * @brief 水果类型枚举
 * @details 使用X-Macro自动生成枚举定义
 */
typedef enum {
    #define X(name, str, val) FRUIT_NAME(name) = val,
    FRUIT_LIST
    #undef X
    FRUIT_MAX
} Fruit_t;

/**
 * @brief 水果名称字符串数组
 * @details 使用X-Macro自动生成字符串数组
 */
static const char *g_fruit_names[] = {
    #define X(name, str, val) [val] = str,
    FRUIT_LIST
    #undef X
};

/*============================================================================*/
/*                           演示函数实现                                       */
/*============================================================================*/

/**
 * @brief 宏定义基础演示
 * @details 展示宏定义的基本用法:
 *          - 对象宏:#define PI 3.14159
 *          - 函数宏:#define MAX(a,b) ((a)>(b)?(a):(b))
 *          - #undef取消定义
 *          - 宏的作用域
 */
void demo_macro_basics(void)
{
    print_separator("宏定义基础演示");

    printf("[基本宏定义]\n");
    printf("  #define PI 3.14159f\n");
    printf("  #define MAX_SIZE 100\n");
    printf("  PI = %f\n", 3.14159f);
    printf("  MAX_SIZE = %d\n", 100);

    printf("\n[函数式宏]\n");
    printf("  #define MAX(a, b) ((a) > (b) ? (a) : (b))\n");
    printf("  #define SQUARE(x) ((x) * (x))\n");
    printf("  MAX(3, 5) = %d\n", MAX(3, 5));
    printf("  SQUARE(4) = %d\n", ((4) * (4)));

    printf("\n[宏的括号重要性]\n");
    printf("  #define DOUBLE(x) x * x  // 错误!\n");
    printf("  DOUBLE(1+2) = 1+2 * 1+2 = %d (期望9)\n", 1+2 * 1+2);
    printf("  #define DOUBLE(x) ((x) * (x))  // 正确\n");
    printf("  DOUBLE(1+2) = ((1+2) * (1+2)) = %d\n", ((1+2) * (1+2)));

    printf("\n[多行宏]\n");
    printf("  #define SWAP(a, b, type) \\\n");
    printf("      do { \\\n");
    printf("          type temp = a; \\\n");
    printf("          a = b; \\\n");
    printf("          b = temp; \\\n");
    printf("      } while(0)\n");

    printf("\n[版本号宏]\n");
    printf("  VERSION_MAJOR = %d\n", VERSION_MAJOR);
    printf("  VERSION_MINOR = %d\n", VERSION_MINOR);
    printf("  VERSION_PATCH = %d\n", VERSION_PATCH);
    printf("  VERSION_STRING = %s\n", VERSION_STRING);
}

void demo_macro_advanced(void)
{
    print_separator("高级宏技巧演示");

    printf("[字符串化操作符 #]\n");
    printf("  #define STRINGIFY(x) #x\n");
    printf("  STRINGIFY(hello) = \"%s\"\n", STRINGIFY(hello));
    printf("  STRINGIFY(123) = \"%s\"\n", STRINGIFY(123));

    printf("\n[连接操作符 ##]\n");
    printf("  #define CONCAT(a, b) a##b\n");
    int32_t CONCAT(var, 1) = 100;
    int32_t CONCAT(var, 2) = 200;
    printf("  CONCAT(var, 1) = var1 = %d\n", var1);
    printf("  CONCAT(var, 2) = var2 = %d\n", var2);

    printf("\n[可变参数宏]\n");
    printf("  #define DEBUG_PRINT(fmt, ...) printf(fmt, ##__VA_ARGS__)\n");
    printf("  ##__VA_ARGS__ 在无额外参数时删除前面的逗号\n");

    printf("\n[宏生成函数]\n");
    printf("  MIN_IMPL(int) 生成 MIN_int(int, int)\n");
    printf("  MIN_int(3, 5) = %d\n", MIN_int(3, 5));
    printf("  MIN_IMPL(float) 生成 MIN_float(float, float)\n");
    printf("  MIN_float(3.5f, 2.1f) = %.1f\n", MIN_float(3.5f, 2.1f));

    printf("\n[条件表达式宏]\n");
    printf("  #define IIF(cond, a, b) ((cond) ? (a) : (b))\n");
    printf("  IIF(1, \"yes\", \"no\") = %s\n", (1) ? "yes" : "no");

    printf("\n[参数计数宏]\n");
    printf("  COUNT_ARGS(a) = %d\n", COUNT_ARGS(a));
    printf("  COUNT_ARGS(a, b) = %d\n", COUNT_ARGS(a, b));
    printf("  COUNT_ARGS(a, b, c) = %d\n", COUNT_ARGS(a, b, c));
}

void demo_conditional_compile(void)
{
    print_separator("条件编译演示");

    printf("[#ifdef / #ifndef]\n");
#ifdef DEBUG_MODE
    printf("  DEBUG_MODE 已定义\n");
#else
    printf("  DEBUG_MODE 未定义\n");
#endif

#ifndef RELEASE_MODE
    printf("  RELEASE_MODE 未定义\n");
#endif

    printf("\n[#if / #elif / #else / #endif]\n");
#if VERSION_MAJOR > 0
    printf("  VERSION_MAJOR > 0: 版本号有效\n");
#else
    printf("  VERSION_MAJOR <= 0: 版本号无效\n");
#endif

#if VERSION_MAJOR == 1 && VERSION_MINOR >= 0
    printf("  版本 >= 1.0\n");
#endif

    printf("\n[defined操作符]\n");
#if defined(DEBUG_MODE) || defined(TEST_MODE)
    printf("  调试或测试模式\n");
#else
    printf("  正式发布模式\n");
#endif

    printf("\n[平台条件编译]\n");
    printf("  平台: Windows\n");

    printf("\n[编译器条件编译]\n");
#ifdef _MSC_VER
    printf("  编译器: MSVC %d\n", _MSC_VER);
#elif defined(__GNUC__)
    printf("  编译器: GCC %d.%d.%d\n", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
#else
    printf("  编译器: 未知\n");
#endif

    printf("\n[嵌入式应用]\n");
    printf("  #ifdef USE_HAL_DRIVER\n");
    printf("  #ifdef STM32F407xx\n");
    printf("  #define DEBUG  // 调试时定义\n");
}

void demo_file_include(void)
{
    print_separator("文件包含演示");

    printf("[#include <file>]\n");
    printf("  从系统标准路径搜索\n");
    printf("  用于标准库头文件\n");
    printf("  例: #include <stdio.h>\n");

    printf("\n[#include \"file\"]\n");
    printf("  先搜索当前目录, 再搜索系统路径\n");
    printf("  用于项目自定义头文件\n");
    printf("  例: #include \"myheader.h\"\n");

    printf("\n[头文件保护]\n");
    printf("  方式1: #ifndef / #define / #endif\n");
    printf("    #ifndef MYHEADER_H\n");
    printf("    #define MYHEADER_H\n");
    printf("    // 内容\n");
    printf("    #endif\n");

    printf("\n  方式2: #pragma once (非标准但广泛支持)\n");
    printf("    #pragma once\n");
    printf("    // 内容\n");

    printf("\n[头文件组织原则]\n");
    printf("  1. 头文件只放声明, 不放定义\n");
    printf("  2. 最小化头文件依赖\n");
    printf("  3. 使用前向声明减少包含\n");
    printf("  4. 接口与实现分离\n");

    printf("\n[包含顺序建议]\n");
    printf("  1. 对应的头文件 (myclass.h)\n");
    printf("  2. 项目头文件\n");
    printf("  3. 第三方库头文件\n");
    printf("  4. 系统头文件\n");
}

void demo_predefined_macros(void)
{
    print_separator("预定义宏演示");

    printf("[文件和行号]\n");
    printf("  __FILE__ = %s\n", __FILE__);
    printf("  __LINE__ = %d\n", __LINE__);
    printf("  用于调试和错误报告\n");

    printf("\n[函数信息]\n");
    printf("  __func__ = %s\n", __func__);
    printf("  __FUNCTION__ = %s (扩展)\n", __FUNCTION__);

    printf("\n[日期和时间]\n");
    printf("  __DATE__ = %s (编译日期)\n", __DATE__);
    printf("  __TIME__ = %s (编译时间)\n", __TIME__);

    printf("\n[调试宏示例]\n");
    printf("  #define LOG_ERROR(msg) \\\n");
    printf("      printf(\"[ERROR] %%s:%%d %%s: %%s\\n\", \\\n");
    printf("             __FILE__, __LINE__, __func__, msg)\n");

    printf("\n[断言宏]\n");
    printf("  #define ASSERT(cond) \\\n");
    printf("      do { \\\n");
    printf("          if (!(cond)) { \\\n");
    printf("              printf(\"Assertion failed: %%s, %%s:%%d\\n\", \\\n");
    printf("                     #cond, __FILE__, __LINE__); \\\n");
    printf("          } \\\n");
    printf("      } while(0)\n");

    printf("\n[编译器特定宏]\n");
#ifdef __COUNTER__
    printf("  __COUNTER__ = %d (每次使用递增)\n", __COUNTER__);
    printf("  __COUNTER__ = %d\n", __COUNTER__);
#endif
}

void demo_pragma_directive(void)
{
    print_separator("#pragma指令演示");

    printf("[#pragma once]\n");
    printf("  确保头文件只被包含一次\n");
    printf("  比#ifndef更简洁, 但非标准\n");

    printf("\n[#pragma pack]\n");
    printf("  #pragma pack(push, 1)  // 保存当前对齐, 设为1字节对齐\n");
    printf("  struct Packed { ... };\n");
    printf("  #pragma pack(pop)  // 恢复对齐\n");

    printf("\n[#pragma message]\n");
    printf("  #pragma message(\"编译时提示信息\")\n");
    printf("  编译时输出自定义消息\n");

    printf("\n[#pragma warning]\n");
    printf("  #pragma warning(disable: 4996)  // 禁用特定警告\n");
    printf("  #pragma warning(push)\n");
    printf("  #pragma warning(pop)\n");

    printf("\n[嵌入式常用pragma]\n");
    printf("  #pragma location = 0x20000000  // IAR: 指定变量位置\n");
    printf("  #pragma section(\".mysection\")  // 指定段\n");
}

void demo_compile_time_checks(void)
{
    print_separator("编译时检查演示");

    printf("[静态断言]\n");
    printf("  _Static_assert(condition, message);\n");
    printf("  C11标准, 编译时检查条件\n");
    printf("  MSVC使用: STATIC_ASSERT(condition, message)\n");

    printf("\n[类型大小检查]\n");
    STATIC_ASSERT(sizeof(int32_t) == 4, "int32_t must be 4 bytes");
    STATIC_ASSERT(sizeof(void*) >= 4, "Pointer must be at least 4 bytes");
    printf("  类型大小检查通过\n");

    printf("\n[数组大小检查]\n");
    printf("  int32_t arr[10];\n");
    printf("  STATIC_ASSERT(sizeof(arr) / sizeof(arr[0]) == 10, \"Array must have 10 elements\");\n");
    printf("  数组大小检查通过\n");

    printf("\n[编译时计算]\n");
    printf("  #define COMPILE_TIME_ASSERT(cond) \\\n");
    printf("      typedef char static_assert_##__LINE__[(cond) ? 1 : -1]\n");
    printf("  C11之前使用数组大小技巧\n");

    printf("\n[枚举值检查]\n");
    typedef enum {
        VALUE_A = 0,
        VALUE_B = 1,
        VALUE_MAX = 2
    } TestEnum_t;
    STATIC_ASSERT(VALUE_MAX == 2, "VALUE_MAX must be 2");
    printf("  枚举值检查通过\n");
}

void demo_code_generation(void)
{
    print_separator("代码生成演示");

    printf("[X-Macro模式]\n");
    printf("  定义数据列表:\n");
    printf("    // sensors.def\n");
    printf("    SENSOR(TEMP, \"温度\", 0)\n");
    printf("    SENSOR(HUMIDITY, \"湿度\", 1)\n");
    printf("    SENSOR(PRESSURE, \"压力\", 2)\n");

    printf("\n  生成枚举:\n");
    printf("    typedef enum {\n");
    printf("      #define SENSOR(name, str, id) SENSOR_##name = id,\n");
    printf("      #include \"sensors.def\"\n");
    printf("      #undef SENSOR\n");
    printf("    } SensorType_t;\n");

    printf("\n  生成字符串数组:\n");
    printf("    const char *sensor_names[] = {\n");
    printf("      #define SENSOR(name, str, id) [id] = str,\n");
    printf("      #include \"sensors.def\"\n");
    printf("      #undef SENSOR\n");
    printf("    };\n");

    printf("\n[简化示例]\n");
    printf("  枚举值: APPLE=%d, ORANGE=%d, BANANA=%d\n", 
           FRUIT_APPLE, FRUIT_ORANGE, FRUIT_BANANA);
    printf("  名称: %s, %s, %s\n", 
           g_fruit_names[FRUIT_APPLE], 
           g_fruit_names[FRUIT_ORANGE], 
           g_fruit_names[FRUIT_BANANA]);

    printf("\n[优点]\n");
    printf("  - 单一数据源, 易于维护\n");
    printf("  - 自动同步枚举和字符串\n");
    printf("  - 减少重复代码\n");
}
