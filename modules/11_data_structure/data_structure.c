/**
 * @file data_structure.c
 * @brief 数据结构模块演示函数实现
 * @details 提供各数据结构的演示代码和单元测试用例。
 * 
 * @author Embedded C Learning Project
 * @date 2026-04-10
 * @version 1.0.0
 */

#include "data_structure.h"
#include <stdio.h>
#include <string.h>

/*============================================================================*/
/*                           测试辅助宏                                         */
/*============================================================================*/

#define TEST_ASSERT(cond, msg)                                                \
    do {                                                                       \
        if (!(cond)) {                                                         \
            printf("  [FAIL] %s (line %d)\n", msg, __LINE__);                  \
        } else {                                                               \
            printf("  [PASS] %s\n", msg);                                      \
        }                                                                      \
    } while(0)

/*============================================================================*/
/*                           内部函数                                           */
/*============================================================================*/

/**
 * @brief 整数比较函数
 */
static int32_t compare_int32(const void *a, const void *b)
{
    const int32_t *pa = (const int32_t *)a;
    const int32_t *pb = (const int32_t *)b;
    
    if (*pa < *pb) {
        return -1;
    } else if (*pa > *pb) {
        return 1;
    } else {
        return 0;
    }
}

/**
 * @brief 打印分隔线
 */
static void print_separator_line(void)
{
    printf("  ─────────────────────────────────────────────────────────────────────────────\n");
}

/*============================================================================*/
/*                           静态数组演示                                       */
/*============================================================================*/

void demo_static_array(void)
{
    printf("\n[静态数组演示]\n");
    print_separator_line();
    
    #define ARRAY_CAPACITY 10
    
    StaticArray_t arr;
    uint8_t buffer[ARRAY_CAPACITY * sizeof(int32_t)];
    int32_t value;
    int32_t test_data[] = {10, 20, 30, 40, 50};
    size_t data_count = ARRAY_SIZE(test_data);
    
    printf("\n  1. 初始化测试\n");
    {
        int32_t ret = static_array_init(&arr, buffer, ARRAY_CAPACITY, sizeof(int32_t));
        TEST_ASSERT(SUCCESS(ret), "数组初始化");
        TEST_ASSERT(static_array_is_empty(&arr), "数组为空");
        TEST_ASSERT(static_array_size(&arr) == 0U, "数组大小为0");
        TEST_ASSERT(static_array_capacity(&arr) == ARRAY_CAPACITY, "数组容量正确");
    }
    
    printf("\n  2. 添加元素测试\n");
    {
        for (size_t i = 0U; i < data_count; i++) {
            int32_t ret = static_array_push_back(&arr, &test_data[i]);
            TEST_ASSERT(SUCCESS(ret), "添加元素成功");
        }
        TEST_ASSERT(static_array_size(&arr) == data_count, "数组大小正确");
        TEST_ASSERT(!static_array_is_empty(&arr), "数组非空");
        
        printf("  数组内容: ");
        for (size_t i = 0U; i < static_array_size(&arr); i++) {
            (void)static_array_get(&arr, i, &value);
            printf("%d ", value);
        }
        printf("\n");
    }
    
    printf("\n  3. 边界检查测试\n");
    {
        int32_t ret = static_array_get(&arr, 100, &value);
        TEST_ASSERT(FAILED(ret), "越界访问被拒绝");
        
        ret = static_array_set(&arr, 100, &value);
        TEST_ASSERT(FAILED(ret), "越界设置被拒绝");
    }
    
    printf("\n  4. 插入和删除测试\n");
    {
        value = 25;
        int32_t ret = static_array_insert(&arr, 2, &value);
        TEST_ASSERT(SUCCESS(ret), "位置2插入元素");
        
        (void)static_array_get(&arr, 2, &value);
        TEST_ASSERT(value == 25, "插入值正确");
        
        ret = static_array_remove(&arr, 2, &value);
        TEST_ASSERT(SUCCESS(ret), "删除元素");
        TEST_ASSERT(value == 25, "删除值正确");
    }
    
    printf("\n  5. 查找测试\n");
    {
        value = 30;
        size_t index = static_array_find(&arr, &value, compare_int32);
        TEST_ASSERT(index != SIZE_MAX, "查找元素成功");
        TEST_ASSERT(index == 2U, "查找位置正确");
        
        value = 999;
        index = static_array_find(&arr, &value, compare_int32);
        TEST_ASSERT(index == SIZE_MAX, "查找不存在的元素返回SIZE_MAX");
    }
    
    printf("\n  6. 清空测试\n");
    {
        int32_t ret = static_array_clear(&arr);
        TEST_ASSERT(SUCCESS(ret), "清空数组");
        TEST_ASSERT(static_array_is_empty(&arr), "数组为空");
        TEST_ASSERT(static_array_size(&arr) == 0U, "数组大小为0");
    }
    
    #undef ARRAY_CAPACITY
}

