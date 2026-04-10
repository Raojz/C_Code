/**
 * @file control_flow.c
 * @brief 控制流模块实现
 * @details 演示C语言控制流：条件语句、循环结构等。
 *          本模块涵盖以下核心知识点：
 *          - 条件语句：if-else、switch-case
 *          - 循环结构：for、while、do-while
 *          - 跳转语句：break、continue、goto
 *          - 嵌套控制结构
 *          - 状态机设计模式
 * 
 * @section embedded_importance 嵌入式开发重要性
 * - 状态机是嵌入式系统的核心设计模式
 * - 无限循环(for(;;))是嵌入式主循环的标准形式
 * - goto常用于错误处理和资源清理
 * - 循环中需考虑看门狗喂狗
 * 
 * @author Embedded C Learning Project
 * @date 2026-03-22
 * @version 1.0.0
 * 
 * @note 本文件遵循 MISRA-C:2012 编码规范
 * @note MISRA建议避免使用goto，但在资源清理场景可接受
 */

#include "control_flow.h"
#include "common/common_utils.h"
#include <stdio.h>
#include <stdlib.h>

/*============================================================================*/
/*                           条件语句演示函数                                   */
/*============================================================================*/

/**
 * @brief if语句演示
 * @details 展示if语句的各种用法：
 *          - 基本if语句
 *          - if-else链
 *          - 嵌套if语句
 *          - if-else常见陷阱（悬空else问题）
 * 
 * @note 建议始终使用花括号，即使只有一条语句
 * @note 悬空else问题：else与最近的未匹配if配对
 */
void demo_if_statement(void)
{
    print_separator("if语句演示");

    int32_t score = 85;
    printf("  分数: %d\n", score);

    if (score >= 90) {
        printf("  等级: A (优秀)\n");
    } else if (score >= 80) {
        printf("  等级: B (良好)\n");
    } else if (score >= 70) {
        printf("  等级: C (中等)\n");
    } else if (score >= 60) {
        printf("  等级: D (及格)\n");
    } else {
        printf("  等级: F (不及格)\n");
    }

    printf("\n[嵌套if语句]\n");
    int32_t age = 25;
    bool has_license = true;
    printf("  年龄: %d, 驾照: %s\n", age, has_license ? "有" : "无");
    
    if (age >= 18) {
        if (has_license) {
            printf("  结果: 可以驾驶\n");
        } else {
            printf("  结果: 需要考取驾照\n");
        }
    } else {
        printf("  结果: 年龄不足\n");
    }

    printf("\n[if-else常见陷阱]\n");
    int32_t x = 0;
    int32_t y = 1;
    if (x > 0)
        if (y > 0)
            printf("  x和y都大于0\n");
        else
            printf("  x大于0但y不大于0 (注意: else与最近的if匹配!)\n");
    else
        printf("  x不大于0 (正确匹配外层if)\n");
}

/**
 * @brief switch语句演示
 * @details 展示switch语句的用法：
 *          - 基本switch-case结构
 *          - break语句的作用
 *          - fall-through用法
 *          - default分支
 * 
 * @note switch只能用于整数类型（char, int, enum）
 * @note 每个case后应加break，除非有意fall-through
 * @note switch非常适合实现状态机
 */
void demo_switch_statement(void)
{
    print_separator("switch语句演示");

    typedef enum {
        CMD_START = 1,
        CMD_STOP = 2,
        CMD_PAUSE = 3,
        CMD_RESET = 4,
        CMD_STATUS = 5
    } CommandType_t;

    CommandType_t cmd = CMD_PAUSE;
    printf("  命令: %d\n", cmd);

    switch (cmd) {
        case CMD_START:
            printf("  执行: 启动系统\n");
            break;
        case CMD_STOP:
            printf("  执行: 停止系统\n");
            break;
        case CMD_PAUSE:
            printf("  执行: 暂停系统\n");
            break;
        case CMD_RESET:
            printf("  执行: 重置系统\n");
            break;
        case CMD_STATUS:
            printf("  执行: 查询状态\n");
            break;
        default:
            printf("  执行: 未知命令\n");
            break;
    }

    printf("\n[fall-through用法]\n");
    char grade = 'B';
    printf("  等级: %c\n", grade);
    switch (grade) {
        case 'A':
        case 'a':
            printf("  评价: 优秀\n");
            break;
        case 'B':
        case 'b':
            printf("  评价: 良好\n");
            break;
        case 'C':
        case 'c':
            printf("  评价: 中等\n");
            break;
        default:
            printf("  评价: 其他\n");
            break;
    }

    printf("\n[switch与状态机]\n");
    printf("  switch语句非常适合实现状态机逻辑\n");
}

/*============================================================================*/
/*                           循环结构演示函数                                   */
/*============================================================================*/

