/**
 * @file test_perf_helpers.c
 * @brief 性能测试辅助函数实现
 * @details 实现用于性能基准测试的数据结构操作辅助函数。
 *          每个函数执行特定数量的操作，以便进行准确的时间测量。
 *
 * @section implementation 实现说明
 * - 每个函数使用静态变量维护测试数据
 * - 函数执行固定次数的操作以确保可重复性
 * - 操作规模可通过修改宏定义进行调整
 *
 * @author Embedded C Learning Project
 * @date 2026-04-21
 * @version 1.0.0
 */

#include "test_perf_helpers.h"
#include "data_structure.h"
#include "static_array.h"
#include "stack.h"
#include "queue.h"
#include "linked_list.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>

/*============================================================================*/
/*                           测试配置宏                                        */
/*============================================================================*/

/** @brief 性能测试的数组/数据结构大小 */
#define PERF_TEST_SIZE          (1000U)

/** @brief 性能测试的查找目标值 */
#define PERF_SEARCH_TARGET       (500)

/*============================================================================*/
/*                           私有全局变量                                       */
/*============================================================================*/

/** @brief 测试用静态数组缓冲区 */
static uint8_t g_array_buffer[PERF_TEST_SIZE * sizeof(int32_t) + 100U];

/** @brief 测试用栈缓冲区 */
static uint8_t g_stack_buffer[PERF_TEST_SIZE * sizeof(int32_t) + 100U];

/** @brief 测试用队列缓冲区 */
static uint8_t g_queue_buffer[PERF_TEST_SIZE * sizeof(int32_t) + 100U];

/** @brief 测试用数组（已初始化标志） */
static bool g_array_initialized = false;

/** @brief 测试用栈（已初始化标志） */
static bool g_stack_initialized = false;

/** @brief 测试用队列（已初始化标志） */
static bool g_queue_initialized = false;

/** @brief 测试用链表 */
static LinkedList_t g_test_list = {0};

/** @brief 链表节点池（静态分配） */
static ListNode_t g_list_node_pool[PERF_TEST_SIZE + 10U];

/** @brief 链表数据池（静态分配） */
static uint8_t g_list_data_pool[PERF_TEST_SIZE * sizeof(int32_t) + 100U];

/** @brief 链表节点计数 */
static uint32_t g_list_node_count = 0U;

/*============================================================================*/
/*                           数组操作辅助函数实现                               */
/*============================================================================*/

/**
 * @brief 初始化测试数组（内部辅助函数）
 */
static void init_perf_array(void)
{
    if (!g_array_initialized) {
        StaticArray_t arr;
        static_array_init(&arr, g_array_buffer, PERF_TEST_SIZE, sizeof(int32_t));

        /* 填充有序数据：0, 1, 2, ..., PERF_TEST_SIZE-1 */
        for (uint32_t i = 0U; i < PERF_TEST_SIZE; i++) {
            int32_t value = (int32_t)i;
            static_array_push_back(&arr, &value);
        }

        g_array_initialized = true;
    }
}

void test_array_create(void)
{
    StaticArray_t arr;
    uint8_t buffer[PERF_TEST_SIZE * sizeof(int32_t)];

    (void)buffer;  /* 避免未使用警告 */

    static_array_init(&arr, buffer, PERF_TEST_SIZE, sizeof(int32_t));

    for (uint32_t i = 0U; i < PERF_TEST_SIZE; i++) {
        int32_t value = (int32_t)i;
        static_array_push_back(&arr, &value);
    }
}