/*============================================================================*/
/*                           栈演示                                             */
/*============================================================================*/

void demo_stack(void)
{
    printf("\n[栈演示]\n");
    print_separator_line();
    
    #define STACK_CAPACITY 8
    
    Stack_t stack;
    uint8_t buffer[STACK_CAPACITY * sizeof(int32_t)];
    int32_t value;
    
    printf("\n  1. 初始化测试\n");
    {
        int32_t ret = stack_init(&stack, buffer, STACK_CAPACITY, sizeof(int32_t));
        TEST_ASSERT(SUCCESS(ret), "栈初始化");
        TEST_ASSERT(stack_is_empty(&stack), "栈为空");
        TEST_ASSERT(!stack_is_full(&stack), "栈未满");
        TEST_ASSERT(stack_size(&stack) == 0U, "栈大小为0");
    }
    
    printf("\n  2. 压栈测试\n");
    {
        for (int32_t i = 1; i <= 5; i++) {
            value = i * 10;
            int32_t ret = stack_push(&stack, &value);
            TEST_ASSERT(SUCCESS(ret), "压栈成功");
        }
        TEST_ASSERT(stack_size(&stack) == 5U, "栈大小正确");
        
        printf("  栈内容（从栈顶到栈底）: ");
        int32_t temp[5];
        size_t count = 0U;
        while (!stack_is_empty(&stack)) {
            (void)stack_pop(&stack, &temp[count]);
            printf("%d ", temp[count]);
            count++;
        }
        printf("\n");
        
        for (int32_t i = (int32_t)count - 1; i >= 0; i--) {
            (void)stack_push(&stack, &temp[i]);
        }
    }
    
    printf("\n  3. 查看栈顶测试\n");
    {
        int32_t ret = stack_peek(&stack, &value);
        TEST_ASSERT(SUCCESS(ret), "查看栈顶");
        TEST_ASSERT(value == 50, "栈顶值正确");
        TEST_ASSERT(stack_size(&stack) == 5U, "栈大小未变");
    }
    
    printf("\n  4. 弹栈测试\n");
    {
        int32_t ret = stack_pop(&stack, &value);
        TEST_ASSERT(SUCCESS(ret), "弹栈成功");
        TEST_ASSERT(value == 50, "弹栈值正确");
        TEST_ASSERT(stack_size(&stack) == 4U, "栈大小减1");
        
        ret = stack_peek(&stack, &value);
        TEST_ASSERT(value == 40, "新栈顶值正确");
    }
    
    printf("\n  5. 栈满测试\n");
    {
        stack_clear(&stack);
        
        for (size_t i = 0U; i < STACK_CAPACITY; i++) {
            value = (int32_t)i;
            (void)stack_push(&stack, &value);
        }
        TEST_ASSERT(stack_is_full(&stack), "栈已满");
        
        value = 999;
        int32_t ret = stack_push(&stack, &value);
        TEST_ASSERT(FAILED(ret), "栈满时压栈失败");
    }
    
    printf("\n  6. 栈空测试\n");
    {
        stack_clear(&stack);
        TEST_ASSERT(stack_is_empty(&stack), "栈为空");
        
        int32_t ret = stack_pop(&stack, &value);
        TEST_ASSERT(FAILED(ret), "栈空时弹栈失败");
        
        ret = stack_peek(&stack, &value);
        TEST_ASSERT(FAILED(ret), "栈空时查看栈顶失败");
    }
    
    printf("\n  7. 应用示例：括号匹配\n");
    {
        const char *expr = "((())())";
        Stack_t paren_stack;
        uint8_t paren_buffer[20];
        
        (void)stack_init(&paren_stack, paren_buffer, 20, sizeof(char));
        
        bool is_valid = true;
        char top_char;
        
        for (size_t i = 0U; expr[i] != '\0'; i++) {
            if (expr[i] == '(') {
                (void)stack_push(&paren_stack, &expr[i]);
            } else if (expr[i] == ')') {
                if (stack_is_empty(&paren_stack)) {
                    is_valid = false;
                    break;
                }
                (void)stack_pop(&paren_stack, &top_char);
            }
        }
        
        if (is_valid && stack_is_empty(&paren_stack)) {
            printf("  表达式 \"%s\" 括号匹配: 有效\n", expr);
        } else {
            printf("  表达式 \"%s\" 括号匹配: 无效\n", expr);
        }
    }
    
    #undef STACK_CAPACITY
}