/**
 * @brief for循环演示
 * @details 展示for循环的各种用法：
 *          - 基本for循环
 *          - 倒序循环
 *          - 步长循环
 *          - 嵌套for循环
 *          - 无限循环 for(;;)
 *          - 多变量for循环
 * 
 * @note for(;;)是嵌入式常用的无限循环形式
 * @note 循环变量应在for语句中声明（C99）
 */
void demo_for_loop(void)
{
    print_separator("for循环演示");

    printf("[基本for循环]\n");
    printf("  1到10求和: ");
    int32_t sum = 0;
    for (int32_t i = 1; i <= 10; i++) {
        sum += i;
    }
    printf("sum = %d\n", sum);

    printf("\n[倒序循环]\n");
    printf("  倒计时: ");
    for (int32_t i = 5; i >= 1; i--) {
        printf("%d ", i);
    }
    printf("\n");

    printf("\n[步长循环]\n");
    printf("  偶数: ");
    for (int32_t i = 0; i <= 10; i += 2) {
        printf("%d ", i);
    }
    printf("\n");

    printf("\n[嵌套for循环 - 九九乘法表]\n");
    for (int32_t i = 1; i <= 9; i++) {
        printf("  ");
        for (int32_t j = 1; j <= i; j++) {
            printf("%d*%d=%-2d ", j, i, i * j);
        }
        printf("\n");
    }

    printf("\n[无限循环]\n");
    printf("  for(;;) { ... } 是嵌入式常用无限循环形式\n");
    printf("  等价于 while(1) { ... }\n");

    printf("\n[多变量for循环]\n");
    for (int32_t i = 0, j = 10; i < j; i++, j--) {
        printf("  i=%d, j=%d\n", i, j);
    }
}

/**
 * @brief while循环演示
 * @details 展示while循环的用法：
 *          - 基本while循环
 *          - 条件循环
 *          - 读取直到条件满足
 *          - 嵌入式典型用法（等待标志位）
 * 
 * @note while先判断后执行，可能一次都不执行
 * @note 嵌入式中等待硬件标志需加超时机制
 */
void demo_while_loop(void)
{
    print_separator("while循环演示");

    printf("[基本while循环]\n");
    int32_t count = 5;
    printf("  倒计时: ");
    while (count > 0) {
        printf("%d ", count);
        count--;
    }
    printf("\n");

    printf("\n[条件循环 - 数字位数统计]\n");
    int32_t num = 12345;
    int32_t digits = 0;
    int32_t temp = num;
    while (temp != 0) {
        temp /= 10;
        digits++;
    }
    printf("  %d 有 %d 位数字\n", num, digits);

    printf("\n[读取直到条件满足]\n");
    const char *str = "Hello World!";
    int32_t index = 0;
    printf("  字符串: \"%s\"\n", str);
    printf("  查找空格位置: ");
    while (str[index] != '\0' && str[index] != ' ') {
        index++;
    }
    if (str[index] == ' ') {
        printf("索引 %d\n", index);
    } else {
        printf("未找到\n");
    }

    printf("\n[嵌入式典型用法 - 等待标志位]\n");
    printf("  while ((reg & FLAG) == 0) { /* 等待硬件就绪 */ }\n");
    printf("  注意: 需添加超时机制防止死循环\n");
}

/**
 * @brief do-while循环演示
 * @details 展示do-while循环的用法：
 *          - 基本do-while循环
 *          - 输入验证
 *          - 菜单选择
 *          - 资源清理模式
 * 
 * @note do-while先执行后判断，至少执行一次
 * @note 常用于需要至少执行一次的场景
 */
void demo_do_while_loop(void)
{
    print_separator("do-while循环演示");

    printf("[基本do-while循环]\n");
    printf("  特点: 至少执行一次循环体\n");
    int32_t i = 0;
    do {
        printf("  i = %d\n", i);
        i++;
    } while (i < 3);

    printf("\n[输入验证模拟]\n");
    int32_t input = -1;
    int32_t attempts = 0;
    printf("  模拟用户输入验证 (有效范围1-10):\n");
    int32_t test_inputs[] = {-5, 0, 15, 7};
    size_t test_idx = 0;
    
    do {
        input = test_inputs[test_idx++];
        attempts++;
        printf("  尝试 %zu: 输入=%d, ", test_idx, input);
        if (input >= 1 && input <= 10) {
            printf("有效\n");
        } else {
            printf("无效, 请重新输入\n");
        }
    } while ((input < 1 || input > 10) && test_idx < ARRAY_SIZE(test_inputs));

    printf("\n[菜单选择模拟]\n");
    printf("  do { 显示菜单; 读取选择; } while (选择 != 退出);\n");

    printf("\n[资源清理模式]\n");
    printf("  do { 操作; if (失败) break; 下一步; } while (0);\n");
    printf("  用于实现类似异常处理的跳转逻辑\n");
}

