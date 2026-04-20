/**
 * @file doubly_linked_list.h
 * @brief 双向链表数据结构头文件
 * @details 双向链表实现，支持双向遍历，适合需要频繁插入删除的场景。
 *
 * @section features 主要特性
 * - 双向遍历：可向前或向后遍历
 * - O(1) 插入删除：在已知节点位置时
 * - 静态内存分配：节点池预分配
 *
 * @author Embedded C Learning Project
 * @date 2026-04-11
 * @version 1.0.0
 */

#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common/common_types.h"

/*============================================================================*/
/*                           结构体定义                                         */
/*============================================================================*/

/**
 * @brief 双向链表节点结构体
 */
typedef struct DoublyListNode {
    struct DoublyListNode *prev;  /**< 前驱节点指针 */
    struct DoublyListNode *next;  /**< 后继节点指针 */
    void                  *data;  /**< 数据指针 */
} DoublyListNode_t;

/**
 * @brief 双向链表结构体
 */
typedef struct {
    DoublyListNode_t *head;       /**< 头节点指针 */
    DoublyListNode_t *tail;       /**< 尾节点指针 */
    size_t            size;       /**< 节点数量 */
} DoublyLinkedList_t;

/**
 * @brief 双向链表节点池（静态分配）
 */
typedef struct {
    DoublyListNode_t *nodes;      /**< 节点数组 */
    size_t            capacity;   /**< 节点池容量 */
    size_t            used;       /**< 已使用数量 */
} DoublyListNodePool_t;

/*============================================================================*/
/*                           函数声明                                           */
/*============================================================================*/

/**
 * @brief 初始化双向链表
 * @param list 链表指针
 * @return STATUS_OK 成功
 */
int32_t doubly_list_init(DoublyLinkedList_t *list);

/**
 * @brief 初始化节点池
 * @param pool 节点池指针
 * @param nodes 节点数组
 * @param capacity 容量
 * @return STATUS_OK 成功
 */
int32_t doubly_list_pool_init(DoublyListNodePool_t *pool,
                               DoublyListNode_t *nodes,
                               size_t capacity);

/**
 * @brief 从节点池分配节点
 * @param pool 节点池指针
 * @return 节点指针，失败返回NULL
 */
DoublyListNode_t* doubly_list_alloc_node(DoublyListNodePool_t *pool);

/**
 * @brief 释放节点到节点池
 * @param pool 节点池指针
 * @param node 节点指针
 */
void doubly_list_free_node(DoublyListNodePool_t *pool, DoublyListNode_t *node);

/**
 * @brief 在头部插入节点
 * @param list 链表指针
 * @param node 节点指针
 * @return STATUS_OK 成功
 */
int32_t doubly_list_insert_head(DoublyLinkedList_t *list, DoublyListNode_t *node);

/**
 * @brief 在尾部插入节点
 * @param list 链表指针
 * @param node 节点指针
 * @return STATUS_OK 成功
 */
int32_t doubly_list_insert_tail(DoublyLinkedList_t *list, DoublyListNode_t *node);

/**
 * @brief 在指定节点后插入
 * @param list 链表指针
 * @param after 目标节点
 * @param node 新节点
 * @return STATUS_OK 成功
 */
int32_t doubly_list_insert_after(DoublyLinkedList_t *list,
                                  DoublyListNode_t *after,
                                  DoublyListNode_t *node);

/**
 * @brief 删除指定节点
 * @param list 链表指针
 * @param node 要删除的节点
 * @return STATUS_OK 成功
 */
int32_t doubly_list_remove(DoublyLinkedList_t *list, DoublyListNode_t *node);

/**
 * @brief 获取链表大小
 * @param list 链表指针
 * @return 节点数量
 */
size_t doubly_list_size(const DoublyLinkedList_t *list);

/**
 * @brief 检查链表是否为空
 * @param list 链表指针
 * @return true 为空
 */
bool doubly_list_is_empty(const DoublyLinkedList_t *list);

/**
 * @brief 正向遍历链表
 * @param list 链表指针
 * @param callback 回调函数
 * @param user_data 用户数据
 */
void doubly_list_foreach_forward(DoublyLinkedList_t *list,
                                  void (*callback)(void *data, void *user_data),
                                  void *user_data);

/**
 * @brief 反向遍历链表
 * @param list 链表指针
 * @param callback 回调函数
 * @param user_data 用户数据
 */
void doubly_list_foreach_backward(DoublyLinkedList_t *list,
                                   void (*callback)(void *data, void *user_data),
                                   void *user_data);

#ifdef __cplusplus
}
#endif

#endif