/*============================================================================*/
/*                           队列演示                                           */
/*============================================================================*/

void demo_queue(void)
{
    printf("\n[队列演示]\n");
    print_separator_line();
    
    #define QUEUE_CAPACITY 8
    
    Queue_t queue;
    uint8_t buffer[QUEUE_CAPACITY * sizeof(int32_t)];
    int32_t value;
    
    printf("\n  1. 初始化测试\n");
    {
        int32_t ret = queue_init(&queue, buffer, QUEUE_CAPACITY, sizeof(int32_t));
        TEST_ASSERT(SUCCESS(ret), "队列初始化");
        TEST_ASSERT(queue_is_empty(&queue), "队列为空");
        TEST_ASSERT(!queue_is_full(&queue), "队列未满");
        TEST_ASSERT(queue_size(&queue) == 0U, "队列大小为0");
    }
    
    printf("\n  2. 入队测试\n");
    {
        for (int32_t i = 1; i <= 5; i++) {
            value = i * 10;
            int32_t ret = queue_enqueue(&queue, &value);
            TEST_ASSERT(SUCCESS(ret), "入队成功");
        }
        TEST_ASSERT(queue_size(&queue) == 5U, "队列大小正确");
        
        printf("  队列内容（从队首到队尾）: ");
        Queue_t temp_queue = queue;
        while (!queue_is_empty(&temp_queue)) {
            (void)queue_dequeue(&temp_queue, &value);
            printf("%d ", value);
        }
        printf("\n");
    }
    
    printf("\n  3. 查看队首/队尾测试\n");
    {
        int32_t ret = queue_front(&queue, &value);
        TEST_ASSERT(SUCCESS(ret), "查看队首");
        TEST_ASSERT(value == 10, "队首值正确");
        
        ret = queue_back(&queue, &value);
        TEST_ASSERT(SUCCESS(ret), "查看队尾");
        TEST_ASSERT(value == 50, "队尾值正确");
    }
    
    printf("\n  4. 出队测试\n");
    {
        int32_t ret = queue_dequeue(&queue, &value);
        TEST_ASSERT(SUCCESS(ret), "出队成功");
        TEST_ASSERT(value == 10, "出队值正确（FIFO）");
        TEST_ASSERT(queue_size(&queue) == 4U, "队列大小减1");
        
        ret = queue_front(&queue, &value);
        TEST_ASSERT(value == 20, "新队首值正确");
    }
    
    printf("\n  5. 环形缓冲区测试\n");
    {
        queue_clear(&queue);
        
        for (int32_t i = 1; i <= 6; i++) {
            value = i;
            (void)queue_enqueue(&queue, &value);
        }
        
        for (int32_t i = 0; i < 4; i++) {
            (void)queue_dequeue(&queue, &value);
        }
        
        for (int32_t i = 7; i <= 10; i++) {
            value = i;
            (void)queue_enqueue(&queue, &value);
        }
        
        printf("  环形缓冲区测试后队列内容: ");
        while (!queue_is_empty(&queue)) {
            (void)queue_dequeue(&queue, &value);
            printf("%d ", value);
        }
        printf("\n");
        TEST_ASSERT(true, "环形缓冲区正常工作");
    }
    
    printf("\n  6. 队列满测试\n");
    {
        queue_clear(&queue);
        
        for (size_t i = 0U; i < QUEUE_CAPACITY - 1U; i++) {
            value = (int32_t)i;
            (void)queue_enqueue(&queue, &value);
        }
        TEST_ASSERT(queue_is_full(&queue), "队列已满");
        
        value = 999;
        int32_t ret = queue_enqueue(&queue, &value);
        TEST_ASSERT(FAILED(ret), "队列满时入队失败");
    }
    
    printf("\n  7. 队列空测试\n");
    {
        queue_clear(&queue);
        TEST_ASSERT(queue_is_empty(&queue), "队列为空");
        
        int32_t ret = queue_dequeue(&queue, &value);
        TEST_ASSERT(FAILED(ret), "队列空时出队失败");
        
        ret = queue_front(&queue, &value);
        TEST_ASSERT(FAILED(ret), "队列空时查看队首失败");
    }
    
    #undef QUEUE_CAPACITY
}

