/**
 * @file linked_list.h
 * @brief 单向链表数据结构头文件
 * @details 基于静态内存池实现的单向链表，支持静态内存分配。
 *          专为嵌入式环境设计，避免使用malloc/free动态内存分配。
 * 
 * @section features 主要特性
 * - 使用静态内存池管理节点，无动态内存分配
 * - 支持头部插入、尾部插入、指定位置插入
 * - 支持按值删除、按位置删除
 * - 支持遍历、查找操作
 * - 支持获取链表长度
 * - 节点可回收重用
 * 
 * @section applications 典型应用场景
 * - 动态数据集合管理
 * - 内存池管理
 * - 任务链表
 * - 事件处理器链表
 * - 哈希表冲突解决
 * - 多项式运算
 * 
 * @section implementation 实现原理
 * 使用静态内存池实现链表：
 * - 预分配固定数量的节点
 * - 使用free_list管理空闲节点
 * - 分配节点时从free_list取
 * - 释放节点时归还到free_list
 * 
 * @section usage 使用示例
 * @code
 * // 定义一个容量为32的整型链表
 * LINKED_LIST_DECLARE(int_list, int32_t, 32)
 * 
 * // 初始化
 * int_list_t list;
 * linked_list_init(&list);
 * 
 * // 头部插入
 * linked_list_push_front(&list, 100);
 * linked_list_push_front(&list, 200);
 * 
 * // 尾部插入
 * linked_list_push_back(&list, 300);
 * 
 * // 遍历
 * linked_list_foreach(&list, print_element, NULL);
 * 
 * // 删除
 * linked_list_remove(&list, 100, compare_int);
 * 
 * // 清空
 * linked_list_clear(&list);
 * @endcode
 * 
 * @section complexity 时间复杂度
 * - 头部插入: O(1)
 * - 尾部插入: O(n) 或 O(1)（维护尾指针）
 * - 指定位置插入: O(n)
 * - 头部删除: O(1)
 * - 尾部删除: O(n)
 * - 按值删除: O(n)
 * - 查找: O(n)
 * - 获取长度: O(1)
 * 
 * @author Embedded C Learning Project
 * @date 2026-04-10
 * @version 1.0.0
 * 
 * @note 遵循 MISRA-C:2012 编码规范
 * @warning 非线程安全，多线程环境需外部加锁
 */

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common/common_types.h"

/*============================================================================*/
/*                           宏定义                                             */
/*============================================================================*/

/**
 * @brief 声明单向链表类型
 * @param name 链表类型名称
 * @param type 元素数据类型
 * @param capacity 链表容量（最大节点数）
 * @details 使用此宏在编译时定义静态链表结构体类型
 * @code
 * LINKED_LIST_DECLARE(my_list, int32_t, 100)
 * // 展开后生成: typedef struct { ... } my_list_t;
 * @endcode
 */
#define LINKED_LIST_DECLARE(name, type, capacity)                            \
    typedef struct name##_node {                                             \
        type                  data;       /**< 节点数据 */                    \
        struct name##_node   *next;       /**< 下一个节点指针 */              \
    } name##_node_t;                                                         \
                                                                             \
    typedef struct {                                                         \
        name##_node_t         nodes[capacity];  /**< 节点池 */               \
        name##_node_t        *head;             /**< 链表头指针 */            \
        name##_node_t        *tail;             /**< 链表尾指针 */            \
        name##_node_t        *free_list;        /**< 空闲节点链表 */          \
        size_t                size;             /**< 当前元素个数 */          \
        size_t                capacity;         /**< 最大节点数 */            \
    } name##_t

/*============================================================================*/
/*                           通用链表节点结构体                                 */
/*============================================================================*/

/**
 * @brief 通用链表节点结构体
 */
typedef struct ListNode {
    void              *data;         /**< 节点数据指针 */
    struct ListNode   *next;         /**< 下一个节点指针 */
} ListNode_t;

/*============================================================================*/
/*                           通用链表结构体                                     */
/*============================================================================*/

/**
 * @brief 通用链表结构体（使用void指针存储）
 * @details 用于需要存储任意类型数据的场景
 * @warning 使用此结构体需要额外传入元素大小
 */
