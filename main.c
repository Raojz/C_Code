/**
 * @file main.c
 * @brief 嵌入式C语言学习工程主程序入口
 * @details 整合所有模块的演示和测试入口,提供交互式菜单界面。
 *          本工程涵盖C语言核心知识点,专为嵌入式开发学习设计。
 * 
 * @section project_structure 工程结构
 * - common/     : 公共类型定义和工具函数
 * - modules/    : 11个学习模块
 * - include/    : 头文件目录
 * - src/        : 源文件目录
 * 
 * @section modules 模块列表
 * 1. 基础语法模块     - 变量、数据类型、运算符
 * 2. 控制流模块       - 条件语句、循环结构
 * 3. 函数与作用域模块 - 函数定义、参数传递、作用域规则
 * 4. 数组与字符串模块 - 数组操作、字符串处理
 * 5. 指针与内存模块   - 指针运算、动态内存管理
 * 6. 结构体与联合体   - 结构体、联合体、枚举、位域
 * 7. 预处理指令模块   - 宏定义、条件编译、文件包含
 * 8. 文件I/O操作模块  - 文件读写、二进制操作
 * 9. 位运算与嵌入式   - 位操作、寄存器访问、中断处理
 * 10. 错误处理与调试  - 错误处理、断言、调试技术
 * 11. 数据结构模块    - 静态数组、栈、队列、链表
 * 
 * @section usage 使用方法
 * 交互模式:直接运行程序,通过菜单选择
 * 命令行模式:
 *   -d, --demo [N]  运行演示
 *   -t, --test [N]  运行测试
 *   -h, --help      显示帮助
 * 
 * @author Embedded C Learning Project
 * @date 2026-03-22
 * @version 1.0.0
 * 
 * @note 本工程遵循 MISRA-C:2012 编码规范
 * @note 支持 Windows 平台
 */

#include "common/common_types.h"
#include "common/common_utils.h"
#include "basic_syntax.h"
#include "control_flow.h"
#include "function_scope.h"
#include "array_string.h"
#include "pointer_memory.h"
#include "struct_union.h"
#include "preprocessor.h"
#include "file_io.h"
#include "bit_operation.h"
#include "error_debug.h"
#include "data_structure.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>

/*============================================================================*/
/*                           菜单选项定义                                       */
/*============================================================================*/

/** @brief 菜单选项:运行演示 */
#define MENU_OPTION_DEMO    1

/** @brief 菜单选项:退出程序 */
#define MENU_OPTION_EXIT    0

/*============================================================================*/
/*                           界面显示函数                                       */
/*============================================================================*/

/**
 * @brief 打印程序启动横幅
 * @details 显示ASCII艺术标题和版本信息
 */
static void print_banner(void)
{
    printf("\n");
    printf("================================================================================\n");
    printf("              嵌入式C语言系统化学习工程 v%s                              \n", VERSION_STRING);
    printf("================================================================================\n");
}

/**
 * @brief 打印模块列表
 * @details 显示所有可用模块的编号和描述
 */
static void print_module_list(void)
{
    printf("\n");
    printf("  可用模块列表:\n");
    printf("  ─────────────────────────────────────────────────────────────────────────────\n");
    printf("  [01] 基础语法模块     - 变量、数据类型、运算符\n");
    printf("  [02] 控制流模块       - 条件语句、循环结构\n");
    printf("  [03] 函数与作用域模块 - 函数定义、参数传递、作用域规则\n");
    printf("  [04] 数组与字符串模块 - 数组操作、字符串处理\n");
    printf("  [05] 指针与内存模块   - 指针运算、动态内存管理\n");
    printf("  [06] 结构体与联合体   - 结构体、联合体、枚举、位域\n");
    printf("  [07] 预处理指令模块   - 宏定义、条件编译、文件包含\n");
    printf("  [08] 文件I/O操作模块  - 文件读写、二进制操作\n");
    printf("  [09] 位运算与嵌入式   - 位操作、寄存器访问、中断处理\n");
    printf("  [10] 错误处理与调试   - 错误处理、断言、调试技术\n");
    printf("  [11] 数据结构模块     - 静态数组、栈、队列、链表\n");
    printf("  ─────────────────────────────────────────────────────────────────────────────\n");
}

