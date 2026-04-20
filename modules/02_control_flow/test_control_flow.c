/**
 * @file test_control_flow.c
 * @brief 控制流模块测试
 */

#include "test_framework.h"
#include "control_flow.h"
#include <stdio.h>

static void test_if_logic(void)
{
    printf("    [测试] if 语句逻辑...\n");

    int32_t a = 10, b = 20, max_val;

    if (a > b) max_val = a;
    else max_val = b;

    int32_t x = 5, result = 0;
    if (x > 0) {
        if (x < 10) result = 1;
        else result = 2;
    } else {
        result = 3;
    }

    bool pass = (max_val == 20) && (result == 1);

    if (pass) {
        printf("    [通过] if 语句逻辑正确\n");
        test_pass();
    } else {
        printf("    [失败] if 语句逻辑错误\n");
        test_fail();
    }
}

static void test_switch_logic(void)
{
    printf("    [测试] switch 语句逻辑...\n");

    int32_t day = 3;
    const char *day_name;

    switch (day) {
        case 1:  day_name = "Monday";    break;
        case 2:  day_name = "Tuesday";   break;
        case 3:  day_name = "Wednesday"; break;
        case 4:  day_name = "Thursday";  break;
        case 5:  day_name = "Friday";    break;
        case 6:  day_name = "Saturday";  break;
        case 7:  day_name = "Sunday";    break;
        default: day_name = "Invalid";   break;
    }

    if (day_name[0] == 'W') {
        printf("    [通过] switch 语句逻辑正确\n");
        test_pass();
    } else {
        printf("    [失败] switch 语句逻辑错误\n");
        test_fail();
    }
}

static void test_for_loop(void)
{
    printf("    [测试] for 循环...\n");

    int32_t sum = 0;
    for (int32_t i = 1; i <= 10; i++) {
        sum += i;
    }

    if (sum == 55) {
        printf("    [通过] for 循环正确\n");
        test_pass();
    } else {
        printf("    [失败] for 循环错误\n");
        test_fail();
    }
}

static void test_while_loop(void)
{
    printf("    [测试] while 循环...\n");

    int32_t n = 1024, count = 0;
    while (n > 0) {
        n = n / 2;
        count++;
    }

    if (count == 10) {
        printf("    [通过] while 循环正确\n");
        test_pass();
    } else {
        printf("    [失败] while 循环错误\n");
        test_fail();
    }
}

static void test_do_while_loop(void)
{
    printf("    [测试] do-while 循环...\n");

    int32_t value = 0, iterations = 0;
    do {
        value += 5;
        iterations++;
    } while (value < 20);

    if (iterations == 4) {
        printf("    [通过] do-while 循环正确\n");
        test_pass();
    } else {
        printf("    [失败] do-while 循环错误\n");
        test_fail();
    }
}

static void test_break_continue(void)
{
    printf("    [测试] break 和 continue...\n");

    int32_t sum = 0;
    for (int32_t i = 1; i <= 10; i++) {
        if (i == 5) break;
        sum += i;
    }
    bool pass = (sum == 10);

    sum = 0;
    for (int32_t i = 1; i <= 5; i++) {
        if (i == 3) continue;
        sum += i;
    }
    pass = pass && (sum == 12);

    if (pass) {
        printf("    [通过] break 和 continue 正确\n");
        test_pass();
    } else {
        printf("    [失败] break 和 continue 错误\n");
        test_fail();
    }
}

int32_t test_control_flow(void)
{
    test_if_logic();
    test_switch_logic();
    test_for_loop();
    test_while_loop();
    test_do_while_loop();
    test_break_continue();

    TestStats_t stats = test_get_stats();
    return stats.failed;
}
