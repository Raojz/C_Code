/**
 * @file linked_list.c
 * @brief 单向链表数据结构实现
 * @details 基于静态内存池实现的链表操作函数。
 * 
 * @author Embedded C Learning Project
 * @date 2026-04-10
 * @version 1.0.0
 */

#include "linked_list.h"
#include <string.h>

/*============================================================================*/
/*                           内部宏定义                                         */
/*============================================================================*/

/** @brief 计算数据在数据池中的偏移量 */
#define DATA_OFFSET(list, index)    ((index) * (list)->element_size)

/** @brief 获取指定位置数据的指针 */
#define DATA_PTR(list, index)       ((void *)((list)->data_pool + DATA_OFFSET(list, index)))

/*============================================================================*/
/*                           内部函数                                           */
/*============================================================================*/

/**
 * @brief 从空闲链表分配一个节点
 * @param list 链表指针
 * @return 分配的节点指针，失败返回NULL
 */
static ListNode_t *allocate_node(LinkedList_t *list)
{
    if ((NULL == list) || (NULL == list->free_list)) {
        return NULL;
    }
    
    ListNode_t *node = list->free_list;
    list->free_list = node->next;
    
    return node;
}

/**
 * @brief 将节点归还到空闲链表
 * @param list 链表指针
 * @param node 要归还的节点指针
 */
static void free_node(LinkedList_t *list, ListNode_t *node)
{
    if ((NULL == list) || (NULL == node)) {
        return;
    }
    
    node->next = list->free_list;
    list->free_list = node;
}

/*============================================================================*/
/*                           函数实现                                           */
/*============================================================================*/

int32_t linked_list_init(LinkedList_t *list, 
                         ListNode_t *node_pool,
                         uint8_t *data_pool,
                         size_t capacity, 
                         size_t element_size)
{
    if ((NULL == list) || (NULL == node_pool) || (NULL == data_pool)) {
        return STATUS_NULL_PTR;
    }
    
    if ((0U == capacity) || (0U == element_size)) {
        return STATUS_INVALID_PARAM;
    }
    
    list->node_pool = node_pool;
    list->data_pool = data_pool;
    list->head = NULL;
    list->tail = NULL;
    list->size = 0U;
    list->capacity = capacity;
    list->element_size = element_size;
    
    for (size_t i = 0U; i < capacity; i++) {
        list->node_pool[i].data = DATA_PTR(list, i);
        list->node_pool[i].next = (i < (capacity - 1U)) ? 
                                   &list->node_pool[i + 1U] : NULL;
    }
    list->free_list = &list->node_pool[0];
    
    return STATUS_OK;
}

size_t linked_list_size(const LinkedList_t *list)
{
    if (NULL == list) {
        return 0U;
    }
    
    return list->size;
}

size_t linked_list_capacity(const LinkedList_t *list)
{
    if (NULL == list) {
        return 0U;
    }
    
    return list->capacity;
}

bool linked_list_is_empty(const LinkedList_t *list)
{
    if (NULL == list) {
        return true;
    }
    
    return (NULL == list->head);
}

bool linked_list_is_full(const LinkedList_t *list)
{
    if (NULL == list) {
        return true;
    }
    
    return (NULL == list->free_list);
}

int32_t linked_list_push_front(LinkedList_t *list, const void *value)
{
    if ((NULL == list) || (NULL == value)) {
        return STATUS_NULL_PTR;
    }
    
    ListNode_t *node = allocate_node(list);
    if (NULL == node) {
        return STATUS_OVERFLOW;
    }
    
    (void)memcpy(node->data, value, list->element_size);
    
    node->next = list->head;
    list->head = node;
    
    if (NULL == list->tail) {
        list->tail = node;
    }
    
    list->size++;
    
    return STATUS_OK;
}

