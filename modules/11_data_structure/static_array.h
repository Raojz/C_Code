/**
 * @file static_array.h
 * @brief 静态数组数据结构头文件
 * @details 实现带边界检查的静态数组，支持安全访问。
 *          专为嵌入式环境设计，使用静态内存分配，避免动态内存分配。
 * 
 * @section features 主要特性
 * - 编译时确定容量，无动态内存分配
 * - 所有访问操作都有边界检查
 * - 支持随机访问，时间复杂度O(1)
 * - 支持元素查找、插入、删除操作
 * - 线程安全考虑（需外部加锁）
 * 
 * @section usage 使用示例
 * @code
 * // 定义一个容量为10的整型数组
 * STATIC_ARRAY_DECLARE(int_array, int32_t, 10)
 * 
 * // 初始化
 * int_array_t arr;
 * static_array_init(&arr);
 * 
 * // 添加元素
 * static_array_push_back(&arr, 100);
 * static_array_push_back(&arr, 200);
 * 
 * // 访问元素
 * int32_t value;
 * if (SUCCESS(static_array_get(&arr, 0, &value))) {
 *     printf("arr[0] = %d\n", value);
 * }
 * @endcode
 * 
 * @section complexity 时间复杂度
 * - 随机访问: O(1)
 * - 尾部插入: O(1)
 * - 指定位置插入: O(n)
 * - 查找元素: O(n)
 * - 删除元素: O(n)
 * 
 * @author Embedded C Learning Project
 * @date 2026-04-10
 * @version 1.0.0
 * 
 * @note 遵循 MISRA-C:2012 编码规范
 * @note 所有函数都有返回值检查
 * @warning 非线程安全，多线程环境需外部加锁
 */

#ifndef STATIC_ARRAY_H
#define STATIC_ARRAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common/common_types.h"

/*============================================================================*/
/*                           宏定义                                             */
/*============================================================================*/

/**
 * @brief 声明静态数组类型
 * @param name 数组类型名称
 * @param type 元素数据类型
 * @param capacity 数组容量
 * @details 使用此宏在编译时定义静态数组结构体类型
 * @code
 * STATIC_ARRAY_DECLARE(my_array, int32_t, 100)
 * // 展开后生成: typedef struct { ... } my_array_t;
 * @endcode
 */
#define STATIC_ARRAY_DECLARE(name, type, capacity)                          \
    typedef struct {                                                         \
        type       data[capacity];    /**< 数据存储区 */                      \
        size_t     size;              /**< 当前元素个数 */                    \
        size_t     capacity;          /**< 数组容量 */                        \
    } name##_t

/*============================================================================*/
/*                           通用静态数组结构体                                  */
/*============================================================================*/

/**
 * @brief 通用静态数组结构体（使用void指针存储）
 * @details 用于需要存储任意类型数据的场景，需要用户自行管理类型转换
 * @warning 使用此结构体需要额外传入元素大小
 */
typedef struct {
    uint8_t    *data;         /**< 数据存储区指针 */
    size_t     size;          /**< 当前元素个数 */
    size_t     capacity;      /**< 数组容量 */
    size_t     element_size;  /**< 单个元素大小（字节） */
} StaticArray_t;

/*============================================================================*/
/*                           函数声明                                           */
/*============================================================================*/

/**
 * @brief 初始化通用静态数组
 * @param arr 数组指针（不能为NULL）
 * @param buffer 数据缓冲区指针（不能为NULL）
 * @param capacity 数组容量
 * @param element_size 单个元素大小（字节）
 * @retval STATUS_OK 初始化成功
 * @retval STATUS_NULL_PTR 参数为NULL
 * @retval STATUS_INVALID_PARAM 容量或元素大小为0
 * @note 必须先调用此函数初始化数组，才能使用其他操作
 */
int32_t static_array_init(StaticArray_t *arr, 
                          uint8_t *buffer, 
                          size_t capacity, 
                          size_t element_size);

/**
 * @brief 获取数组当前元素个数
 * @param arr 数组指针（不能为NULL）
 * @return 当前元素个数，失败返回0
 */
size_t static_array_size(const StaticArray_t *arr);

/**
 * @brief 获取数组容量
 * @param arr 数组指针（不能为NULL）
 * @return 数组容量，失败返回0
 */
size_t static_array_capacity(const StaticArray_t *arr);

/**
 * @brief 检查数组是否为空
 * @param arr 数组指针（不能为NULL）
 * @return true表示数组为空，false表示数组非空
 */
bool static_array_is_empty(const StaticArray_t *arr);

/**
 * @brief 检查数组是否已满
 * @param arr 数组指针（不能为NULL）
 * @return true表示数组已满，false表示数组未满
 */
