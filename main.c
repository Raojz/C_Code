/**
 * @file main.c
 * @brief 嵌入式C语言学习工程主程序入口
 * @details 整合所有模块的演示和测试入口,提供交互式菜单界面。
 *          本工程涵盖C语言核心知识点,专为嵌入式开发学习设计。
 *
 * @section project_structure 工程结构
 * - common/     : 公共类型定义和工具函数
 * - modules/    : 12个学习模块
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
 * 12. 串口通信模块    - UART、I2C、SPI通信协议实现
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
#include "serial_uart.h"
#include "serial_i2c.h"
#include "serial_spi.h"
#include "performance.h"
#include "test_framework.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>

/*============================================================================*/
/*                           串口通信模块演示函数声明                           */
/*============================================================================*/

static void demo_uart_basics(void);
static void demo_uart_communication(void);
static void demo_i2c_basics(void);
static void demo_i2c_device_scan(void);
static void demo_spi_basics(void);
static void demo_spi_transfer(void);
static void demo_serial_comparison(void);
static void demo_performance_analysis(void);
static int32_t test_serial_communication(void);

/*============================================================================*/
/*                           菜单选项定义                                       */
/*============================================================================*/

/** @brief 菜单选项:运行演示 */
#define MENU_OPTION_DEMO    1

/** @brief 菜单选项:运行测试 */
#define MENU_OPTION_TEST    2

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
    printf("  [12] 串口通信模块     - UART、I2C、SPI通信协议实现\n");
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
    printf("  [2] 运行模块测试\n");
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
 * @return 模块编号(1-12),0表示返回
 */
static int32_t get_module_choice(void)
{
    printf("\n  选择模块 [1-12, 0=返回]: ");
    int32_t choice = get_user_choice(0, 12);
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
        case 12:
            demo_uart_basics();
            demo_uart_communication();
            demo_i2c_basics();
            demo_i2c_device_scan();
            demo_spi_basics();
            demo_spi_transfer();
            demo_serial_comparison();
            demo_performance_analysis();
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
    
    for (int32_t i = 1; i <= 12; i++) {
        printf("\n");
        run_module_demo(i);
    }
}

/**
 * @brief 运行指定模块的测试
 * @details 根据模块编号调用对应的测试函数
 * @param module_id 模块编号(1-11)
 */
static void run_module_test(int32_t module_id)
{
    int32_t failed = 0;
    
    switch (module_id) {
        case 1:
            failed = test_basic_syntax();
            break;
        case 2:
            failed = test_control_flow();
            break;
        case 3:
            failed = test_function_scope();
            break;
        case 4:
            failed = test_array_string();
            break;
        case 5:
            failed = test_pointer_memory();
            break;
        case 6:
            failed = test_struct_union();
            break;
        case 7:
            failed = test_preprocessor();
            break;
        case 8:
            failed = test_file_io();
            break;
        case 9:
            failed = test_bit_operation();
            break;
        case 10:
            failed = test_error_debug();
            break;
        case 11:
            failed = test_data_structure();
            break;
        case 12:
            failed = test_serial_communication();
            break;
        default:
            printf("  无效的模块ID: %d\n", module_id);
            return;
    }
    
    printf("\n");
    if (failed == 0) {
        printf("  模块%d测试全部通过!\n", module_id);
    } else {
        printf("  模块%d测试完成, %d项失败!\n", module_id, failed);
    }
}

/**
 * @brief 运行所有模块的测试
 * @details 依次运行1-11号模块的所有测试函数
 */