/*============================================================================*/
/*                           跳转语句演示函数                                   */
/*============================================================================*/

/**
 * @brief break和continue演示
 * @details 展示break和continue的用法：
 *          - break：跳出循环
 *          - continue：跳过当前迭代
 *          - 嵌套循环中的break
 *          - 嵌入式应用场景
 * 
 * @note break只跳出最内层循环
 * @note continue跳过本次迭代，继续下一次
 */
void demo_break_continue(void)
{
    print_separator("break和continue演示");

    printf("[break - 跳出循环]\n");
    printf("  查找第一个负数: ");
    int32_t arr[] = {5, 3, -2, 8, -1, 4};
    int32_t found_idx = -1;
    for (int32_t i = 0; i < (int32_t)ARRAY_SIZE(arr); i++) {
        if (arr[i] < 0) {
            found_idx = i;
            break;
        }
    }
    if (found_idx >= 0) {
        printf("索引 %d, 值 %d\n", found_idx, arr[found_idx]);
    } else {
        printf("未找到\n");
    }

    printf("\n[continue - 跳过当前迭代]\n");
    printf("  打印正数: ");
    for (int32_t i = 0; i < (int32_t)ARRAY_SIZE(arr); i++) {
        if (arr[i] < 0) {
            continue;
        }
        printf("%d ", arr[i]);
    }
    printf("\n");

    printf("\n[嵌套循环中的break]\n");
    printf("  注意: break只跳出最内层循环\n");
    bool found = false;
    for (int32_t i = 0; i < 3 && !found; i++) {
        for (int32_t j = 0; j < 3; j++) {
            printf("  (%d,%d) ", i, j);
            if (i == 1 && j == 1) {
                printf("-> 找到目标\n");
                found = true;
                break;
            }
        }
        if (!found) {
            printf("\n");
        }
    }

    printf("\n[嵌入式应用 - 数据处理]\n");
    printf("  for处理数据包 {\n");
    printf("    if (校验失败) continue;  // 跳过无效包\n");
    printf("    if (结束标志) break;      // 停止处理\n");
    printf("    处理有效数据;\n");
    printf("  }\n");
}

/**
 * @brief goto语句演示
 * @details 展示goto语句的正确使用场景：
 *          - 错误处理和资源清理
 *          - 跳出多层嵌套循环
 *          - goto使用原则
 * 
 * @note MISRA-C限制goto的使用，但资源清理场景可接受
 * @note goto只能在同一函数内跳转
 * @note 避免向前跳转（容易产生混乱代码）
 */