bool static_array_is_full(const StaticArray_t *arr);

/**
 * @brief 安全获取指定位置的元素
 * @param arr 数组指针（不能为NULL）
 * @param index 元素索引（0-based）
 * @param out_value 输出元素值指针（不能为NULL）
 * @retval STATUS_OK 获取成功
 * @retval STATUS_NULL_PTR 参数为NULL
 * @retval STATUS_INVALID_PARAM 索引越界
 * @note 时间复杂度: O(1)
 */
int32_t static_array_get(const StaticArray_t *arr, 
                         size_t index, 
                         void *out_value);

/**
 * @brief 安全设置指定位置的元素
 * @param arr 数组指针（不能为NULL）
 * @param index 元素索引（0-based）
 * @param value 要设置的元素值指针（不能为NULL）
 * @retval STATUS_OK 设置成功
 * @retval STATUS_NULL_PTR 参数为NULL
 * @retval STATUS_INVALID_PARAM 索引越界
 * @note 时间复杂度: O(1)
 */
int32_t static_array_set(StaticArray_t *arr, 
                         size_t index, 
                         const void *value);

/**
 * @brief 在数组尾部添加元素
 * @param arr 数组指针（不能为NULL）
 * @param value 要添加的元素值指针（不能为NULL）
 * @retval STATUS_OK 添加成功
 * @retval STATUS_NULL_PTR 参数为NULL
 * @retval STATUS_OVERFLOW 数组已满
 * @note 时间复杂度: O(1)
 */
int32_t static_array_push_back(StaticArray_t *arr, const void *value);

/**
 * @brief 移除数组尾部元素
 * @param arr 数组指针（不能为NULL）
 * @param out_value 输出被移除的元素值指针（可为NULL）
 * @retval STATUS_OK 移除成功
 * @retval STATUS_NULL_PTR 数组指针为NULL
 * @retval STATUS_UNDERFLOW 数组为空
 * @note 时间复杂度: O(1)
 */
int32_t static_array_pop_back(StaticArray_t *arr, void *out_value);

/**
 * @brief 在指定位置插入元素
 * @param arr 数组指针（不能为NULL）
 * @param index 插入位置索引（0-based）
 * @param value 要插入的元素值指针（不能为NULL）
 * @retval STATUS_OK 插入成功
 * @retval STATUS_NULL_PTR 参数为NULL
 * @retval STATUS_INVALID_PARAM 索引越界（index > size）
 * @retval STATUS_OVERFLOW 数组已满
 * @note 插入位置及之后的元素会后移，时间复杂度: O(n)
 */
int32_t static_array_insert(StaticArray_t *arr, 
                            size_t index, 
                            const void *value);

/**
 * @brief 移除指定位置的元素
 * @param arr 数组指针（不能为NULL）
 * @param index 要移除的元素索引（0-based）
 * @param out_value 输出被移除的元素值指针（可为NULL）
 * @retval STATUS_OK 移除成功
 * @retval STATUS_NULL_PTR 数组指针为NULL
 * @retval STATUS_INVALID_PARAM 索引越界
 * @note 移除位置之后的元素会前移，时间复杂度: O(n)
 */
int32_t static_array_remove(StaticArray_t *arr, 
                            size_t index, 
                            void *out_value);

/**
 * @brief 查找元素第一次出现的位置
 * @param arr 数组指针（不能为NULL）
 * @param value 要查找的元素值指针（不能为NULL）
 * @param compare 比较函数指针（不能为NULL）
 * @return 元素索引，未找到返回SIZE_MAX
 * @note 时间复杂度: O(n)
 */
size_t static_array_find(const StaticArray_t *arr, 
                         const void *value, 
                         CompareFunc_t compare);

/**
 * @brief 清空数组
 * @param arr 数组指针（不能为NULL）
 * @retval STATUS_OK 清空成功
 * @retval STATUS_NULL_PTR 参数为NULL
 * @note 仅将size置0，不释放内存（因为是静态分配）
 */
int32_t static_array_clear(StaticArray_t *arr);

/**
 * @brief 遍历数组并调用回调函数
 * @param arr 数组指针（不能为NULL）
 * @param callback 回调函数指针（不能为NULL）
 * @param user_data 用户数据指针（可传NULL）
 * @retval STATUS_OK 遍历完成
 * @retval STATUS_NULL_PTR 参数为NULL
 * @note 回调函数返回true继续遍历，返回false停止遍历
 */
int32_t static_array_foreach(StaticArray_t *arr, 
                             bool (*callback)(void *element, void *user_data),
                             void *user_data);

#ifdef __cplusplus
}
#endif

#endif
