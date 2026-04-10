/**
 * @file queue.h
 * @brief 队列数据结构头文件
 * @details 基于环形缓冲区实现的队列（FIFO - 先进先出），支持静态内存分配。
 *          专为嵌入式环境设计，避免动态内存分配。
 * 
 * @section features 主要特性
 * - 编译时确定容量，无动态内存分配
 * - 基于环形缓冲区实现，空间利用率高
 * - 支持入队（enqueue）、出队（dequeue）、查看队首（front）操作
 * - 支持队列空、队列满判断
 * - 所有操作时间复杂度均为O(1)
 * - 支持获取队列中元素个数
 * 
 * @section applications 典型应用场景
 * - 数据缓冲（串口接收缓冲区）
 * - 任务调度队列
 * - 消息队列
 * - 事件队列
 * - 生产者-消费者模型
 * - BFS广度优先搜索
 * 
 * @section implementation 实现原理
 * 使用环形缓冲区实现队列：
 * - head指针：指向队首元素（出队位置）
 * - tail指针：指向队尾元素的下一个位置（入队位置）
 * - 当head == tail时，队列为空
 * - 当(tail + 1) % capacity == head时，队列满
 * - 注意：实际可用容量为capacity - 1
 * 
 * @section usage 使用示例
 * @code
 * // 定义一个容量为32的整型队列
 * QUEUE_DECLARE(int_queue, int32_t, 32)
 * 
 * // 初始化
 * int_queue_t queue;
 * queue_init(&queue);
 * 
 * // 入队
 * queue_enqueue(&queue, 100);
 * queue_enqueue(&queue, 200);
 * 
 * // 查看队首
 * int32_t front;
 * if (SUCCESS(queue_front(&queue, &front))) {
 *     printf("队首元素: %d\n", front);  // 输出: 100
 * }
 * 
 * // 出队
 * queue_dequeue(&queue, &front);  // front = 100
 * queue_dequeue(&queue, &front);  // front = 200
 * @endcode
 * 
 * @section complexity 时间复杂度
 * - 入队(enqueue): O(1)
 * - 出队(dequeue): O(1)
 * - 查看队首(front): O(1)
 * - 判断空/满: O(1)
 * 
 * @author Embedded C Learning Project
 * @date 2026-04-10
 * @version 1.0.0
 * 
 * @note 遵循 MISRA-C:2012 编码规范
 * @warning 非线程安全，多线程环境需外部加锁
 * @warning 实际可用容量为 capacity - 1
 */

#ifndef QUEUE_H
#define QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common/common_types.h"

/*============================================================================*/
/*                           宏定义                                             */
/*============================================================================*/

/**
 * @brief 声明队列类型
 * @param name 队列类型名称
 * @param type 元素数据类型
 * @param capacity 队列容量（实际可用容量为capacity - 1）
 * @details 使用此宏在编译时定义静态队列结构体类型
 * @code
 * QUEUE_DECLARE(my_queue, int32_t, 100)
 * // 展开后生成: typedef struct { ... } my_queue_t;
 * @endcode
 */
#define QUEUE_DECLARE(name, type, capacity)                                  \
    typedef struct {                                                         \
        type       data[capacity];    /**< 数据存储区 */                      \
        size_t     head;              /**< 队首指针（出队位置） */             \
        size_t     tail;              /**< 队尾指针（入队位置） */             \
        size_t     capacity;          /**< 队列容量 */                        \
    } name##_t

/*============================================================================*/
/*                           通用队列结构体                                     */
/*============================================================================*/

/**
 * @brief 通用队列结构体（使用void指针存储）
 * @details 用于需要存储任意类型数据的场景
 * @warning 使用此结构体需要额外传入元素大小
 */
typedef struct {
    uint8_t    *data;         /**< 数据存储区指针 */
    size_t     head;          /**< 队首指针（出队位置） */
    size_t     tail;          /**< 队尾指针（入队位置） */
    size_t     capacity;      /**< 队列容量 */
    size_t     element_size;  /**< 单个元素大小（字节） */
} Queue_t;

/*============================================================================*/
/*                           函数声明                                           */
/*============================================================================*/