/**
 * @brief 打印主菜单
 * @details 显示主菜单选项
 */
static void print_main_menu(void)
{
    printf("\n");
    printf("  主菜单:\n");
    printf("  ─────────────────────────────────────────────────────────────────────────────\n");
    printf("  [1] 运行模块演示\n");
    printf("  [0] 退出程序\n");
    printf("  ─────────────────────────────────────────────────────────────────────────────\n");
}

/*============================================================================*/
/*                           用户输入函数                                       */
/*============================================================================*/

/**
 * @brief 获取用户输入的选项
 * @details 从标准输入读取用户选择,验证输入范围
 * @param min_val 最小有效值
 * @param max_val 最大有效值
 * @return 用户输入的有效选项
 */
static int32_t get_user_choice(int32_t min_val, int32_t max_val)
{
    int32_t choice;
    char input[32];
    
    while (1) {
        printf("\n  请输入选项 [%d-%d]: ", min_val, max_val);
        if (fgets(input, sizeof(input), stdin) != NULL) {
            if (sscanf(input, "%d", &choice) == 1) {
                if (choice >= min_val && choice <= max_val) {
                    return choice;
                }
            }
        }
        printf("  输入无效, 请重新输入!\n");
    }
}

/**
 * @brief 获取用户选择的模块编号
 * @return 模块编号(1-11),0表示返回
 */
static int32_t get_module_choice(void)
{
    printf("\n  选择模块 [1-11, 0=返回]: ");
    int32_t choice = get_user_choice(0, 11);
    return choice;
}

/*============================================================================*/
/*                           模块运行函数                                       */
/*============================================================================*/

/**
 * @brief 运行指定模块的演示
 * @details 根据模块编号调用对应的演示函数
 * @param module_id 模块编号(1-10)
 */
static void run_module_demo(int32_t module_id)
{
    switch (module_id) {
        case 1:
            demo_data_types();
            demo_variable_scope();
            demo_storage_classes();
            demo_constant_types();
            demo_arithmetic_operators();
            demo_relational_operators();
            demo_logical_operators();
            demo_bitwise_operators();
            demo_assignment_operators();
            demo_conditional_operator();
            demo_type_conversion();
            demo_sizeof_operator();
            demo_operator_precedence();
            break;
        case 2:
            demo_if_statement();
            demo_switch_statement();
            demo_for_loop();
            demo_while_loop();
            demo_do_while_loop();
            demo_break_continue();
            demo_goto_statement();
            demo_nested_control();
            demo_state_machine();
            break;
        case 3:
            demo_function_basics();
            demo_parameter_passing();
            demo_multiple_return_values();
            demo_scope_rules();
            demo_storage_class();
            demo_recursion();
            demo_function_pointer();
            demo_variadic_function();
            break;
        case 4:
            demo_one_dimensional_array();
            demo_multi_dimensional_array();
            demo_character_array_string();
            demo_string_operations();
            demo_string_techniques();
            demo_array_sorting();
            demo_array_searching();
            demo_array_common_errors();
            break;
        case 5:
            demo_pointer_basics();
            demo_pointer_arithmetic();
            demo_pointer_array();
            demo_pointer_function();
            demo_multi_level_pointer();
            demo_dynamic_memory();
            demo_pointer_errors();
            demo_pointer_advanced();
            break;
        case 6:
            demo_struct_basics();
            demo_struct_pointer();
            demo_struct_array();
            demo_nested_struct();
            demo_union();
            demo_enum();
            demo_bit_field();
            demo_memory_alignment();
            demo_comprehensive();
            break;
        case 7:
            demo_macro_basics();
            demo_macro_advanced();
            demo_conditional_compile();
            demo_file_include();
            demo_predefined_macros();
            demo_pragma_directive();
            demo_compile_time_checks();
            demo_code_generation();
            break;
        case 8:
            demo_file_basics();
            demo_file_read_write();
            demo_file_positioning();
            demo_file_error_handling();
            demo_config_file();
            break;
        case 9:
            demo_bit_operators();
            demo_bit_tricks();
            demo_hardware_register();
            demo_bitmask_application();
            demo_bit_algorithms();
            break;
        case 10:
            demo_error_code();
            demo_assertion();
            demo_logging();
            demo_defensive_programming();
            demo_debug_techniques();
            demo_error_handling_patterns();
            break;
        case 11:
            demo_static_array();
            demo_stack();
            demo_queue();
            demo_linked_list();
            demo_data_structure_application();
            break;
        default:
            printf("  无效的模块ID: %d\n", module_id);
            break;
    }
}

