/**
 * @file test_data_structure.c
 * @brief 数据结构模块测试
 */

#include "test_framework.h"
#include "data_structure.h"
#include "stack.h"
#include "queue.h"
#include "linked_list.h"
#include <stdio.h>

static int32_t test_compare_int32(const void *a, const void *b)
{
    const int32_t *pa = (const int32_t *)a;
    const int32_t *pb = (const int32_t *)b;
    
    if (*pa < *pb) {
        return -1;
    } else if (*pa > *pb) {
        return 1;
    }
    return 0;
}

static void test_stack_operations(void)
{
    printf("    [测试] 栈操作...\n");

    Stack_t stack;
    uint8_t buffer[10 * sizeof(int32_t)];

    int32_t ret = stack_init(&stack, buffer, 10, sizeof(int32_t));
    if (ret != STATUS_OK) {
        printf("    [失败] 栈初始化错误\n");
        test_fail();
        return;
    }

    if (!stack_is_empty(&stack)) {
        printf("    [失败] 新栈应为空\n");
        test_fail();
        return;
    }

    int32_t value = 10;
    stack_push(&stack, &value);
    value = 20;
    stack_push(&stack, &value);
    value = 30;
    stack_push(&stack, &value);

    bool pass = (stack_size(&stack) == 3);

    int32_t top;
    ret = stack_peek(&stack, &top);
    pass = pass && (ret == STATUS_OK) && (top == 30);

    ret = stack_pop(&stack, &top);
    pass = pass && (ret == STATUS_OK) && (top == 30);

    ret = stack_pop(&stack, &top);
    pass = pass && (ret == STATUS_OK) && (top == 20);

    if (pass) {
        printf("    [通过] 栈操作正确\n");
        test_pass();
    } else {
        printf("    [失败] 栈操作错误\n");
        test_fail();
    }
}

static void test_queue_operations(void)
{
    printf("    [测试] 队列操作...\n");

    Queue_t queue;
    uint8_t buffer[10 * sizeof(int32_t)];

    int32_t ret = queue_init(&queue, buffer, 10, sizeof(int32_t));
    if (ret != STATUS_OK) {
        printf("    [失败] 队列初始化错误\n");
        test_fail();
        return;
    }

    int32_t value = 1;
    queue_enqueue(&queue, &value);
    value = 2;
    queue_enqueue(&queue, &value);
    value = 3;
    queue_enqueue(&queue, &value);

    bool pass = (queue_size(&queue) == 3);

    int32_t front;
    ret = queue_dequeue(&queue, &front);
    pass = pass && (ret == STATUS_OK) && (front == 1);

    ret = queue_dequeue(&queue, &front);
    pass = pass && (ret == STATUS_OK) && (front == 2);

    if (pass) {
        printf("    [通过] 队列操作正确\n");
        test_pass();
    } else {
        printf("    [失败] 队列操作错误\n");
        test_fail();
    }
}

static void test_linked_list_operations(void)
{
    printf("    [测试] 链表操作...\n");

    LinkedList_t list;
    ListNode_t node_pool[10];
    uint8_t data_pool[10 * sizeof(int32_t)];

    int32_t ret = linked_list_init(&list, node_pool, data_pool, 10, sizeof(int32_t));
    if (ret != STATUS_OK) {
        printf("    [失败] 链表初始化错误\n");
        test_fail();
        return;
    }

    if (!linked_list_is_empty(&list)) {
        printf("    [失败] 新链表应为空\n");
        test_fail();
        return;
    }

    int32_t data1 = 10, data2 = 20, data3 = 30;
    linked_list_push_front(&list, &data1);
    linked_list_push_front(&list, &data2);
    linked_list_push_back(&list, &data3);

    bool pass = (linked_list_size(&list) == 3);

    size_t index = linked_list_find(&list, &data2, test_compare_int32);
    pass = pass && (index != SIZE_MAX);

    if (pass) {
        printf("    [通过] 链表操作正确\n");
        test_pass();
    } else {
        printf("    [失败] 链表操作错误\n");
        test_fail();
    }
}

static void test_static_array_operations(void)
{
    printf("    [测试] 静态数组操作...\n");

    StaticArray_t arr;
    uint8_t buffer[10 * sizeof(int32_t)];
    int32_t ret = static_array_init(&arr, buffer, 10, sizeof(int32_t));
    if (ret != STATUS_OK) {
        printf("    [失败] 静态数组初始化错误\n");
        test_fail();
        return;
    }

    for (int32_t i = 0; i < 5; i++) {
        int32_t val = i * 10;
        static_array_push_back(&arr, &val);
    }

    bool pass = (static_array_size(&arr) == 5);

    int32_t elem;
    ret = static_array_get(&arr, 2, &elem);
    pass = pass && (ret == STATUS_OK) && (elem == 20);

    if (pass) {
        printf("    [通过] 静态数组操作正确\n");
        test_pass();
    } else {
        printf("    [失败] 静态数组操作错误\n");
        test_fail();
    }
}

static void test_boundary_conditions(void)
{
    printf("    [测试] 边界条件...\n");

    Stack_t empty_stack;
    uint8_t buffer[10 * sizeof(int32_t)];
    stack_init(&empty_stack, buffer, 10, sizeof(int32_t));

    int32_t value;
    int32_t ret = stack_pop(&empty_stack, &value);
    bool pass = (ret == STATUS_UNDERFLOW);

    Queue_t empty_queue;
    queue_init(&empty_queue, buffer, 10, sizeof(int32_t));
    ret = queue_dequeue(&empty_queue, &value);
    pass = pass && (ret == STATUS_UNDERFLOW);

    if (pass) {
        printf("    [通过] 边界条件正确\n");
        test_pass();
    } else {
        printf("    [失败] 边界条件错误\n");
        test_fail();
    }
}

static void test_data_structure_integrity(void)
{
    printf("    [测试] 数据结构完整性...\n");

    Stack_t stack;
    uint8_t buffer[5 * sizeof(int32_t)];
    stack_init(&stack, buffer, 5, sizeof(int32_t));

    int32_t values[] = {1, 2, 3, 4, 5};
    for (int32_t i = 0; i < 5; i++) {
        stack_push(&stack, &values[i]);
    }

    bool pass = true;
    for (int32_t i = 4; i >= 0; i--) {
        int32_t val;
        stack_pop(&stack, &val);
        pass = pass && (val == values[i]);
    }

    if (pass) {
        printf("    [通过] 数据结构完整性正确\n");
        test_pass();
    } else {
        printf("    [失败] 栈 LIFO 特性错误\n");
        test_fail();
    }
}

int32_t test_data_structure(void)
{
    test_stack_operations();
    test_queue_operations();
    test_linked_list_operations();
    test_static_array_operations();
    test_boundary_conditions();
    test_data_structure_integrity();

    TestStats_t stats = test_get_stats();
    return stats.failed;
}