/**
 * @brief 初始化通用队列
 * @param queue 队列指针（不能为NULL）
 * @param buffer 数据缓冲区指针（不能为NULL）
 * @param capacity 队列容量
 * @param element_size 单个元素大小（字节）
 * @retval STATUS_OK 初始化成功
 * @retval STATUS_NULL_PTR 参数为NULL
 * @retval STATUS_INVALID_PARAM 容量或元素大小为0
 * @note 必须先调用此函数初始化队列，才能使用其他操作
 */
int32_t queue_init(Queue_t *queue, 
                   uint8_t *buffer, 
                   size_t capacity, 
                   size_t element_size);

/**
 * @brief 获取队列中元素个数
 * @param queue 队列指针（不能为NULL）
 * @return 队列中元素个数，失败返回0
 */
size_t queue_size(const Queue_t *queue);

/**
 * @brief 获取队列容量
 * @param queue 队列指针（不能为NULL）
 * @return 队列容量，失败返回0
 */
size_t queue_capacity(const Queue_t *queue);

/**
 * @brief 检查队列是否为空
 * @param queue 队列指针（不能为NULL）
 * @return true表示队列为空，false表示队列非空
 */
bool queue_is_empty(const Queue_t *queue);

/**
 * @brief 检查队列是否已满
 * @param queue 队列指针（不能为NULL）
 * @return true表示队列已满，false表示队列未满
 */
bool queue_is_full(const Queue_t *queue);

/**
 * @brief 入队操作
 * @param queue 队列指针（不能为NULL）
 * @param value 要入队的元素值指针（不能为NULL）
 * @retval STATUS_OK 入队成功
 * @retval STATUS_NULL_PTR 参数为NULL
 * @retval STATUS_OVERFLOW 队列已满
 * @note 时间复杂度: O(1)
 */
int32_t queue_enqueue(Queue_t *queue, const void *value);

/**
 * @brief 出队操作
 * @param queue 队列指针（不能为NULL）
 * @param out_value 输出被出队的元素值指针（可为NULL）
 * @retval STATUS_OK 出队成功
 * @retval STATUS_NULL_PTR 队列指针为NULL
 * @retval STATUS_UNDERFLOW 队列为空
 * @note 时间复杂度: O(1)
 */
int32_t queue_dequeue(Queue_t *queue, void *out_value);

/**
 * @brief 查看队首元素（不出队）
 * @param queue 队列指针（不能为NULL）
 * @param out_value 输出队首元素值指针（不能为NULL）
 * @retval STATUS_OK 获取成功
 * @retval STATUS_NULL_PTR 参数为NULL
 * @retval STATUS_UNDERFLOW 队列为空
 * @note 时间复杂度: O(1)
 */
int32_t queue_front(const Queue_t *queue, void *out_value);

/**
 * @brief 查看队尾元素（不出队）
 * @param queue 队列指针（不能为NULL）
 * @param out_value 输出队尾元素值指针（不能为NULL）
 * @retval STATUS_OK 获取成功
 * @retval STATUS_NULL_PTR 参数为NULL
 * @retval STATUS_UNDERFLOW 队列为空
 * @note 时间复杂度: O(1)
 */
int32_t queue_back(const Queue_t *queue, void *out_value);

/**
 * @brief 清空队列
 * @param queue 队列指针（不能为NULL）
 * @retval STATUS_OK 清空成功
 * @retval STATUS_NULL_PTR 参数为NULL
 * @note 仅将head和tail置0，不释放内存
 */
int32_t queue_clear(Queue_t *queue);

/**
 * @brief 遍历队列（从队首到队尾）
 * @param queue 队列指针（不能为NULL）
 * @param callback 回调函数指针（不能为NULL）
 * @param user_data 用户数据指针（可传NULL）
 * @retval STATUS_OK 遍历完成
 * @retval STATUS_NULL_PTR 参数为NULL
 * @note 回调函数返回true继续遍历，返回false停止遍历
 */
int32_t queue_foreach(Queue_t *queue, 
                      bool (*callback)(void *element, void *user_data),
                      void *user_data);

#ifdef __cplusplus
}
#endif

#endif
