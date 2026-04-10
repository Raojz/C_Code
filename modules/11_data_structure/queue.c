/**
 * @file queue.c
 * @brief 队列数据结构实现
 * @details 基于环形缓冲区实现的队列操作函数。
 * 
 * @author Embedded C Learning Project
 * @date 2026-04-10
 * @version 1.0.0
 */

#include "queue.h"
#include <string.h>

/*============================================================================*/
/*                           内部宏定义                                         */
/*============================================================================*/

/** @brief 计算元素在数组中的偏移量 */
#define ELEMENT_OFFSET(queue, index)    ((index) * (queue)->element_size)

/** @brief 获取指定位置元素的指针 */
#define ELEMENT_PTR(queue, index)       ((void *)((queue)->data + ELEMENT_OFFSET(queue, index)))

/** @brief 计算环形缓冲区的下一个位置 */
#define NEXT_INDEX(queue, index)        (((index) + 1U) % (queue)->capacity)

/** @brief 计算环形缓冲区的前一个位置 */
#define PREV_INDEX(queue, index)        (((index) + (queue)->capacity - 1U) % (queue)->capacity)

/*============================================================================*/
/*                           函数实现                                           */
/*============================================================================*/

int32_t queue_init(Queue_t *queue, 
                   uint8_t *buffer, 
                   size_t capacity, 
                   size_t element_size)
{
    if ((NULL == queue) || (NULL == buffer)) {
        return STATUS_NULL_PTR;
    }
    
    if ((0U == capacity) || (0U == element_size)) {
        return STATUS_INVALID_PARAM;
    }
    
    queue->data = buffer;
    queue->head = 0U;
    queue->tail = 0U;
    queue->capacity = capacity;
    queue->element_size = element_size;
    
    return STATUS_OK;
}

size_t queue_size(const Queue_t *queue)
{
    if (NULL == queue) {
        return 0U;
    }
    
    if (queue->tail >= queue->head) {
        return (queue->tail - queue->head);
    } else {
        return (queue->capacity - queue->head + queue->tail);
    }
}

size_t queue_capacity(const Queue_t *queue)
{
    if (NULL == queue) {
        return 0U;
    }
    
    return queue->capacity;
}

bool queue_is_empty(const Queue_t *queue)
{
    if (NULL == queue) {
        return true;
    }
    
    return (queue->head == queue->tail);
}

bool queue_is_full(const Queue_t *queue)
{
    if (NULL == queue) {
        return true;
    }
    
    return (NEXT_INDEX(queue, queue->tail) == queue->head);
}

int32_t queue_enqueue(Queue_t *queue, const void *value)
{
    if ((NULL == queue) || (NULL == value)) {
        return STATUS_NULL_PTR;
    }
    
    if (queue_is_full(queue)) {
        return STATUS_OVERFLOW;
    }
    
    void *dest = ELEMENT_PTR(queue, queue->tail);
    (void)memcpy(dest, value, queue->element_size);
    queue->tail = NEXT_INDEX(queue, queue->tail);
    
    return STATUS_OK;
}

int32_t queue_dequeue(Queue_t *queue, void *out_value)
{
    if (NULL == queue) {
        return STATUS_NULL_PTR;
    }
    
    if (queue_is_empty(queue)) {
        return STATUS_UNDERFLOW;
    }
    
    if (NULL != out_value) {
        const void *src = ELEMENT_PTR(queue, queue->head);
        (void)memcpy(out_value, src, queue->element_size);
    }
    
    queue->head = NEXT_INDEX(queue, queue->head);
    
    return STATUS_OK;
}

int32_t queue_front(const Queue_t *queue, void *out_value)
{
    if ((NULL == queue) || (NULL == out_value)) {
        return STATUS_NULL_PTR;
    }
    
    if (queue_is_empty(queue)) {
        return STATUS_UNDERFLOW;
    }
    
    const void *src = ELEMENT_PTR(queue, queue->head);
    (void)memcpy(out_value, src, queue->element_size);
    
    return STATUS_OK;
}

int32_t queue_back(const Queue_t *queue, void *out_value)
{
    if ((NULL == queue) || (NULL == out_value)) {
        return STATUS_NULL_PTR;
    }
    
    if (queue_is_empty(queue)) {
        return STATUS_UNDERFLOW;
    }
    
    size_t back_index = PREV_INDEX(queue, queue->tail);
    const void *src = ELEMENT_PTR(queue, back_index);
    (void)memcpy(out_value, src, queue->element_size);
    
    return STATUS_OK;
}

int32_t queue_clear(Queue_t *queue)
{
    if (NULL == queue) {
        return STATUS_NULL_PTR;
    }
    
    queue->head = 0U;
    queue->tail = 0U;
    
    return STATUS_OK;
}

int32_t queue_foreach(Queue_t *queue, 
                      bool (*callback)(void *element, void *user_data),
                      void *user_data)
{
    if ((NULL == queue) || (NULL == callback)) {
        return STATUS_NULL_PTR;
    }
    
    size_t index = queue->head;
    
    while (index != queue->tail) {
        void *element = ELEMENT_PTR(queue, index);
        if (!callback(element, user_data)) {
            break;
        }
        index = NEXT_INDEX(queue, index);
    }
    
    return STATUS_OK;
}