void demo_goto_statement(void)
{
    print_separator("goto语句演示");

    printf("[goto的正确使用场景]\n");
    printf("  1. 错误处理和资源清理\n");
    printf("  2. 跳出多层嵌套循环\n");
    printf("  3. 优化性能关键代码\n\n");

    printf("[示例: 资源清理模式]\n");
    int32_t *ptr1 = NULL;
    int32_t *ptr2 = NULL;
    int32_t *ptr3 = NULL;
    Status_t status = STATUS_OK;

    ptr1 = (int32_t *)malloc(sizeof(int32_t) * 10);
    if (ptr1 == NULL) {
        status = STATUS_NO_MEM;
        goto cleanup;
    }
    printf("  分配ptr1成功\n");

    ptr2 = (int32_t *)malloc(sizeof(int32_t) * 10);
    if (ptr2 == NULL) {
        status = STATUS_NO_MEM;
        goto cleanup;
    }
    printf("  分配ptr2成功\n");

    ptr3 = (int32_t *)malloc(sizeof(int32_t) * 10);
    if (ptr3 == NULL) {
        status = STATUS_NO_MEM;
        goto cleanup;
    }
    printf("  分配ptr3成功\n");

    printf("  所有资源分配成功\n");

cleanup:
    if (ptr3 != NULL) {
        free(ptr3);
        printf("  释放ptr3\n");
    }
    if (ptr2 != NULL) {
        free(ptr2);
        printf("  释放ptr2\n");
    }
    if (ptr1 != NULL) {
        free(ptr1);
        printf("  释放ptr1\n");
    }

    printf("\n[示例: 跳出多层循环]\n");
    int32_t matrix[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    int32_t target = 5;
    bool found_target = false;
    int32_t found_row = -1, found_col = -1;

    for (int32_t i = 0; i < 3; i++) {
        for (int32_t j = 0; j < 3; j++) {
            if (matrix[i][j] == target) {
                found_target = true;
                found_row = i;
                found_col = j;
                goto found;
            }
        }
    }
found:
    if (found_target) {
        printf("  找到 %d 在位置 [%d][%d]\n", target, found_row, found_col);
    } else {
        printf("  未找到 %d\n", target);
    }

    printf("\n[goto使用原则]\n");
    printf("  - 仅向后跳转(向前跳转容易产生意大利面条式代码)\n");
    printf("  - 跳转目标在同一函数内\n");
    printf("  - 避免跳过变量初始化\n");
    printf("  - 优先使用其他控制结构\n");
}

/*============================================================================*/
/*                           嵌套控制演示函数                                   */
/*============================================================================*/

/**
 * @brief 嵌套控制结构演示
 * @details 展示嵌套控制结构的应用：
 *          - 冒泡排序
 *          - 查找质数
 *          - 复杂条件组合（FizzBuzz问题）
 * 
 * @note 嵌套层次不宜过深，建议不超过3层
 * @note 复杂嵌套可考虑提取为函数
 */
void demo_nested_control(void)
{
    print_separator("嵌套控制结构演示");

    printf("[冒泡排序]\n");
    int32_t arr[] = {64, 34, 25, 12, 22, 11, 90, 45};
    size_t n = ARRAY_SIZE(arr);
    
    printf("  排序前: ");
    for (size_t i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    for (size_t i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (size_t j = 0; j < n - i - 1; j++) {
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

    printf("  排序后: ");
    for (size_t i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    printf("\n[查找质数]\n");
    printf("  2到30之间的质数: ");
    for (int32_t num = 2; num <= 30; num++) {
        bool is_prime = true;
        for (int32_t i = 2; i * i <= num; i++) {
            if (num % i == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            printf("%d ", num);
        }
    }
    printf("\n");

    printf("\n[复杂条件组合]\n");
    for (int32_t i = 1; i <= 20; i++) {
        bool div_by_3 = (i % 3 == 0);
        bool div_by_5 = (i % 5 == 0);
        
        if (div_by_3 && div_by_5) {
            printf("  %d: FizzBuzz\n", i);
        } else if (div_by_3) {
            printf("  %d: Fizz\n", i);
        } else if (div_by_5) {
            printf("  %d: Buzz\n", i);
        }
    }
}

/*============================================================================*/
/*                           状态机演示函数                                     */
/*============================================================================*/

/**
 * @brief 状态机演示
 * @details 展示有限状态机(FSM)的设计和实现：
 *          - 状态定义
 *          - 事件定义
 *          - 状态转换表
 *          - 状态机执行
 * 
 * @note 状态机是嵌入式系统的核心设计模式
 * @note 每个状态应处理所有可能的事件
 * @note 状态机设计原则：明确、完整、可测试
 */
void demo_state_machine(void)
{
    print_separator("状态机演示");

    typedef enum {
        STATE_IDLE,
        STATE_RUNNING,
        STATE_PAUSED,
        STATE_ERROR,
        STATE_MAX
    } State_t;

    typedef enum {
        EVENT_START,
        EVENT_STOP,
        EVENT_PAUSE,
        EVENT_RESUME,
        EVENT_ERROR,
        EVENT_RESET,
        EVENT_MAX
    } Event_t;

    printf("[有限状态机实现]\n");
    State_t current_state = STATE_IDLE;
    Event_t events[] = {EVENT_START, EVENT_PAUSE, EVENT_RESUME, EVENT_STOP};
    
    const char *state_names[] = {"IDLE", "RUNNING", "PAUSED", "ERROR"};
    const char *event_names[] = {"START", "STOP", "PAUSE", "RESUME", "ERROR", "RESET"};

    printf("  初始状态: %s\n", state_names[current_state]);

    for (size_t i = 0; i < ARRAY_SIZE(events); i++) {
        Event_t event = events[i];
        printf("  事件: %s -> ", event_names[event]);

        switch (current_state) {
            case STATE_IDLE:
                if (event == EVENT_START) {
                    current_state = STATE_RUNNING;
                }
                break;
            case STATE_RUNNING:
                if (event == EVENT_STOP) {
                    current_state = STATE_IDLE;
                } else if (event == EVENT_PAUSE) {
                    current_state = STATE_PAUSED;
                } else if (event == EVENT_ERROR) {
                    current_state = STATE_ERROR;
                }
                break;
            case STATE_PAUSED:
                if (event == EVENT_RESUME) {
                    current_state = STATE_RUNNING;
                } else if (event == EVENT_STOP) {
                    current_state = STATE_IDLE;
                }
                break;
            case STATE_ERROR:
                if (event == EVENT_RESET) {
                    current_state = STATE_IDLE;
                }
                break;
            default:
                break;
        }
        printf("状态: %s\n", state_names[current_state]);
    }

    printf("\n[状态机设计原则]\n");
    printf("  1. 明确定义所有状态和事件\n");
    printf("  2. 每个状态处理所有可能事件\n");
    printf("  3. 状态转换逻辑清晰\n");
    printf("  4. 考虑异常状态处理\n");
}