typedef struct {
    ListNode_t   *node_pool;        /**< 节点池指针 */
    uint8_t      *data_pool;        /**< 数据池指针 */
    ListNode_t   *head;             /**< 链表头指针 */
    ListNode_t   *tail;             /**< 链表尾指针 */
    ListNode_t   *free_list;        /**< 空闲节点链表 */
    size_t        size;             /**< 当前元素个数 */
    size_t        capacity;         /**< 最大节点数 */
    size_t        element_size;     /**< 单个元素大小（字节） */
} LinkedList_t;

/*============================================================================*/
/*                           函数声明                                           */
/*============================================================================*/

/**
 * @brief 初始化通用链表
 * @param list 链表指针（不能为NULL）
 * @param node_pool 节点池缓冲区指针（不能为NULL）
 * @param data_pool 数据池缓冲区指针（不能为NULL）
 * @param capacity 链表容量
 * @param element_size 单个元素大小（字节）
 * @retval STATUS_OK 初始化成功
 * @retval STATUS_NULL_PTR 参数为NULL
 * @retval STATUS_INVALID_PARAM 容量或元素大小为0
 * @note 必须先调用此函数初始化链表，才能使用其他操作
 */
int32_t linked_list_init(LinkedList_t *list, 
                         ListNode_t *node_pool,
                         uint8_t *data_pool,
                         size_t capacity, 
                         size_t element_size);

/**
 * @brief 获取链表中元素个数
 * @param list 链表指针（不能为NULL）
 * @return 链表中元素个数，失败返回0
 */
size_t linked_list_size(const LinkedList_t *list);

/**
 * @brief 获取链表容量
 * @param list 链表指针（不能为NULL）
 * @return 链表容量，失败返回0
 */
size_t linked_list_capacity(const LinkedList_t *list);

/**
 * @brief 检查链表是否为空
 * @param list 链表指针（不能为NULL）
 * @return true表示链表为空，false表示链表非空
 */
bool linked_list_is_empty(const LinkedList_t *list);

/**
 * @brief 检查链表是否已满
 * @param list 链表指针（不能为NULL）
 * @return true表示链表已满，false表示链表未满
 */
bool linked_list_is_full(const LinkedList_t *list);

/**
 * @brief 在链表头部插入元素
 * @param list 链表指针（不能为NULL）
 * @param value 要插入的元素值指针（不能为NULL）
 * @retval STATUS_OK 插入成功
 * @retval STATUS_NULL_PTR 参数为NULL
 * @retval STATUS_OVERFLOW 链表已满
 * @note 时间复杂度: O(1)
 */
int32_t linked_list_push_front(LinkedList_t *list, const void *value);

/**
 * @brief 在链表尾部插入元素
 * @param list 链表指针（不能为NULL）
 * @param value 要插入的元素值指针（不能为NULL）
 * @retval STATUS_OK 插入成功
 * @retval STATUS_NULL_PTR 参数为NULL
 * @retval STATUS_OVERFLOW 链表已满
 * @note 时间复杂度: O(1)（维护尾指针）
 */
int32_t linked_list_push_back(LinkedList_t *list, const void *value);

/**
 * @brief 移除链表头部元素
 * @param list 链表指针（不能为NULL）
 * @param out_value 输出被移除的元素值指针（可为NULL）
 * @retval STATUS_OK 移除成功
 * @retval STATUS_NULL_PTR 链表指针为NULL
 * @retval STATUS_UNDERFLOW 链表为空
 * @note 时间复杂度: O(1)
 */
int32_t linked_list_pop_front(LinkedList_t *list, void *out_value);

/**
 * @brief 移除链表尾部元素
 * @param list 链表指针（不能为NULL）
 * @param out_value 输出被移除的元素值指针（可为NULL）
 * @retval STATUS_OK 移除成功
 * @retval STATUS_NULL_PTR 链表指针为NULL
 * @retval STATUS_UNDERFLOW 链表为空
 * @note 时间复杂度: O(n)
 */
int32_t linked_list_pop_back(LinkedList_t *list, void *out_value);

/**
 * @brief 获取链表头部元素
 * @param list 链表指针（不能为NULL）
 * @param out_value 输出头部元素值指针（不能为NULL）
 * @retval STATUS_OK 获取成功
 * @retval STATUS_NULL_PTR 参数为NULL
 * @retval STATUS_UNDERFLOW 链表为空
 * @note 时间复杂度: O(1)
 */
int32_t linked_list_front(const LinkedList_t *list, void *out_value);