int32_t linked_list_push_back(LinkedList_t *list, const void *value)
{
    if ((NULL == list) || (NULL == value)) {
        return STATUS_NULL_PTR;
    }
    
    ListNode_t *node = allocate_node(list);
    if (NULL == node) {
        return STATUS_OVERFLOW;
    }
    
    (void)memcpy(node->data, value, list->element_size);
    node->next = NULL;
    
    if (NULL == list->tail) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }
    
    list->size++;
    
    return STATUS_OK;
}

int32_t linked_list_pop_front(LinkedList_t *list, void *out_value)
{
    if (NULL == list) {
        return STATUS_NULL_PTR;
    }
    
    if (NULL == list->head) {
        return STATUS_UNDERFLOW;
    }
    
    ListNode_t *node = list->head;
    
    if (NULL != out_value) {
        (void)memcpy(out_value, node->data, list->element_size);
    }
    
    list->head = node->next;
    
    if (NULL == list->head) {
        list->tail = NULL;
    }
    
    free_node(list, node);
    list->size--;
    
    return STATUS_OK;
}

int32_t linked_list_pop_back(LinkedList_t *list, void *out_value)
{
    if (NULL == list) {
        return STATUS_NULL_PTR;
    }
    
    if (NULL == list->head) {
        return STATUS_UNDERFLOW;
    }
    
    if (list->head == list->tail) {
        if (NULL != out_value) {
            (void)memcpy(out_value, list->head->data, list->element_size);
        }
        free_node(list, list->head);
        list->head = NULL;
        list->tail = NULL;
        list->size = 0U;
        return STATUS_OK;
    }
    
    ListNode_t *prev = list->head;
    while (prev->next != list->tail) {
        prev = prev->next;
    }
    
    if (NULL != out_value) {
        (void)memcpy(out_value, list->tail->data, list->element_size);
    }
    
    free_node(list, list->tail);
    prev->next = NULL;
    list->tail = prev;
    list->size--;
    
    return STATUS_OK;
}

int32_t linked_list_front(const LinkedList_t *list, void *out_value)
{
    if ((NULL == list) || (NULL == out_value)) {
        return STATUS_NULL_PTR;
    }
    
    if (NULL == list->head) {
        return STATUS_UNDERFLOW;
    }
    
    (void)memcpy(out_value, list->head->data, list->element_size);
    
    return STATUS_OK;
}

int32_t linked_list_back(const LinkedList_t *list, void *out_value)
{
    if ((NULL == list) || (NULL == out_value)) {
        return STATUS_NULL_PTR;
    }
    
    if (NULL == list->tail) {
        return STATUS_UNDERFLOW;
    }
    
    (void)memcpy(out_value, list->tail->data, list->element_size);
    
    return STATUS_OK;
}

int32_t linked_list_insert_at(LinkedList_t *list, 
                              size_t index, 
                              const void *value)
{
    if ((NULL == list) || (NULL == value)) {
        return STATUS_NULL_PTR;
    }
    
    if (index > list->size) {
        return STATUS_INVALID_PARAM;
    }
    
    if (0U == index) {
        return linked_list_push_front(list, value);
    }
    
    if (index == list->size) {
        return linked_list_push_back(list, value);
    }
    
    ListNode_t *node = allocate_node(list);
    if (NULL == node) {
        return STATUS_OVERFLOW;
    }
    
    (void)memcpy(node->data, value, list->element_size);
    
    ListNode_t *prev = list->head;
    for (size_t i = 1U; i < index; i++) {
        prev = prev->next;
    }
    
    node->next = prev->next;
    prev->next = node;
    
    list->size++;
    
    return STATUS_OK;
}