/**
 * @brief 运行所有模块的演示
 * @details 依次运行1-11号模块的所有演示函数
 */
static void run_all_demos(void)
{
    print_separator("运行所有模块演示");
    
    for (int32_t i = 1; i <= 11; i++) {
        printf("\n");
        run_module_demo(i);
    }
}

/*============================================================================*/
/*                           交互模式函数                                       */
/*============================================================================*/

/**
 * @brief 演示模式交互界面
 * @details 显示模块列表,让用户选择要运行的演示
 */
static void demo_mode(void)
{
    print_module_list();
    
    while (1) {
        int32_t module_id = get_module_choice();
        
        if (module_id == 0) {
            break;
        }
        
        printf("\n");
        run_module_demo(module_id);
        
        printf("\n");
        printf("  演示完成, 按回车继续...");
        getchar();
        print_module_list();
    }
}

/*============================================================================*/
/*                           主函数                                             */
/*============================================================================*/

/**
 * @brief 程序主入口
 * @details 初始化系统,处理命令行参数,运行交互式菜单
 * 
 * @param argc 命令行参数个数
 * @param argv 命令行参数数组
 * @return 程序退出码:0表示正常退出
 * 
 * @section cmdline 命令行参数
 * - 无参数    : 进入交互模式
 * - -d, --demo [N] : 运行演示(N为模块号,省略则运行全部)
 * - -h, --help     : 显示帮助信息
 * 
 * @code
 * // 交互模式
 * ./embedded_c_learning
 * 
 * // 运行全部演示
 * ./embedded_c_learning --demo
 * 
 * // 运行指定模块演示
 * ./embedded_c_learning -d 5
 * @endcode
 */
int32_t main(int32_t argc, char *argv[])
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    print_banner();
    
    if (argc > 1) {
        if (strcmp(argv[1], "--demo") == 0 || strcmp(argv[1], "-d") == 0) {
            if (argc > 2) {
                int32_t module_id = atoi(argv[2]);
                if (module_id >= 1 && module_id <= 11) {
                    run_module_demo(module_id);
                    return 0;
                }
            }
            run_all_demos();
            return 0;
        }
        
        if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
            printf("\n  用法: %s [选项] [模块号]\n", argv[0]);
            printf("\n  选项:\n");
            printf("    -d, --demo [N]  运行演示 (N=1-11, 省略则运行全部)\n");
            printf("    -h, --help      显示帮助信息\n");
            printf("\n  模块号:\n");
            printf("    1-基础语法  2-控制流  3-函数作用域  4-数组字符串  5-指针内存\n");
            printf("    6-结构联合  7-预处理  8-文件I/O     9-位运算     10-错误调试\n");
            printf("    11-数据结构\n");
            return 0;
        }
    }
    
    while (1) {
        print_main_menu();
        int32_t choice = get_user_choice(0, 1);
        
        switch (choice) {
            case MENU_OPTION_DEMO:
                demo_mode();
                break;
            case MENU_OPTION_EXIT:
                printf("\n  感谢使用嵌入式C语言学习工程!\n\n");
                return 0;
            default:
                break;
        }
    }
    
    return 0;
}
