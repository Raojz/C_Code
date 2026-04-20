/**
 * @file doubly_linked_list.c
 * @brief 双向链表实现
 */

#include "doubly_linked_list.h"
#include <string.h>

int32_t doubly_list_init(DoublyLinkedList_t *list)
{
    if (list == NULL) {
        return STATUS_NULL_PTR;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return STATUS_OK;
}

int32_t doubly_list_pool_init(DoublyListNodePool_t *pool,
                               DoublyListNode_t *nodes,
                               size_t capacity)
{
    if (pool == NULL || nodes == NULL) {
        return STATUS_NULL_PTR;
    }

    pool->nodes = nodes;
    pool->capacity = capacity;
    pool->used = 0;

    memset(nodes, 0, capacity * sizeof(DoublyListNode_t));

    return STATUS_OK;
}

DoublyListNode_t* doubly_list_alloc_node(DoublyListNodePool_t *pool)
{
    if (pool == NULL || pool->used >= pool->capacity) {
        return NULL;
    }

    DoublyListNode_t *node = &pool->nodes[pool->used];
    pool->used++;

    node->prev = NULL;
    node->next = NULL;
    node->data = NULL;

    return node;
}

void doubly_list_free_node(DoublyListNodePool_t *pool, DoublyListNode_t *node)
{
    if (pool == NULL || node == NULL) {
        return;
    }

    node->prev = NULL;
    node->next = NULL;
    node->data = NULL;
}

int32_t doubly_list_insert_head(DoublyLinkedList_t *list, DoublyListNode_t *node)
{
    if (list == NULL || node == NULL) {
        return STATUS_NULL_PTR;
    }

    node->prev = NULL;
    node->next = list->head;

    if (list->head != NULL) {
        list->head->prev = node;
    } else {
        list->tail = node;
    }

    list->head = node;
    list->size++;

    return STATUS_OK;
}

int32_t doubly_list_insert_tail(DoublyLinkedList_t *list, DoublyListNode_t *node)
{
    if (list == NULL || node == NULL) {
        return STATUS_NULL_PTR;
    }

    node->prev = list->tail;
    node->next = NULL;

    if (list->tail != NULL) {
        list->tail->next = node;
    } else {
        list->head = node;
    }

    list->tail = node;
    list->size++;

    return STATUS_OK;
}

int32_t doubly_list_insert_after(DoublyLinkedList_t *list,
                                  DoublyListNode_t *after,
                                  DoublyListNode_t *node)
{
    if (list == NULL || after == NULL || node == NULL) {
        return STATUS_NULL_PTR;
    }

    node->prev = after;
    node->next = after->next;

    if (after->next != NULL) {
        after->next->prev = node;
    } else {
        list->tail = node;
    }

    after->next = node;
    list->size++;

    return STATUS_OK;
}

int32_t doubly_list_remove(DoublyLinkedList_t *list, DoublyListNode_t *node)
{
    if (list == NULL || node == NULL) {
        return STATUS_NULL_PTR;
    }

    if (node->prev != NULL) {
        node->prev->next = node->next;
    } else {
        list->head = node->next;
    }

    if (node->next != NULL) {
        node->next->prev = node->prev;
    } else {
        list->tail = node->prev;
    }

    node->prev = NULL;
    node->next = NULL;
    list->size--;

    return STATUS_OK;
}

size_t doubly_list_size(const DoublyLinkedList_t *list)
{
    if (list == NULL) {
        return 0U;
    }
    return list->size;
}

bool doubly_list_is_empty(const DoublyLinkedList_t *list)
{
    if (list == NULL) {
        return true;
    }
    return (list->size == 0U);
}

void doubly_list_foreach_forward(DoublyLinkedList_t *list,
                                  void (*callback)(void *data, void *user_data),
                                  void *user_data)
{
    if (list == NULL || callback == NULL) {
        return;
    }

    DoublyListNode_t *node = list->head;
    while (node != NULL) {
        callback(node->data, user_data);
        node = node->next;
    }
}

void doubly_list_foreach_backward(DoublyLinkedList_t *list,
                                   void (*callback)(void *data, void *user_data),
                                   void *user_data)
{
    if (list == NULL || callback == NULL) {
        return;
    }

    DoublyListNode_t *node = list->tail;
    while (node != NULL) {
        callback(node->data, user_data);
        node = node->prev;
    }
}