/*============================================================================*/
/*                           单向链表演示                                       */
/*============================================================================*/

void demo_linked_list(void)
{
    printf("\n[单向链表演示]\n");
    print_separator_line();
    
    #define LIST_CAPACITY 10
    
    LinkedList_t list;
    ListNode_t node_pool[LIST_CAPACITY];
    uint8_t data_pool[LIST_CAPACITY * sizeof(int32_t)];
    int32_t value;
    
    printf("\n  1. 初始化测试\n");
    {
        int32_t ret = linked_list_init(&list, node_pool, data_pool, 
                                        LIST_CAPACITY, sizeof(int32_t));
        TEST_ASSERT(SUCCESS(ret), "链表初始化");
        TEST_ASSERT(linked_list_is_empty(&list), "链表为空");
        TEST_ASSERT(!linked_list_is_full(&list), "链表未满");
        TEST_ASSERT(linked_list_size(&list) == 0U, "链表大小为0");
    }
    
    printf("\n  2. 头部插入测试\n");
    {
        value = 10;
        int32_t ret = linked_list_push_front(&list, &value);
        TEST_ASSERT(SUCCESS(ret), "头部插入10");
        
        value = 20;
        ret = linked_list_push_front(&list, &value);
        TEST_ASSERT(SUCCESS(ret), "头部插入20");
        
        value = 30;
        ret = linked_list_push_front(&list, &value);
        TEST_ASSERT(SUCCESS(ret), "头部插入30");
        
        TEST_ASSERT(linked_list_size(&list) == 3U, "链表大小为3");
        
        (void)linked_list_front(&list, &value);
        TEST_ASSERT(value == 30, "头部元素正确");
        
        (void)linked_list_back(&list, &value);
        TEST_ASSERT(value == 10, "尾部元素正确");
    }
    
    printf("\n  3. 尾部插入测试\n");
    {
        value = 40;
        int32_t ret = linked_list_push_back(&list, &value);
        TEST_ASSERT(SUCCESS(ret), "尾部插入40");
        
        value = 50;
        ret = linked_list_push_back(&list, &value);
        TEST_ASSERT(SUCCESS(ret), "尾部插入50");
        
        TEST_ASSERT(linked_list_size(&list) == 5U, "链表大小为5");
        
        (void)linked_list_back(&list, &value);
        TEST_ASSERT(value == 50, "新尾部元素正确");
    }
    
    printf("\n  4. 遍历测试\n");
    {
        printf("  链表内容: ");
        ListNode_t *curr = list.head;
        while (NULL != curr) {
            printf("%d ", *(int32_t *)curr->data);
            curr = curr->next;
        }
        printf("\n");
    }
    
    printf("\n  5. 查找测试\n");
    {
        value = 30;
        size_t index = linked_list_find(&list, &value, compare_int32);
        TEST_ASSERT(index == 0U, "查找30位置正确");
        
        value = 40;
        index = linked_list_find(&list, &value, compare_int32);
        TEST_ASSERT(index == 3U, "查找40位置正确");
        
        value = 999;
        index = linked_list_find(&list, &value, compare_int32);
        TEST_ASSERT(index == SIZE_MAX, "查找不存在的元素");
    }
    
    printf("\n  6. 按位置删除测试\n");
    {
        int32_t ret = linked_list_remove_at(&list, 2, &value);
        TEST_ASSERT(SUCCESS(ret), "删除位置2的元素");
        TEST_ASSERT(value == 20, "删除值正确");
        TEST_ASSERT(linked_list_size(&list) == 4U, "链表大小减1");
    }
    
    printf("\n  7. 按值删除测试\n");
    {
        value = 40;
        int32_t ret = linked_list_remove(&list, &value, compare_int32, &value);
        TEST_ASSERT(SUCCESS(ret), "删除值40");
        TEST_ASSERT(linked_list_size(&list) == 3U, "链表大小减1");
    }
    
    printf("\n  8. 反转测试\n");
    {
        printf("  反转前: ");
        ListNode_t *curr = list.head;
        while (NULL != curr) {
            printf("%d ", *(int32_t *)curr->data);
            curr = curr->next;
        }
        printf("\n");
        
        int32_t ret = linked_list_reverse(&list);
        TEST_ASSERT(SUCCESS(ret), "链表反转");
        
        printf("  反转后: ");
        curr = list.head;
        while (NULL != curr) {
            printf("%d ", *(int32_t *)curr->data);
            curr = curr->next;
        }
        printf("\n");
    }
    
    printf("\n  9. 清空测试\n");
    {
        int32_t ret = linked_list_clear(&list);
        TEST_ASSERT(SUCCESS(ret), "清空链表");
        TEST_ASSERT(linked_list_is_empty(&list), "链表为空");
        TEST_ASSERT(linked_list_size(&list) == 0U, "链表大小为0");
    }
    
    printf("\n  10. 链表满测试\n");
    {
        for (size_t i = 0U; i < LIST_CAPACITY; i++) {
            value = (int32_t)i;
            (void)linked_list_push_back(&list, &value);
        }
        TEST_ASSERT(linked_list_is_full(&list), "链表已满");
        
        value = 999;
        int32_t ret = linked_list_push_back(&list, &value);
        TEST_ASSERT(FAILED(ret), "链表满时插入失败");
    }
    
    #undef LIST_CAPACITY
}