/**
 * @brief 获取链表尾部元素
 * @param list 链表指针（不能为NULL）
 * @param out_value 输出尾部元素值指针（不能为NULL）
 * @retval STATUS_OK 获取成功
 * @retval STATUS_NULL_PTR 参数为NULL
 * @retval STATUS_UNDERFLOW 链表为空
 * @note 时间复杂度: O(1)
 */
int32_t linked_list_back(const LinkedList_t *list, void *out_value);

/**
 * @brief 在指定位置插入元素
 * @param list 链表指针（不能为NULL）
 * @param index 插入位置索引（0-based，0表示头部插入）
 * @param value 要插入的元素值指针（不能为NULL）
 * @retval STATUS_OK 插入成功
 * @retval STATUS_NULL_PTR 参数为NULL
 * @retval STATUS_INVALID_PARAM 索引越界
 * @retval STATUS_OVERFLOW 链表已满
 * @note 时间复杂度: O(n)
 */
int32_t linked_list_insert_at(LinkedList_t *list, 
                              size_t index, 
                              const void *value);

/**
 * @brief 移除指定位置的元素
 * @param list 链表指针（不能为NULL）
 * @param index 要移除的元素索引（0-based）
 * @param out_value 输出被移除的元素值指针（可为NULL）
 * @retval STATUS_OK 移除成功
 * @retval STATUS_NULL_PTR 链表指针为NULL
 * @retval STATUS_INVALID_PARAM 索引越界
 * @note 时间复杂度: O(n)
 */
int32_t linked_list_remove_at(LinkedList_t *list, 
                              size_t index, 
                              void *out_value);

/**
 * @brief 根据值移除第一个匹配的元素
 * @param list 链表指针（不能为NULL）
 * @param value 要移除的元素值指针（不能为NULL）
 * @param compare 比较函数指针（不能为NULL）
 * @param out_value 输出被移除的元素值指针（可为NULL）
 * @retval STATUS_OK 移除成功
 * @retval STATUS_NULL_PTR 参数为NULL
 * @retval STATUS_NOT_FOUND 未找到匹配元素
 * @note 时间复杂度: O(n)
 */
int32_t linked_list_remove(LinkedList_t *list, 
                           const void *value, 
                           CompareFunc_t compare,
                           void *out_value);

/**
 * @brief 查找元素第一次出现的位置
 * @param list 链表指针（不能为NULL）
 * @param value 要查找的元素值指针（不能为NULL）
 * @param compare 比较函数指针（不能为NULL）
 * @return 元素索引，未找到返回SIZE_MAX
 * @note 时间复杂度: O(n)
 */
size_t linked_list_find(const LinkedList_t *list, 
                        const void *value, 
                        CompareFunc_t compare);

/**
 * @brief 获取指定位置的元素
 * @param list 链表指针（不能为NULL）
 * @param index 元素索引（0-based）
 * @param out_value 输出元素值指针（不能为NULL）
 * @retval STATUS_OK 获取成功
 * @retval STATUS_NULL_PTR 参数为NULL
 * @retval STATUS_INVALID_PARAM 索引越界
 * @note 时间复杂度: O(n)
 */
int32_t linked_list_get(const LinkedList_t *list, 
                        size_t index, 
                        void *out_value);

/**
 * @brief 清空链表
 * @param list 链表指针（不能为NULL）
 * @retval STATUS_OK 清空成功
 * @retval STATUS_NULL_PTR 参数为NULL
 * @note 将所有节点归还到空闲链表
 */
int32_t linked_list_clear(LinkedList_t *list);

/**
 * @brief 遍历链表
 * @param list 链表指针（不能为NULL）
 * @param callback 回调函数指针（不能为NULL）
 * @param user_data 用户数据指针（可传NULL）
 * @retval STATUS_OK 遍历完成
 * @retval STATUS_NULL_PTR 参数为NULL
 * @note 回调函数返回true继续遍历，返回false停止遍历
 */
int32_t linked_list_foreach(LinkedList_t *list, 
                            bool (*callback)(void *element, void *user_data),
                            void *user_data);

/**
 * @brief 反转链表
 * @param list 链表指针（不能为NULL）
 * @retval STATUS_OK 反转成功
 * @retval STATUS_NULL_PTR 参数为NULL
 * @note 时间复杂度: O(n)
 */
int32_t linked_list_reverse(LinkedList_t *list);

#ifdef __cplusplus
}
#endif

#endif
