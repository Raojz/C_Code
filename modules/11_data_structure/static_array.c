/**
 * @file static_array.c
 * @brief 静态数组数据结构实现
 * @details 实现带边界检查的静态数组操作函数。
 * 
 * @author Embedded C Learning Project
 * @date 2026-04-10
 * @version 1.0.0
 */

#include "static_array.h"
#include <string.h>

/*============================================================================*/
/*                           内部宏定义                                         */
/*============================================================================*/

/** @brief 计算元素在数组中的偏移量 */
#define ELEMENT_OFFSET(arr, index)    ((index) * (arr)->element_size)

/** @brief 获取指定位置元素的指针 */
#define ELEMENT_PTR(arr, index)       ((void *)((arr)->data + ELEMENT_OFFSET(arr, index)))

/*============================================================================*/
/*                           函数实现                                           */
/*============================================================================*/

int32_t static_array_init(StaticArray_t *arr, 
                          uint8_t *buffer, 
                          size_t capacity, 
                          size_t element_size)
{
    if ((NULL == arr) || (NULL == buffer)) {
        return STATUS_NULL_PTR;
    }
    
    if ((0U == capacity) || (0U == element_size)) {
        return STATUS_INVALID_PARAM;
    }
    
    arr->data = buffer;
    arr->size = 0U;
    arr->capacity = capacity;
    arr->element_size = element_size;
    
    return STATUS_OK;
}

size_t static_array_size(const StaticArray_t *arr)
{
    if (NULL == arr) {
        return 0U;
    }
    
    return arr->size;
}

size_t static_array_capacity(const StaticArray_t *arr)
{
    if (NULL == arr) {
        return 0U;
    }
    
    return arr->capacity;
}

bool static_array_is_empty(const StaticArray_t *arr)
{
    if (NULL == arr) {
        return true;
    }
    
    return (0U == arr->size);
}

bool static_array_is_full(const StaticArray_t *arr)
{
    if (NULL == arr) {
        return true;
    }
    
    return (arr->size >= arr->capacity);
}

int32_t static_array_get(const StaticArray_t *arr, 
                         size_t index, 
                         void *out_value)
{
    if ((NULL == arr) || (NULL == out_value)) {
        return STATUS_NULL_PTR;
    }
    
    if (index >= arr->size) {
        return STATUS_INVALID_PARAM;
    }
    
    const void *src = ELEMENT_PTR(arr, index);
    (void)memcpy(out_value, src, arr->element_size);
    
    return STATUS_OK;
}

int32_t static_array_set(StaticArray_t *arr, 
                         size_t index, 
                         const void *value)
{
    if ((NULL == arr) || (NULL == value)) {
        return STATUS_NULL_PTR;
    }
    
    if (index >= arr->size) {
        return STATUS_INVALID_PARAM;
    }
    
    void *dest = ELEMENT_PTR(arr, index);
    (void)memcpy(dest, value, arr->element_size);
    
    return STATUS_OK;
}

int32_t static_array_push_back(StaticArray_t *arr, const void *value)
{
    if ((NULL == arr) || (NULL == value)) {
        return STATUS_NULL_PTR;
    }
    
    if (arr->size >= arr->capacity) {
        return STATUS_OVERFLOW;
    }
    
    void *dest = ELEMENT_PTR(arr, arr->size);
    (void)memcpy(dest, value, arr->element_size);
    arr->size++;
    
    return STATUS_OK;
}

int32_t static_array_pop_back(StaticArray_t *arr, void *out_value)
{
    if (NULL == arr) {
        return STATUS_NULL_PTR;
    }
    
    if (0U == arr->size) {
        return STATUS_UNDERFLOW;
    }
    
    arr->size--;
    
    if (NULL != out_value) {
        const void *src = ELEMENT_PTR(arr, arr->size);
        (void)memcpy(out_value, src, arr->element_size);
    }
    
    return STATUS_OK;
}

int32_t static_array_insert(StaticArray_t *arr, 
                            size_t index, 
                            const void *value)
{
    if ((NULL == arr) || (NULL == value)) {
        return STATUS_NULL_PTR;
    }
    
    if (index > arr->size) {
        return STATUS_INVALID_PARAM;
    }
    
    if (arr->size >= arr->capacity) {
        return STATUS_OVERFLOW;
    }
    
    if (index < arr->size) {
        void *dest = ELEMENT_PTR(arr, index + 1U);
        const void *src = ELEMENT_PTR(arr, index);
        size_t move_size = (arr->size - index) * arr->element_size;
        (void)memmove(dest, src, move_size);
    }
    
    void *dest = ELEMENT_PTR(arr, index);
    (void)memcpy(dest, value, arr->element_size);
    arr->size++;
    
    return STATUS_OK;
}

int32_t static_array_remove(StaticArray_t *arr, 
                            size_t index, 
                            void *out_value)
{
    if (NULL == arr) {
        return STATUS_NULL_PTR;
    }
    
    if (index >= arr->size) {
        return STATUS_INVALID_PARAM;
    }
    
    if (NULL != out_value) {
        const void *src = ELEMENT_PTR(arr, index);
        (void)memcpy(out_value, src, arr->element_size);
    }
    
    if (index < (arr->size - 1U)) {
        void *dest = ELEMENT_PTR(arr, index);
        const void *src = ELEMENT_PTR(arr, index + 1U);
        size_t move_size = (arr->size - index - 1U) * arr->element_size;
        (void)memmove(dest, src, move_size);
    }
    
    arr->size--;
    
    return STATUS_OK;
}

size_t static_array_find(const StaticArray_t *arr, 
                         const void *value, 
                         CompareFunc_t compare)
{
    if ((NULL == arr) || (NULL == value) || (NULL == compare)) {
        return SIZE_MAX;
    }
    
    for (size_t i = 0U; i < arr->size; i++) {
        const void *element = ELEMENT_PTR(arr, i);
        if (0 == compare(element, value)) {
            return i;
        }
    }
    
    return SIZE_MAX;
}

int32_t static_array_clear(StaticArray_t *arr)
{
    if (NULL == arr) {
        return STATUS_NULL_PTR;
    }
    
    arr->size = 0U;
    
    return STATUS_OK;
}

int32_t static_array_foreach(StaticArray_t *arr, 
                             bool (*callback)(void *element, void *user_data),
                             void *user_data)
{
    if ((NULL == arr) || (NULL == callback)) {
        return STATUS_NULL_PTR;
    }
    
    for (size_t i = 0U; i < arr->size; i++) {
        void *element = ELEMENT_PTR(arr, i);
        if (!callback(element, user_data)) {
            break;
        }
    }
    
    return STATUS_OK;
}