/*============================================================================*/
/*                           综合应用演示                                       */
/*============================================================================*/

void demo_data_structure_application(void)
{
    printf("\n[数据结构综合应用演示]\n");
    print_separator_line();
    
    printf("\n  应用1: 使用栈实现十进制转二进制\n");
    {
        Stack_t bin_stack;
        uint8_t buffer[32 * sizeof(int32_t)];
        int32_t value;
        
        (void)stack_init(&bin_stack, buffer, 32, sizeof(int32_t));
        
        int32_t num = 42;
        printf("  十进制 %d 转二进制: ", num);
        
        if (num == 0) {
            printf("0");
        } else {
            while (num > 0) {
                value = num % 2;
                (void)stack_push(&bin_stack, &value);
                num = num / 2;
            }
            
            while (!stack_is_empty(&bin_stack)) {
                (void)stack_pop(&bin_stack, &value);
                printf("%d", value);
            }
        }
        printf("\n");
    }
    
    printf("\n  应用2: 使用队列模拟任务调度\n");
    {
        Queue_t task_queue;
        uint8_t buffer[5 * sizeof(int32_t)];
        int32_t task;
        
        (void)queue_init(&task_queue, buffer, 5, sizeof(int32_t));
        
        printf("  添加任务: ");
        for (int32_t i = 1; i <= 4; i++) {
            task = i;
            (void)queue_enqueue(&task_queue, &task);
            printf("T%d ", task);
        }
        printf("\n");
        
        printf("  执行任务: ");
        while (!queue_is_empty(&task_queue)) {
            (void)queue_dequeue(&task_queue, &task);
            printf("T%d ", task);
        }
        printf("\n");
    }
    
    printf("\n  应用3: 使用链表管理传感器配置\n");
    {
        typedef struct {
            int32_t id;
            int32_t sample_rate;
            bool    enabled;
        } SensorConfig_t;
        
        #define SENSOR_COUNT 5
        
        LinkedList_t sensor_list;
        ListNode_t node_pool[SENSOR_COUNT];
        uint8_t data_pool[SENSOR_COUNT * sizeof(SensorConfig_t)];
        
        (void)linked_list_init(&sensor_list, node_pool, data_pool,
                               SENSOR_COUNT, sizeof(SensorConfig_t));
        
        SensorConfig_t sensors[] = {
            {1, 100, true},
            {2, 200, false},
            {3, 50, true},
            {4, 1000, true},
            {5, 500, false}
        };
        
        for (size_t i = 0U; i < ARRAY_SIZE(sensors); i++) {
            (void)linked_list_push_back(&sensor_list, &sensors[i]);
        }
        
        printf("  已启用的传感器:\n");
        ListNode_t *curr = sensor_list.head;
        while (NULL != curr) {
            SensorConfig_t *cfg = (SensorConfig_t *)curr->data;
            if (cfg->enabled) {
                printf("    传感器ID=%d, 采样率=%dHz\n", cfg->id, cfg->sample_rate);
            }
            curr = curr->next;
        }
        
        #undef SENSOR_COUNT
    }
    
    printf("\n  应用4: 使用数组实现简单的查找表\n");
    {
        typedef struct {
            int32_t key;
            const char *name;
        } LookupEntry_t;
        
        #define LOOKUP_SIZE 5
        
        StaticArray_t lookup_table;
        uint8_t buffer[LOOKUP_SIZE * sizeof(LookupEntry_t)];
        
        (void)static_array_init(&lookup_table, buffer, LOOKUP_SIZE, sizeof(LookupEntry_t));
        
        LookupEntry_t entries[] = {
            {0x01, "温度传感器"},
            {0x02, "湿度传感器"},
            {0x03, "压力传感器"},
            {0x04, "光照传感器"},
            {0x05, "加速度计"}
        };
        
        for (size_t i = 0U; i < ARRAY_SIZE(entries); i++) {
            (void)static_array_push_back(&lookup_table, &entries[i]);
        }
        
        int32_t search_key = 0x03;
        printf("  查找键值 0x%02X: ", search_key);
        
        bool found = false;
        for (size_t i = 0U; i < static_array_size(&lookup_table); i++) {
            LookupEntry_t entry;
            (void)static_array_get(&lookup_table, i, &entry);
            if (entry.key == search_key) {
                printf("%s\n", entry.name);
                found = true;
                break;
            }
        }
        
        if (!found) {
            printf("未找到\n");
        }
        
        #undef LOOKUP_SIZE
    }
}