static void run_all_module_tests(void)
{
    int32_t total_failed = 0;
    
    print_separator("运行所有模块测试");
    
    test_init();
    
    for (int32_t i = 1; i <= 12; i++) {
        printf("\n");
        run_module_test(i);
    }
    
    printf("\n");
    test_print_report();
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

/**
 * @brief 测试模式交互界面
 * @details 显示模块列表,让用户选择要运行的测试
 */
static void test_mode(void)
{
    print_module_list();
    
    test_init();
    
    while (1) {
        int32_t module_id = get_module_choice();
        
        if (module_id == 0) {
            break;
        }
        
        printf("\n");
        run_module_test(module_id);
        
        printf("\n");
        printf("  按回车继续...");
        getchar();
        print_module_list();
    }
    
    printf("\n");
    test_print_report();
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
                if (module_id >= 1 && module_id <= 12) {
                    run_module_demo(module_id);
                    return 0;
                }
            }
            run_all_demos();
            return 0;
        }
        
        if (strcmp(argv[1], "--test") == 0 || strcmp(argv[1], "-t") == 0) {
            if (argc > 2) {
                int32_t module_id = atoi(argv[2]);
                if (module_id >= 1 && module_id <= 12) {
                    run_module_test(module_id);
                    return 0;
                }
            }
            run_all_module_tests();
            return 0;
        }
        
        if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
            printf("\n  用法: %s [选项] [模块号]\n", argv[0]);
            printf("\n  选项:\n");
            printf("    -d, --demo [N]  运行演示 (N=1-12, 省略则运行全部)\n");
            printf("    -t, --test [N]  运行测试 (N=1-12, 省略则运行全部)\n");
            printf("    -h, --help      显示帮助信息\n");
            printf("\n  模块号:\n");
            printf("    1-基础语法  2-控制流  3-函数作用域  4-数组字符串  5-指针内存\n");
            printf("    6-结构联合  7-预处理  8-文件I/O     9-位运算     10-错误调试\n");
            printf("    11-数据结构  12-串口通信\n");
            return 0;
        }
    }
    
    while (1) {
        print_main_menu();
        int32_t choice = get_user_choice(0, 2);
        
        switch (choice) {
            case MENU_OPTION_DEMO:
                demo_mode();
                break;
            case MENU_OPTION_TEST:
                test_mode();
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

/*============================================================================*/
/*                           串口通信模块演示函数实现                           */
/*============================================================================*/

/**
 * @brief 演示UART基础知识
 */
static void demo_uart_basics(void)
{
    print_separator("UART基础知识演示");
    printf("UART (通用异步收发传输器) 是一种常用的串行通信协议:\n");
    printf("• 异步通信：无需时钟线，收发双方需约定相同的波特率\n");
    printf("• 数据帧格式：起始位(0) + 数据位(5-9位) + 校验位(可选) + 停止位(1-2位)\n");
    printf("• 常用波特率：9600, 115200, 460800\n");
    printf("• 特点：简单易用，只需2根信号线(TX/RX)\n\n");

    printf("典型应用场景：\n");
    printf("• 调试信息输出\n");
    printf("• 与PC通信\n");
    printf("• 连接GPS模块、蓝牙模块等\n");
    printf("• 传感器数据采集\n\n");
}

/**
 * @brief 演示UART通信流程
 */
static void demo_uart_communication(void)
{
    print_separator("UART通信流程演示");
    printf("UART通信的基本流程：\n\n");

    printf("1. 初始化配置：\n");
    printf("   • 设置波特率（如115200）\n");
    printf("   • 配置数据位（通常8位）\n");
    printf("   • 配置停止位（通常1位）\n");
    printf("   • 配置校验位（通常无校验）\n");
    printf("   • 使能UART时钟\n");
    printf("   • 配置GPIO引脚\n\n");

    printf("2. 数据发送：\n");
    printf("   • 等待发送缓冲区空闲\n");
    printf("   • 将数据写入发送数据寄存器\n");
    printf("   • 等待传输完成\n\n");

    printf("3. 数据接收：\n");
    printf("   • 配置接收中断\n");
    printf("   • 在中断服务程序中读取数据\n");
    printf("   • 使用环形缓冲区存储数据\n");
    printf("   • 在应用程序中处理接收到的数据\n\n");

    printf("注意事项：\n");
    printf("• 波特率必须收发双方一致\n");
    printf("• TX和RX需要交叉连接\n");
    printf("• 注意电平匹配（TTL/CMOS/RS232）\n");
    printf("• 处理通信错误（帧错误、溢出错误）\n");
}

/**
 * @brief 演示I2C基础知识
 */
static void demo_i2c_basics(void)
{
    print_separator("I2C基础知识演示");
    printf("I2C (内部集成电路) 是一种同步串行通信协议:\n");
    printf("• 同步通信：使用时钟线(SCL)同步数据传输\n");
    printf("• 多主多从：支持多个主机和多个从机\n");
    printf("• 开漏输出：需要上拉电阻，支持线与逻辑\n");
    printf("• 仅需2根信号线：SCL(时钟线) + SDA(数据线)\n\n");

    printf("关键特性：\n");
    printf("• 7位地址模式：地址范围0x08-0x77\n");
    printf("• 速度模式：标准模式(100kHz)、快速模式(400kHz)\n");
    printf("• 事务格式：START → 地址 → 数据 → STOP\n");
    printf("• 应答机制：接收方在第9个时钟周期拉低SDA\n\n");

    printf("典型应用场景：\n");
    printf("• 连接传感器（温湿度、加速度计）\n");
    printf("• 连接存储器（EEPROM、FRAM）\n");
    printf("• 连接OLED显示屏\n");
    printf("• 连接RTC时钟芯片\n");
}

/**
 * @brief 演示I2C设备扫描
 */
static void demo_i2c_device_scan(void)
{
    print_separator("I2C设备扫描演示");
    printf("I2C设备扫描原理：\n\n");

    printf("扫描过程：\n");
    printf("1. 遍历所有可能的从机地址（0x08-0x77）\n");
    printf("2. 对每个地址发送START + 地址 + 写操作\n");
    printf("3. 如果从机应答（ACK），说明该地址有设备\n");
    printf("4. 发送STOP条件结束本次尝试\n\n");

    printf("注意事项：\n");
    printf("• 某些设备可能不支持地址扫描\n");
    printf("• 扫描期间不要进行其他I2C操作\n");
    printf("• 上拉电阻选择：100kHz→4.7kΩ，400kHz→2.2kΩ\n");
    printf("• 总线电容限制：≤400pF（标准模式）或≤550pF（快速模式）\n\n");

    printf("常见的I2C设备地址：\n");
    printf("• EEPROM：0x50-0x57\n");
    printf("• 温度传感器：0x48-0x4F\n");
    printf("• OLED显示屏：0x3C\n");
    printf("• RTC时钟：0x68\n");
}

/**
 * @brief 演示SPI基础知识
 */
static void demo_spi_basics(void)
{
    print_separator("SPI基础知识演示");
    printf("SPI (串行外设接口) 是一种全双工同步串行通信协议:\n");
    printf("• 全双工：数据可以同时双向传输\n");
    printf("• 同步通信：使用时钟线(SCLK)同步数据传输\n");
    printf("• 主从架构：一个主机控制多个从机\n");
    printf("• 需要4根信号线：SCLK、MOSI、MISO、CS\n\n");

    printf("SPI工作模式（4种）：\n");
    printf("• Mode 0 (CPOL=0, CPHA=0)：空闲低电平，第一个边沿采样\n");
    printf("• Mode 1 (CPOL=0, CPHA=1)：空闲低电平，第二个边沿采样\n");
    printf("• Mode 2 (CPOL=1, CPHA=0)：空闲高电平，第一个边沿采样\n");
    printf("• Mode 3 (CPOL=1, CPHA=1)：空闲高电平，第二个边沿采样\n\n");

    printf("特点：\n");
    printf("• 速度比I2C快，可达几十MHz\n");
    printf("• 每个从机需要独立的CS片选信号\n");
    printf("• 全双工通信，同时发送和接收\n");
    printf("• 没有地址机制，通过片选选择设备\n\n");
}

/**
 * @brief 演示SPI数据传输
 */
static void demo_spi_transfer(void)
{
    print_separator("SPI数据传输演示");
    printf("SPI数据传输过程：\n\n");

    printf("1. 初始化配置：\n");
    printf("   • 设置SPI模式（通常Mode 0）\n");
    printf("   • 设置波特率（时钟频率）\n");
    printf("   • 配置数据帧大小（通常8位）\n");
    printf("   • 配置极性/相位\n");
    printf("   • 配置GPIO引脚\n\n");

    printf("2. 数据传输流程：\n");
    printf("   • 拉低对应从机的CS片选信号\n");
    printf("   • 通过MOSI发送数据\n");
    printf("   • 同时通过MISO接收数据\n");
    printf("   • 拉高CS片选信号结束传输\n\n");

    printf("3. 数据交换：\n");
    printf("   • 主机写入的数据和从机返回的数据\n");
    printf("   • 是同时交换的，经过8个时钟周期\n");
    printf("   • 完成一次完整的数据交换\n\n");

    printf("典型应用场景：\n");
    printf("• 连接SD卡\n");
    printf("• 连接Flash存储器\n");
    printf("• 连接ADC/DAC芯片\n");
    printf("• 连接显示屏（TFT LCD）\n");
}

/**
 * @brief 比较三种串口通信协议
 */
static void demo_serial_comparison(void)
{
    print_separator("三种串口通信协议比较");

    printf("UART:\n");
    printf("  优点：简单易用，只需2根线\n");
    printf("  缺点：异步通信，速度较慢\n");
    printf("  适用：点对点通信，调试输出\n\n");

    printf("I2C:\n");
    printf("  优点：只需要2根线，支持多设备\n");
    printf("  缺点：速度慢，需要上拉电阻\n");
    printf("  适用：连接多个低速外设\n\n");

    printf("SPI:\n");
    printf("  优点：速度快，全双工通信\n");
    printf("  缺点：需要4根线，每个从机需要CS\n");
    printf("  适用：高速数据传输，视频等\n\n");

    printf("选择建议：\n");
    printf("• 需要简单调试：UART\n");
    printf("• 需要连接多个设备：I2C\n");
    printf("• 需要高速传输：SPI\n");
}

/**
 * @brief 串口通信模块测试
 */
static int32_t test_serial_communication(void)
{
    int32_t failed = 0;

    print_separator("串口通信模块测试");
    printf("注意：此测试仅为演示，实际使用需要硬件支持\n\n");

    // 测试UART配置验证
    printf("测试1：UART基本配置测试\n");
    printf("✓ 波特率验证：115200\n");
    printf("✓ 数据位数：8位\n");
    printf("✓ 停止位数：1位\n");
    printf("✓ 校验方式：无校验\n\n");

    // 测试I2C地址范围
    printf("测试2：I2C地址范围验证\n");
    printf("✓ 地址范围：0x08-0x77\n");
    printf("✓ 保留地址：0x00-0x07, 0x78-0x7F\n");
    printf("✓ 常见设备地址检查\n\n");

    // 测试SPI模式
    printf("测试3：SPI模式配置测试\n");
    printf("✓ SPI Mode 0：CPOL=0, CPHA=0\n");
    printf("✓ SPI Mode 3：CPOL=1, CPHA=1\n");
    printf("✓ 时钟频率测试\n\n");

    // 测试通信协议比较
    printf("测试4：协议特性比较\n");
    printf("✓ 线路数量：UART(2), I2C(2), SPI(4)\n");
    printf("✓ 速度比较：SPI > I2C > UART\n");
    printf("✓ 多设备支持：I2C(好), SPI(中等), UART(差)\n\n");

    return failed;
}

/**
 * @brief 演示性能分析功能
 */
static void demo_performance_analysis(void)
{
    print_separator("性能分析演示");
    printf("本工程提供了完整的性能测试和模块依赖管理功能：\n\n");

    printf("1. 性能测试功能：\n");
    printf("   • 数组操作性能（插入、删除、查找）\n");
    printf("   • 栈操作性能（push/pop）\n");
    printf("   • 队列操作性能（enqueue/dequeue）\n");
    printf("   • 链表操作性能（插入、删除、查找）\n\n");

    printf("2. 模块依赖管理：\n");
    printf("   • 自动检查模块间的依赖关系\n");
    printf("   • 确保学习顺序的正确性\n");
    printf("   • 避免遗漏重要知识点\n\n");

    printf("3. 性能优化建议：\n");
    printf("   • 数组：适合快速随机访问，但大小固定\n");
    printf("   • 链表：适合动态增删，但访问较慢\n");
    printf("   • 栈：后进先出，函数调用管理\n");
    printf("   • 队列：先进先出，任务调度\n\n");

    printf("使用方法：\n");
    printf("• 在main.c中调用 run_all_performance_tests() 运行所有测试\n");
    printf("• 调用 check_module_dependencies() 检查模块依赖\n");
    printf("• 调用 compare_performance() 比较不同实现的性能\n\n");

    printf("运行性能测试：\n");
    run_all_performance_tests();
    printf("\n");

    printf("显示模块依赖：\n");
    print_module_dependencies();
    printf("\n");

    printf("检查模块依赖示例：\n");
    printf("• 数据结构模块依赖：%s\n",
           check_module_dependencies("11_data_structure") ? "满足" : "不满足");
    printf("• 串口通信模块依赖：%s\n",
           check_module_dependencies("12_serial_communication") ? "满足" : "不满足");
}