int32_t linked_list_remove_at(LinkedList_t *list, 
                              size_t index, 
                              void *out_value)
{
    if (NULL == list) {
        return STATUS_NULL_PTR;
    }
    
    if (index >= list->size) {
        return STATUS_INVALID_PARAM;
    }
    
    if (0U == index) {
        return linked_list_pop_front(list, out_value);
    }
    
    if (index == (list->size - 1U)) {
        return linked_list_pop_back(list, out_value);
    }
    
    ListNode_t *prev = list->head;
    for (size_t i = 1U; i < index; i++) {
        prev = prev->next;
    }
    
    ListNode_t *node = prev->next;
    
    if (NULL != out_value) {
        (void)memcpy(out_value, node->data, list->element_size);
    }
    
    prev->next = node->next;
    free_node(list, node);
    list->size--;
    
    return STATUS_OK;
}

int32_t linked_list_remove(LinkedList_t *list, 
                           const void *value, 
                           CompareFunc_t compare,
                           void *out_value)
{
    if ((NULL == list) || (NULL == value) || (NULL == compare)) {
        return STATUS_NULL_PTR;
    }
    
    if (NULL == list->head) {
        return STATUS_NOT_FOUND;
    }
    
    if (0 == compare(list->head->data, value)) {
        return linked_list_pop_front(list, out_value);
    }
    
    ListNode_t *prev = list->head;
    ListNode_t *curr = list->head->next;
    
    while (NULL != curr) {
        if (0 == compare(curr->data, value)) {
            if (NULL != out_value) {
                (void)memcpy(out_value, curr->data, list->element_size);
            }
            
            prev->next = curr->next;
            
            if (curr == list->tail) {
                list->tail = prev;
            }
            
            free_node(list, curr);
            list->size--;
            
            return STATUS_OK;
        }
        
        prev = curr;
        curr = curr->next;
    }
    
    return STATUS_NOT_FOUND;
}

size_t linked_list_find(const LinkedList_t *list, 
                        const void *value, 
                        CompareFunc_t compare)
{
    if ((NULL == list) || (NULL == value) || (NULL == compare)) {
        return SIZE_MAX;
    }
    
    ListNode_t *curr = list->head;
    size_t index = 0U;
    
    while (NULL != curr) {
        if (0 == compare(curr->data, value)) {
            return index;
        }
        curr = curr->next;
        index++;
    }
    
    return SIZE_MAX;
}

int32_t linked_list_get(const LinkedList_t *list, 
                        size_t index, 
                        void *out_value)
{
    if ((NULL == list) || (NULL == out_value)) {
        return STATUS_NULL_PTR;
    }
    
    if (index >= list->size) {
        return STATUS_INVALID_PARAM;
    }
    
    ListNode_t *curr = list->head;
    for (size_t i = 0U; i < index; i++) {
        curr = curr->next;
    }
    
    (void)memcpy(out_value, curr->data, list->element_size);
    
    return STATUS_OK;
}

int32_t linked_list_clear(LinkedList_t *list)
{
    if (NULL == list) {
        return STATUS_NULL_PTR;
    }
    
    list->head = NULL;
    list->tail = NULL;
    list->size = 0U;
    
    for (size_t i = 0U; i < list->capacity; i++) {
        list->node_pool[i].next = (i < (list->capacity - 1U)) ? 
                                   &list->node_pool[i + 1U] : NULL;
    }
    list->free_list = &list->node_pool[0];
    
    return STATUS_OK;
}

int32_t linked_list_foreach(LinkedList_t *list, 
                            bool (*callback)(void *element, void *user_data),
                            void *user_data)
{
    if ((NULL == list) || (NULL == callback)) {
        return STATUS_NULL_PTR;
    }
    
    ListNode_t *curr = list->head;
    
    while (NULL != curr) {
        if (!callback(curr->data, user_data)) {
            break;
        }
        curr = curr->next;
    }
    
    return STATUS_OK;
}

int32_t linked_list_reverse(LinkedList_t *list)
{
    if (NULL == list) {
        return STATUS_NULL_PTR;
    }
    
    ListNode_t *prev = NULL;
    ListNode_t *curr = list->head;
    ListNode_t *next = NULL;
    
    list->tail = list->head;
    
    while (NULL != curr) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    
    list->head = prev;
    
    return STATUS_OK;
}
