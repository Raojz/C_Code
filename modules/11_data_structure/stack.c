/**
 * @file stack.c
 * @brief 栈数据结构实现
 * @details 基于数组实现的栈操作函数。
 * 
 * @author Embedded C Learning Project
 * @date 2026-04-10
 * @version 1.0.0
 */

#include "stack.h"
#include <string.h>

/*============================================================================*/
/*                           内部宏定义                                         */
/*============================================================================*/

/** @brief 计算元素在数组中的偏移量 */
#define ELEMENT_OFFSET(stack, index)    ((index) * (stack)->element_size)

/** @brief 获取指定位置元素的指针 */
#define ELEMENT_PTR(stack, index)       ((void *)((stack)->data + ELEMENT_OFFSET(stack, index)))

/*============================================================================*/
/*                           函数实现                                           */
/*============================================================================*/

int32_t stack_init(Stack_t *stack, 
                   uint8_t *buffer, 
                   size_t capacity, 
                   size_t element_size)
{
    if ((NULL == stack) || (NULL == buffer)) {
        return STATUS_NULL_PTR;
    }
    
    if ((0U == capacity) || (0U == element_size)) {
        return STATUS_INVALID_PARAM;
    }
    
    stack->data = buffer;
    stack->top = 0U;
    stack->capacity = capacity;
    stack->element_size = element_size;
    
    return STATUS_OK;
}

size_t stack_size(const Stack_t *stack)
{
    if (NULL == stack) {
        return 0U;
    }
    
    return stack->top;
}

size_t stack_capacity(const Stack_t *stack)
{
    if (NULL == stack) {
        return 0U;
    }
    
    return stack->capacity;
}

bool stack_is_empty(const Stack_t *stack)
{
    if (NULL == stack) {
        return true;
    }
    
    return (0U == stack->top);
}

bool stack_is_full(const Stack_t *stack)
{
    if (NULL == stack) {
        return true;
    }
    
    return (stack->top >= stack->capacity);
}

int32_t stack_push(Stack_t *stack, const void *value)
{
    if ((NULL == stack) || (NULL == value)) {
        return STATUS_NULL_PTR;
    }
    
    if (stack->top >= stack->capacity) {
        return STATUS_OVERFLOW;
    }
    
    void *dest = ELEMENT_PTR(stack, stack->top);
    (void)memcpy(dest, value, stack->element_size);
    stack->top++;
    
    return STATUS_OK;
}

int32_t stack_pop(Stack_t *stack, void *out_value)
{
    if (NULL == stack) {
        return STATUS_NULL_PTR;
    }
    
    if (0U == stack->top) {
        return STATUS_UNDERFLOW;
    }
    
    stack->top--;
    
    if (NULL != out_value) {
        const void *src = ELEMENT_PTR(stack, stack->top);
        (void)memcpy(out_value, src, stack->element_size);
    }
    
    return STATUS_OK;
}

int32_t stack_peek(const Stack_t *stack, void *out_value)
{
    if ((NULL == stack) || (NULL == out_value)) {
        return STATUS_NULL_PTR;
    }
    
    if (0U == stack->top) {
        return STATUS_UNDERFLOW;
    }
    
    const void *src = ELEMENT_PTR(stack, stack->top - 1U);
    (void)memcpy(out_value, src, stack->element_size);
    
    return STATUS_OK;
}

int32_t stack_clear(Stack_t *stack)
{
    if (NULL == stack) {
        return STATUS_NULL_PTR;
    }
    
    stack->top = 0U;
    
    return STATUS_OK;
}

int32_t stack_foreach(Stack_t *stack, 
                      bool (*callback)(void *element, void *user_data),
                      void *user_data)
{
    if ((NULL == stack) || (NULL == callback)) {
        return STATUS_NULL_PTR;
    }
    
    for (size_t i = stack->top; i > 0U; i--) {
        void *element = ELEMENT_PTR(stack, i - 1U);
        if (!callback(element, user_data)) {
            break;
        }
    }
    
    return STATUS_OK;
}
