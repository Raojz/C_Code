/**
 * @file stack.h
 * @brief 栈数据结构头文件
 * @details 基于数组实现的栈（LIFO - 后进先出），支持静态内存分配。
 *          专为嵌入式环境设计，避免动态内存分配。
 * 
 * @section features 主要特性
 * - 编译时确定容量，无动态内存分配
 * - 支持压栈（push）、弹栈（pop）、查看栈顶（peek）操作
 * - 支持栈空、栈满判断
 * - 所有操作时间复杂度均为O(1)
 * - 支持获取栈中元素个数
 * 
 * @section applications 典型应用场景
 * - 函数调用栈管理
 * - 表达式求值（中缀转后缀）
 * - 括号匹配检查
 * - 撤销/重做功能
 * - 深度优先搜索（DFS）
 * - 中断嵌套管理
 * 
 * @section usage 使用示例
 * @code
 * // 定义一个容量为32的整型栈
 * STACK_DECLARE(int_stack, int32_t, 32)
 * 
 * // 初始化
 * int_stack_t stack;
 * stack_init(&stack);
 * 
 * // 压栈
 * stack_push(&stack, 100);
 * stack_push(&stack, 200);
 * 
 * // 查看栈顶
 * int32_t top;
 * if (SUCCESS(stack_peek(&stack, &top))) {
 *     printf("栈顶元素: %d\n", top);  // 输出: 200
 * }
 * 
 * // 弹栈
 * stack_pop(&stack, &top);  // top = 200
 * stack_pop(&stack, &top);  // top = 100
 * @endcode
 * 
 * @section complexity 时间复杂度
 * - 压栈(push): O(1)
 * - 弹栈(pop): O(1)
 * - 查看栈顶(peek): O(1)
 * - 判断空/满: O(1)
 * 
 * @author Embedded C Learning Project
 * @date 2026-04-10
 * @version 1.0.0
 * 
 * @note 遵循 MISRA-C:2012 编码规范
 * @warning 非线程安全，多线程环境需外部加锁
 */

#ifndef STACK_H
#define STACK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common/common_types.h"

/*============================================================================*/
/*                           宏定义                                             */
/*============================================================================*/

/**
 * @brief 声明栈类型
 * @param name 栈类型名称
 * @param type 元素数据类型
 * @param capacity 栈容量
 * @details 使用此宏在编译时定义静态栈结构体类型
 * @code
 * STACK_DECLARE(my_stack, int32_t, 100)
 * // 展开后生成: typedef struct { ... } my_stack_t;
 * @endcode
 */
#define STACK_DECLARE(name, type, capacity)                                  \
    typedef struct {                                                         \
        type       data[capacity];    /**< 数据存储区 */                      \
        size_t     top;               /**< 栈顶指针（下一个可写入位置） */     \
        size_t     capacity;          /**< 栈容量 */                          \
    } name##_t

/*============================================================================*/
/*                           通用栈结构体                                       */
/*============================================================================*/

/**
 * @brief 通用栈结构体（使用void指针存储）
 * @details 用于需要存储任意类型数据的场景
 * @warning 使用此结构体需要额外传入元素大小
 */
typedef struct {
    uint8_t    *data;         /**< 数据存储区指针 */
    size_t     top;           /**< 栈顶指针（下一个可写入位置） */
    size_t     capacity;      /**< 栈容量 */
    size_t     element_size;  /**< 单个元素大小（字节） */
} Stack_t;

/*============================================================================*/
/*                           函数声明                                           */
/*============================================================================*/

/**
 * @brief 初始化通用栈
 * @param stack 栈指针（不能为NULL）
 * @param buffer 数据缓冲区指针（不能为NULL）
 * @param capacity 栈容量
 * @param element_size 单个元素大小（字节）
 * @retval STATUS_OK 初始化成功
 * @retval STATUS_NULL_PTR 参数为NULL
 * @retval STATUS_INVALID_PARAM 容量或元素大小为0
 * @note 必须先调用此函数初始化栈，才能使用其他操作
 */
int32_t stack_init(Stack_t *stack, 
                   uint8_t *buffer, 
                   size_t capacity, 
                   size_t element_size);

/**
 * @brief 获取栈中元素个数
 * @param stack 栈指针（不能为NULL）
 * @return 栈中元素个数，失败返回0
 */
size_t stack_size(const Stack_t *stack);

/**
 * @brief 获取栈容量
 * @param stack 栈指针（不能为NULL）
 * @return 栈容量，失败返回0
 */
size_t stack_capacity(const Stack_t *stack);

/**
 * @brief 检查栈是否为空
 * @param stack 栈指针（不能为NULL）
 * @return true表示栈为空，false表示栈非空
 */
bool stack_is_empty(const Stack_t *stack);

/**
 * @brief 检查栈是否已满
 * @param stack 栈指针（不能为NULL）
 * @return true表示栈已满，false表示栈未满
 */
bool stack_is_full(const Stack_t *stack);

/**
 * @brief 压栈操作
 * @param stack 栈指针（不能为NULL）
 * @param value 要压入的元素值指针（不能为NULL）
 * @retval STATUS_OK 压栈成功
 * @retval STATUS_NULL_PTR 参数为NULL
 * @retval STATUS_OVERFLOW 栈已满
 * @note 时间复杂度: O(1)
 */
int32_t stack_push(Stack_t *stack, const void *value);

/**
 * @brief 弹栈操作
 * @param stack 栈指针（不能为NULL）
 * @param out_value 输出被弹出的元素值指针（可为NULL）
 * @retval STATUS_OK 弹栈成功
 * @retval STATUS_NULL_PTR 栈指针为NULL
 * @retval STATUS_UNDERFLOW 栈为空
 * @note 时间复杂度: O(1)
 */
int32_t stack_pop(Stack_t *stack, void *out_value);

/**
 * @brief 查看栈顶元素（不弹出）
 * @param stack 栈指针（不能为NULL）
 * @param out_value 输出栈顶元素值指针（不能为NULL）
 * @retval STATUS_OK 获取成功
 * @retval STATUS_NULL_PTR 参数为NULL
 * @retval STATUS_UNDERFLOW 栈为空
 * @note 时间复杂度: O(1)
 */
int32_t stack_peek(const Stack_t *stack, void *out_value);

/**
 * @brief 清空栈
 * @param stack 栈指针（不能为NULL）
 * @retval STATUS_OK 清空成功
 * @retval STATUS_NULL_PTR 参数为NULL
 * @note 仅将top置0，不释放内存
 */
int32_t stack_clear(Stack_t *stack);

/**
 * @brief 遍历栈（从栈顶到栈底）
 * @param stack 栈指针（不能为NULL）
 * @param callback 回调函数指针（不能为NULL）
 * @param user_data 用户数据指针（可传NULL）
 * @retval STATUS_OK 遍历完成
 * @retval STATUS_NULL_PTR 参数为NULL
 * @note 回调函数返回true继续遍历，返回false停止遍历
 */
int32_t stack_foreach(Stack_t *stack, 
                      bool (*callback)(void *element, void *user_data),
                      void *user_data);

#ifdef __cplusplus
}
#endif

#endif