void test_array_binary_search(void)
{
    init_perf_array();

    StaticArray_t arr;
    static_array_init(&arr, g_array_buffer, PERF_TEST_SIZE, sizeof(int32_t));

    /* 执行多次二分查找以获得可测量的时间 */
    for (uint32_t i = 0U; i < 100U; i++) {
        int32_t target = PERF_SEARCH_TARGET;
        int32_t result;
        int32_t low = 0;
        int32_t high = (int32_t)(PERF_TEST_SIZE - 1U);

        while (low <= high) {
            int32_t mid = low + (high - low) / 2;
            (void)static_array_get(&arr, (uint32_t)mid, &result);

            if (result == target) {
                break;
            } else if (result < target) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
    }
}

void test_array_traverse(void)
{
    init_perf_array();

    StaticArray_t arr;
    static_array_init(&arr, g_array_buffer, PERF_TEST_SIZE, sizeof(int32_t));

    volatile int32_t sum = 0;  /* volatile 防止编译器优化掉循环 */

    for (uint32_t i = 0U; i < static_array_size(&arr); i++) {
        int32_t value;
        if (STATUS_OK == static_array_get(&arr, i, &value)) {
            sum += value;
        }
    }

    (void)sum;  /* 使用结果避免优化 */
}

/*============================================================================*/
/*                           栈操作辅助函数实现                                 */
/*============================================================================*/

/**
 * @brief 初始化测试栈（内部辅助函数）
 */
static void init_perf_stack(void)
{
    if (!g_stack_initialized) {
        Stack_t stack;
        stack_init(&stack, g_stack_buffer, PERF_TEST_SIZE, sizeof(int32_t));

        for (uint32_t i = 0U; i < PERF_TEST_SIZE; i++) {
            int32_t value = (int32_t)i;
            stack_push(&stack, &value);
        }

        g_stack_initialized = true;
    }
}

void test_stack_create(void)
{
    Stack_t stack;
    uint8_t buffer[PERF_TEST_SIZE * sizeof(int32_t)];

    (void)buffer;

    stack_init(&stack, buffer, PERF_TEST_SIZE, sizeof(int32_t));
}

void test_stack_push(void)
{
    init_perf_stack();

    Stack_t stack;
    stack_init(&stack, g_stack_buffer, PERF_TEST_SIZE, sizeof(int32_t));

    for (uint32_t i = 0U; i < PERF_TEST_SIZE; i++) {
        int32_t value = (int32_t)i;
        stack_push(&stack, &value);
    }
}

void test_stack_pop(void)
{
    init_perf_stack();

    Stack_t stack;
    stack_init(&stack, g_stack_buffer, PERF_TEST_SIZE, sizeof(int32_t));

    /* 先压入元素 */
    for (uint32_t i = 0U; i < PERF_TEST_SIZE; i++) {
        int32_t value = (int32_t)i;
        stack_push(&stack, &value);
    }

    /* 再弹出所有元素 */
    int32_t value;
    for (uint32_t i = 0U; i < PERF_TEST_SIZE; i++) {
        (void)stack_pop(&stack, &value);
    }
}

/*============================================================================*/
/*                           队列操作辅助函数实现                               */
/*============================================================================*/

/**
 * @brief 初始化测试队列（内部辅助函数）
 */
static void init_perf_queue(void)
{
    if (!g_queue_initialized) {
        Queue_t queue;
        queue_init(&queue, g_queue_buffer, PERF_TEST_SIZE, sizeof(int32_t));

        for (uint32_t i = 0U; i < PERF_TEST_SIZE; i++) {
            int32_t value = (int32_t)i;
            queue_enqueue(&queue, &value);
        }

        g_queue_initialized = true;
    }
}

void test_queue_create(void)
{
    Queue_t queue;
    uint8_t buffer[PERF_TEST_SIZE * sizeof(int32_t)];

    (void)buffer;

    queue_init(&queue, buffer, PERF_TEST_SIZE, sizeof(int32_t));
}

void test_queue_enqueue(void)
{
    init_perf_queue();

    Queue_t queue;
    queue_init(&queue, g_queue_buffer, PERF_TEST_SIZE, sizeof(int32_t));

    for (uint32_t i = 0U; i < PERF_TEST_SIZE; i++) {
        int32_t value = (int32_t)i;
        queue_enqueue(&queue, &value);
    }
}

void test_queue_dequeue(void)
{
    init_perf_queue();

    Queue_t queue;
    queue_init(&queue, g_queue_buffer, PERF_TEST_SIZE, sizeof(int32_t));

    /* 先入队元素 */
    for (uint32_t i = 0U; i < PERF_TEST_SIZE; i++) {
        int32_t value = (int32_t)i;
        queue_enqueue(&queue, &value);
    }

    /* 再出队所有元素 */
    int32_t value;
    for (uint32_t i = 0U; i < PERF_TEST_SIZE; i++) {
        (void)queue_dequeue(&queue, &value);
    }
}

/*============================================================================*/
/*                           链表操作辅助函数实现                               */
/*============================================================================*/

/**
 * @brief int32_t 比较函数（用于 linked_list_find）
 * @param a 第一个操作数指针
 * @param b 第二个操作数指针
 * @retval 0 两值相等
 * @retval <0 a < b
 * @retval >0 a > b
 */
static int32_t int32_compare(const void *a, const void *b)
{
    if ((NULL == a) || (NULL == b)) {
        return -1;
    }

    int32_t val_a = *(const int32_t *)a;
    int32_t val_b = *(const int32_t *)b;

    if (val_a < val_b) { return -1; }
    if (val_a > val_b) { return 1; }
    return 0;
}

/**
 * @brief 初始化测试链表（内部辅助函数）
 */
static void init_perf_list(void)
{
    if (0U == g_list_node_count) {
        linked_list_init(&g_test_list,
                         g_list_node_pool,
                         g_list_data_pool,
                         PERF_TEST_SIZE,
                         sizeof(int32_t));

        for (uint32_t i = 0U; i < PERF_TEST_SIZE; i++) {
            int32_t value = (int32_t)i;
            linked_list_push_back(&g_test_list, &value);
            g_list_node_count++;
        }
    }
}

void test_linked_list_create(void)
{
    ListNode_t node_pool[PERF_TEST_SIZE + 10U];
    uint8_t data_pool[PERF_TEST_SIZE * sizeof(int32_t) + 100U];
    LinkedList_t list;

    linked_list_init(&list, node_pool, data_pool, PERF_TEST_SIZE, sizeof(int32_t));

    for (uint32_t i = 0U; i < PERF_TEST_SIZE; i++) {
        int32_t value = (int32_t)i;
        linked_list_push_back(&list, &value);
    }

    linked_list_clear(&list);
}

void test_linked_list_insert(void)
{
    init_perf_list();

    ListNode_t node_pool[PERF_TEST_SIZE + 10U];
    uint8_t data_pool[PERF_TEST_SIZE * sizeof(int32_t) + 100U];
    LinkedList_t list;

    linked_list_init(&list, node_pool, data_pool, PERF_TEST_SIZE, sizeof(int32_t));

    /* 先创建基础链表 */
    for (uint32_t i = 0U; i < PERF_TEST_SIZE / 2U; i++) {
        int32_t value = (int32_t)(i * 2);  /* 偶数 */
        linked_list_push_back(&list, &value);
    }

    /* 在中间位置插入奇数值 */
    for (uint32_t i = 0U; i < PERF_TEST_SIZE / 2U; i++) {
        int32_t value = (int32_t)(i * 2 + 1);  /* 奇数 */
        linked_list_insert_at(&list, i, &value);
    }

    linked_list_clear(&list);
}

void test_linked_list_delete(void)
{
    init_perf_list();

    ListNode_t node_pool[PERF_TEST_SIZE + 10U];
    uint8_t data_pool[PERF_TEST_SIZE * sizeof(int32_t) + 100U];
    LinkedList_t list;

    linked_list_init(&list, node_pool, data_pool, PERF_TEST_SIZE, sizeof(int32_t));

    /* 创建完整链表 */
    for (uint32_t i = 0U; i < PERF_TEST_SIZE; i++) {
        int32_t value = (int32_t)i;
        linked_list_push_back(&list, &value);
    }

    /* 从尾部开始删除节点 */
    for (uint32_t i = 0U; i < PERF_TEST_SIZE / 2U; i++) {
        uint32_t index = (PERF_TEST_SIZE - 1U) - i;
        linked_list_remove_at(&list, index, NULL);
    }

    linked_list_clear(&list);
}

void test_linked_list_search(void)
{
    init_perf_list();

    ListNode_t node_pool[PERF_TEST_SIZE + 10U];
    uint8_t data_pool[PERF_TEST_SIZE * sizeof(int32_t) + 100U];
    LinkedList_t list;

    linked_list_init(&list, node_pool, data_pool, PERF_TEST_SIZE, sizeof(int32_t));

    /* 创建有序链表 */
    for (uint32_t i = 0U; i < PERF_TEST_SIZE; i++) {
        int32_t value = (int32_t)i;
        linked_list_push_back(&list, &value);
    }

    /* 执行多次查找 */
    volatile int32_t found_value = 0;

    for (uint32_t i = 0U; i < 100U; i++) {
        int32_t target = PERF_SEARCH_TARGET;
        size_t found_index = linked_list_find(&list, &target, int32_compare);

        if (found_index != SIZE_MAX) {
            int32_t value;
            (void)linked_list_get(&list, found_index, &value);
            found_value = value;
        }
    }

    (void)found_value;

    linked_list_clear(&list);
}
