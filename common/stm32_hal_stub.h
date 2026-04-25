/**
 * @file stm32_hal_stub.h
 * @brief STM32 HAL 库类型存根定义（用于 Windows 学习环境）
 * @details 在 Windows/PC 环境下学习嵌入式 C 语言时，
 *          无法访问真实的 STM32 HAL 库头文件。
 *          本文件提供了 HAL 库核心类型的简化定义和函数声明，
 *          使串口通信驱动代码可以在 Windows 上编译通过语法检查。
 *
 * @section usage 使用说明
 * - 本文件仅供 **Windows 学习环境** 使用
 * - 实际的 STM32 硬件开发应使用 CubeMX 生成的真实 HAL 库
 * - 存根定义仅包含类型声明和函数原型，不包含功能实现
 *
 * @section stub_types 提供的类型
 * - I2C_HandleTypeDef: I2C 外设句柄
 * - UART_HandleTypeDef: UART/USART 外设句柄
 * - SPI_HandleTypeDef: SPI 外设句柄
 * - GPIO_TypeDef: GPIO 端口寄存器结构
 * - GPIO_InitTypeDef: GPIO 初始化结构体
 * - HAL_StatusTypeDef: HAL 库状态枚举
 *
 * @warning 本文件中的类型定义与真实 HAL 库保持兼容，
 *          但成员字段已被简化。切勿在真实硬件上使用此存根！
 *
 * @author Embedded C Learning Project
 * @date 2026-04-21
 * @version 1.1.0
 */

#ifndef STM32_HAL_STUB_H
#define STM32_HAL_STUB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/*============================================================================*/
/*                           条件编译检测                                       */
/*============================================================================*/

/**
 * @brief 检测是否已包含真实的 HAL 库
 * @note 如果已经包含了 stm32f4xx_hal.h，则跳过所有存根定义
 */
#if !defined(__STM32F4xx_HAL_H)

/*============================================================================*/
/*                           HAL 状态码枚举                                     */
/*============================================================================*/

/**
 * @brief HAL 库操作状态码
 * @details 与真实 HAL 库的定义完全一致，确保代码兼容性
 */
typedef enum {
    HAL_OK       = 0x00U,  /**< 操作成功 */
    HAL_ERROR    = 0x01U,  /**< 未定义错误 */
    HAL_BUSY     = 0x02U,  /**< 外设忙 */
    HAL_TIMEOUT  = 0x03U   /**< 操作超时 */
} HAL_StatusTypeDef;

/*============================================================================*/
/*                           GPIO 类型定义                                      */
/*============================================================================*/

/**
 * @brief GPIO 引脚编号宏（常用引脚）
 * @note 与 STM32CubeMX 生成的定义一致
 */
#define GPIO_PIN_0                ((uint16_t)0x0001)
#define GPIO_PIN_1                ((uint16_t)0x0002)
#define GPIO_PIN_2                ((uint16_t)0x0004)
#define GPIO_PIN_3                ((uint16_t)0x0008)
#define GPIO_PIN_4                ((uint16_t)0x0010)
#define GPIO_PIN_5                ((uint16_t)0x0020)
#define GPIO_PIN_6                ((uint16_t)0x0040)
#define GPIO_PIN_7                ((uint16_t)0x0080)
#define GPIO_PIN_8                ((uint16_t)0x0100)
#define GPIO_PIN_9                ((uint16_t)0x0200)
#define GPIO_PIN_10               ((uint16_t)0x0400)
#define GPIO_PIN_11               ((uint16_t)0x0800)
#define GPIO_PIN_12               ((uint16_t)0x1000)
#define GPIO_PIN_13               ((uint16_t)0x2000)
#define GPIO_PIN_14               ((uint16_t)0x4000)
#define GPIO_PIN_15               ((uint16_t)0x8000)
#define GPIO_PIN_ALL              ((uint16_t)0xFFFF)

/**
 * @brief GPIO 引脚电平宏
 */
#define GPIO_PIN_RESET             ((uint8_t)0x00U)
#define GPIO_PIN_SET               ((uint8_t)0x01U)

/**
 * @brief GPIO 模式定义
 */
#define GPIO_MODE_INPUT           ((uint32_t)0x00000000U)
#define GPIO_MODE_OUTPUT_PP       ((uint32_t)0x00000001U)
#define GPIO_MODE_OUTPUT_OD       ((uint32_t)0x00000011U)
#define GPIO_MODE_AF_PP           ((uint32_t)0x00000002U)
#define GPIO_MODE_AF_OD           ((uint32_t)0x00000012U)
#define GPIO_MODE_ANALOG          ((uint32_t)0x00000003U)
#define GPIO_MODE_IT_RISING       ((uint32_t)0x10110000U)
#define GPIO_MODE_IT_FALLING      ((uint32_t)0x10210000U)

/**
 * @brief GPIO 上拉/下拉配置
 */
#define GPIO_NOPULL               ((uint32_t)0x00000000U)
#define GPIO_PULLUP               ((uint32_t)0x00000001U)
#define GPIO_PULLDOWN             ((uint32_t)0x00000002U)

/**
 * @brief GPIO 速度定义
 */
#define GPIO_SPEED_FREQ_LOW       ((uint32_t)0x00000000U)
#define GPIO_SPEED_FREQ_MEDIUM    ((uint32_t)0x00000001U)
#define GPIO_SPEED_FREQ_HIGH      ((uint32_t)0x00000002U)
#define GPIO_SPEED_FREQ_VERY_HIGH ((uint32_t)0x00000003U)

/**
 * @brief GPIO 端口寄存器结构（简化版）
 * @note 真实定义包含 MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR, LCKR, AFR
 */
typedef struct {
    volatile uint32_t MODER;    /**< 模式寄存器 */
    volatile uint32_t OTYPER;   /**< 输出类型寄存器 */
    volatile uint32_t OSPEEDR;  /**< 输出速度寄存器 */
    volatile uint32_t PUPDR;    /**< 上拉/下拉寄存器 */
    volatile uint32_t IDR;      /**< 输入数据寄存器 */
    volatile uint32_t ODR;      /**< 输出数据寄存器 */
    volatile uint32_t BSRR;     /**< 位设置/清除寄存器 */
    volatile uint32_t LCKR;     /**< 配置锁存寄存器 */
    volatile uint32_t AFR[2U];  /**< 复用功能选择寄存器 */
} GPIO_TypeDef;

/**
 * @brief GPIO 初始化结构体
 * @note 用于配置 GPIO 引脚的模式、速度、上下拉等参数
 */
typedef struct {
    uint32_t Pin;        /**< 要配置的引脚号 (GPIO_PIN_x) */
    uint32_t Mode;       /**< 工作模式 (输入/输出/复用/中断) */
    uint32_t Pull;       /**< 上拉/下拉配置 */
    uint32_t Speed;      /**< 输出速度 */
    uint32_t Alternate;  /**< 复用功能编号 */
} GPIO_InitTypeDef;

/*============================================================================*/
/*                           I2C 类型和宏定义                                   */
/*============================================================================*/

/**
 * @brief I2C 时钟占空比宏
 */
#define I2C_DUTYCYCLE_2          ((uint32_t)0x00000000U)
#define I2C_DUTYCYCLE_16_9       ((uint32_t)0x40000000U)

/**
 * @brief I2C 地址模式宏
 */
#define I2C_ADDRESSINGMODE_7BIT  ((uint16_t)0x0400U)
#define I2C_ADDRESSINGMODE_10BIT ((uint16_t)0xC000U)

/**
 * @brief I2C 双地址模式宏
 */
#define I2C_DUALADDRESS_DISABLE  ((uint16_t)0x0000U)
#define I2C_DUALADDRESS_ENABLE   ((uint16_t)0x0400U)

/**
 * @brief I2C 广播呼叫模式宏
 */
#define I2C_GENERALCALL_DISABLE  ((uint16_t)0x0000U)
#define I2C_GENERALCALL_ENABLE   ((uint16_t)0x0010U)

/**
 * @brief I2C 时钟拉伸模式宏
 */
#define I2C_NOSTRETCH_DISABLE    ((uint16_t)0x0000U)
#define I2C_NOSTRETCH_ENABLE     ((uint16_t)0x8000U)

/**
 * @brief I2C 存储器地址大小宏
 */
#define I2C_MEMADD_SIZE_8BIT     ((uint16_t)0x0001U)
#define I2C_MEMADD_SIZE_16BIT    ((uint16_t)0x0002U)

/**
 * @brief I2C 外设句柄结构（简化版）
 * @note 真实定义包含 Instance, Init, ErrorCode, State, Lock 等
 */
typedef struct __I2C_HandleTypeDef {
    void                *Instance;        /**< 寄存器基地址指针 */
    struct {
        uint32_t ClockSpeed;             /**< SCL 时钟频率 */
        uint32_t DutyCycle;              /**< 时钟占空比 */
        uint32_t OwnAddress1;            /**< 自身地址1 */
        uint16_t AddressingMode;         /**< 地址模式 (7bit/10bit) */
        uint16_t DualAddressMode;        /**< 双地址模式 */
        uint16_t OwnAddress2;            /**< 自身地址2 */
        uint16_t GeneralCallMode;        /**< 广播呼叫模式 */
        uint16_t NoStretchMode;          /**< 时钟拉伸模式 */
    } Init;                              /**< 初始化参数 */
    uint32_t           ErrorCode;         /**< 错误码 */
    volatile uint8_t   State;             /**< 设备状态 */
    void               *Lock;             /**< 锁对象 */
} I2C_HandleTypeDef;

/*============================================================================*/
/*                           UART 类型和宏定义                                 */
/*============================================================================*/

/**
 * @brief UART 数据位长度宏
 */
#define UART_WORDLENGTH_8B         ((uint32_t)0x00000000U)
#define UART_WORDLENGTH_9B         ((uint32_t)0x00001000U)

/**
 * @brief UART 停止位宏
 */
#define UART_STOPBITS_1            ((uint32_t)0x00000000U)
#define UART_STOPBITS_2            ((uint32_t)0x00002000U)

/**
 * @brief UART 校验位宏
 */
#define UART_PARITY_NONE           ((uint32_t)0x00000000U)
#define UART_PARITY_EVEN           ((uint32_t)0x00000200U)
#define UART_PARITY_ODD            ((uint32_t)0x00000300U)

/**
 * @brief UART 收发模式宏
 */
#define UART_MODE_RX               ((uint32_t)0x00000004U)
#define UART_MODE_TX               ((uint32_t)0x00000008U)
#define UART_MODE_TX_RX            ((uint32_t)0x0000000CU)

/**
 * @brief UART 硬件流控宏
 */
#define UART_HWCONTROL_NONE        ((uint32_t)0x00000000U)
#define UART_HWCONTROL_RTS         ((uint32_t)0x00000100U)
#define UART_HWCONTROL_CTS         ((uint32_t)0x00000200U)
#define UART_HWCONTROL_RTS_CTS     ((uint32_t)0x00000300U)

/**
 * @brief UART 过采样模式宏
 */
#define UART_OVERSAMPLING_16       ((uint32_t)0x00000000U)
#define UART_OVERSAMPLING_8        ((uint32_t)0x00008000U)

/**
 * @brief UART/USART 外设句柄结构（简化版）
 * @note 真实定义包含 Instance, Init, AdvancedInit, pTxBuffPtr 等
 */
typedef struct __UART_HandleTypeDef {
    void                *Instance;        /**< 寄存器基地址指针 */
    struct {
        uint32_t BaudRate;               /**< 波特率 */
        uint32_t WordLength;             /**< 数据位长度 */
        uint32_t StopBits;               /**< 停止位 */
        uint32_t Parity;                 /**< 校验位 */
        uint32_t Mode;                   /**< 收发模式 */
        uint32_t HwFlowCtl;              /**< 硬件流控 */
        uint32_t OverSampling;           /**< 过采样模式 */
    } Init;                              /**< 初始化参数 */
    uint32_t           ErrorCode;         /**< 错误码 */
    volatile uint8_t   State;             /**< 设备状态 */
    void               *Lock;             /**< 锁对象 */
} UART_HandleTypeDef;

/*============================================================================*/
/*                           SPI 类型和宏定义                                   */
/*============================================================================*/

/**
 * @brief SPI 主从模式宏
 */
#define SPI_MODE_MASTER            ((uint32_t)0x0104U)
#define SPI_MODE_SLAVE             ((uint32_t)0x0000U)

/**
 * @brief SPI 数据方向宏
 */
#define SPI_DIRECTION_2LINES       ((uint32_t)0x00000000U)
#define SPI_DIRECTION_2LINES_RXONLY ((uint32_t)0x0080U)
#define SPI_DIRECTION_1LINE        ((uint32_t)0x0400U)

/**
 * @brief SPI 数据大小宏
 */
#define SPI_DATASIZE_8BIT          ((uint32_t)0x00000000U)
#define SPI_DATASIZE_16BIT         ((uint32_t)0x0800U)

/**
 * @brief SPI 时钟极性宏
 */
#define SPI_POLARITY_LOW           ((uint32_t)0x00000000U)
#define SPI_POLARITY_HIGH          ((uint32_t)0x00020000U)

/**
 * @brief SPI 时钟相位宏
 */
#define SPI_PHASE_1EDGE            ((uint32_t)0x00000000U)
#define SPI_PHASE_2EDGE            ((uint32_t)0x00040000U)

/**
 * @brief SPI 片选管理宏
 */
#define SPI_NSS_SOFT               ((uint32_t)0x0200U)
#define SPI_NSS_HARD_INPUT         ((uint32_t)0x00000000U)
#define SPI_NSS_HARD_OUTPUT        ((uint32_t)0x0400U)

/**
 * @brief SPI 波特率预分频宏
 */
#define SPI_BAUDRATEPRESCALER_2    ((uint32_t)0x00000000U)
#define SPI_BAUDRATEPRESCALER_4    ((uint32_t)0x00080000U)
#define SPI_BAUDRATEPRESCALER_8    ((uint32_t)0x00100000U)
#define SPI_BAUDRATEPRESCALER_16   ((uint32_t)0x00180000U)
#define SPI_BAUDRATEPRESCALER_32   ((uint32_t)0x00200000U)
#define SPI_BAUDRATEPRESCALER_64   ((uint32_t)0x00280000U)
#define SPI_BAUDRATEPRESCALER_128  ((uint32_t)0x00300000U)
#define SPI_BAUDRATEPRESCALER_256  ((uint32_t)0x00380000U)

/**
 * @brief SPI MSB/LSB 优先宏
 */
#define SPI_FIRSTBIT_MSB           ((uint32_t)0x00000000U)
#define SPI_FIRSTBIT_LSB           ((uint32_t)0x00800000U)

/**
 * @brief SPI TI 模式宏
 */
#define SPI_TIMODE_DISABLE         ((uint32_t)0x00000000U)
#define SPI_TIMODE_ENABLE          ((uint32_t)0x00400000U)

/**
 * @brief SPI CRC 计算宏
 */
#define SPI_CRCCALCULATION_DISABLE ((uint32_t)0x00000000U)
#define SPI_CRCCALCULATION_ENABLE  ((uint32_t)0x01000000U)

/**
 * @brief SPI 外设句柄结构（简化版）
 * @note 真实定义包含 Instance, Init, pTxBuffPtr, TxXferSize 等
 */
typedef struct __SPI_HandleTypeDef {
    void                *Instance;        /**< 寄存器基地址指针 */
    struct {
        uint32_t Mode;                    /**< 主从模式 */
        uint32_t Direction;               /**< 数据方向 */
        uint32_t DataSize;                /**< 数据大小 */
        uint32_t CLKPolarity;             /**< 时钟极性 */
        uint32_t CLKPhase;                /**< 时钟相位 */
        uint32_t NSS;                     /**< 片选管理 */
        uint32_t BaudRatePrescaler;       /**< 波特率预分频 */
        uint32_t FirstBit;                /**< MSB/LSB 优先 */
        uint32_t TIMode;                  /**< TI 模式 */
        uint32_t CRCCalculation;          /**< CRC 计算 */
        uint32_t CRCPolynomial;           /**< CRC 多项式 */
    } Init;                              /**< 初始化参数 */
    uint32_t           ErrorCode;         /**< 错误码 */
    volatile uint8_t   State;             /**< 设备状态 */
    void               *Lock;             /**< 锁对象 */
} SPI_HandleTypeDef;

/*============================================================================*/
/*                           HAL 函数声明                                       */
/*============================================================================*/

/**
 * @defgroup HAL_GPIO_Functions GPIO 函数存根
 * @{
 */

/**
 * @brief 初始化 GPIO 引脚
 * @param port GPIO 端口（如 GPIOA）
 * @param gpio_init 初始化参数结构体指针
 * @retval 无返回值
 * @note Stub 版本：仅提供函数声明，无实际实现
 */
void HAL_GPIO_Init(GPIO_TypeDef *port, const GPIO_InitTypeDef *gpio_init);

/**
 * @brief 设置 GPIO 引脚电平
 * @param port GPIO 端口
 * @param pin 引脚编号
 * @param pin_state 电平状态 (GPIO_PIN_SET / GPIO_PIN_RESET)
 * @retval 无返回值
 */
void HAL_GPIO_WritePin(GPIO_TypeDef *port, uint16_t pin, uint8_t pin_state);

/** @} */

/**
 * @defgroup HAL_I2C_Functions I2C 函数存根
 * @{
 */

/**
 * @brief 初始化 I2C 外设
 * @param hi2c I2C 句柄指针
 * @retval HAL_OK 成功 / 其他值 失败
 */
HAL_StatusTypeDef HAL_I2C_Init(I2C_HandleTypeDef *hi2c);

/**
 * @brief I2C 主机发送数据（阻塞方式）
 * @param hi2c I2C 句柄
 * @param dev_addr 设备地址（已左移）
 * @param data 发送数据缓冲区
 * @param size 数据长度
 * @param timeout 超时时间
 * @retval HAL_StatusTypeDef 状态码
 */
HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c,
                                           uint16_t dev_addr,
                                           uint8_t *data,
                                           uint16_t size,
                                           uint32_t timeout);

/**
 * @brief I2C 主机接收数据（阻塞方式）
 * @param hi2c I2C 句柄
 * @param dev_addr 设备地址（已左移）
 * @param data 接收数据缓冲区
 * @param size 数据长度
 * @param timeout 超时时间
 * @retval HAL_StatusTypeDef 状态码
 */
HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef *hi2c,
                                          uint16_t dev_addr,
                                          uint8_t *data,
                                          uint16_t size,
                                          uint32_t timeout);

/**
 * @brief I2C 内存读取操作（写后读）
 * @param hi2c I2C 句柄
 * @param dev_addr 设备地址
 * @param mem_addr 内存地址（寄存器地址）
 * @param mem_addr_size 地址宽度 (I2C_MEMADD_SIZE_8BIT / 16BIT)
 * @param data 接收数据缓冲区
 * @param size 数据长度
 * @param timeout 超时时间
 * @retval HAL_StatusTypeDef 状态码
 */
HAL_StatusTypeDef HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c,
                                    uint16_t dev_addr,
                                    uint16_t mem_addr,
                                    uint16_t mem_addr_size,
                                    uint8_t *data,
                                    uint16_t size,
                                    uint32_t timeout);

/**
 * @brief I2C 内存写入操作
 * @param hi2c I2C 句柄
 * @param dev_addr 设备地址
 * @param mem_addr 内存地址
 * @param mem_addr_size 地址宽度
 * @param data 发送数据缓冲区
 * @param size 数据长度
 * @param timeout 超时时间
 * @retval HAL_StatusTypeDef 状态码
 */
HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c,
                                     uint16_t dev_addr,
                                     uint16_t mem_addr,
                                     uint16_t mem_addr_size,
                                     const uint8_t *data,
                                     uint16_t size,
                                     uint32_t timeout);

/**
 * @brief 检查 I2C 设备是否就绪
 * @param hi2c I2C 句柄
 * @param dev_addr 设备地址
 * @param trials 重试次数
 * @param timeout 超时时间
 * @retval HAL_StatusTypeDef 状态码
 */
HAL_StatusTypeDef HAL_I2C_IsDeviceReady(I2C_HandleTypeDef *hi2c,
                                         uint16_t dev_addr,
                                         uint32_t trials,
                                         uint32_t timeout);

/** @} */

/**
 * @defgroup HAL_UART_Functions UART 函数存根
 * @{
 */

/**
 * @brief 初始化 UART 外设
 * @param huart UART 句柄指针
 * @retval HAL_OK 成功 / 其他值 失败
 */
HAL_StatusTypeDef HAL_UART_Init(UART_HandleTypeDef *huart);

/**
 * @brief UART 阻塞方式发送数据
 * @param huart UART 句柄
 * @param data 发送数据缓冲区
 * @param size 数据长度
 * @param timeout 超时时间
 * @retval HAL_StatusTypeDef 状态码
 */
HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart,
                                     const uint8_t *data,
                                     uint16_t size,
                                     uint32_t timeout);

/**
 * @brief UART 中断方式接收数据
 * @param huart UART 句柄
 * @param data 接收数据缓冲区
 * @param size 数据长度
 * @retval HAL_StatusTypeDef 状态码
 */
HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *huart,
                                       uint8_t *data,
                                       uint16_t size);

/**
 * @brief UART 接收完成回调函数（弱定义）
 * @param huart 触发回调的 UART 句柄
 * @note 用户可重定义此函数以处理接收完成事件
 * @note 在 Windows 环境下此声明为普通函数声明（无弱定义属性）
 */
#if defined(__GNUC__) || defined(__clang__)
    /* GCC/Clang: 支持弱定义属性 */
    __attribute__((weak)) void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
#else
    /* MSVC/其他编译器: 普通函数声明 */
    void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
#endif

/** @} */

/**
 * @defgroup HAL_SPI_Functions SPI 函数存根
 * @{
 */

/**
 * @brief 初始化 SPI 外设
 * @param hspi SPI 句柄指针
 * @retval HAL_OK 成功 / 其他值 失败
 */
HAL_StatusTypeDef HAL_SPI_Init(SPI_HandleTypeDef *hspi);

/**
 * @brief SPI 全双工同时发送和接收数据
 * @param hspi SPI 句柄
 * @param tx_data 发送数据缓冲区（可为 NULL）
 * @param rx_data 接收数据缓冲区（可为 NULL）
 * @param size 数据长度
 * @param timeout 超时时间
 * @retval HAL_StatusTypeDef 状态码
 */
HAL_StatusTypeDef HAL_SPI_TransmitReceive(SPI_HandleTypeDef *hspi,
                                          const uint8_t *tx_data,
                                          uint8_t *rx_data,
                                          uint16_t size,
                                          uint32_t timeout);

/**
 * @brief SPI 仅发送数据
 * @param hspi SPI 句柄
 * @param data 发送数据缓冲区
 * @param size 数据长度
 * @param timeout 超时时间
 * @retval HAL_StatusTypeDef 状态码
 */
HAL_StatusTypeDef HAL_SPI_Transmit(SPI_HandleTypeDef *hspi,
                                    const uint8_t *data,
                                    uint16_t size,
                                    uint32_t timeout);

/**
 * @brief SPI 仅接收数据
 * @param hspi SPI 句柄
 * @param data 接收数据缓冲区
 * @param size 数据长度
 * @param timeout 超时时间
 * @retval HAL_StatusTypeDef 状态码
 */
HAL_StatusTypeDef HAL_SPI_Receive(SPI_HandleTypeDef *hspi,
                                   uint8_t *data,
                                   uint16_t size,
                                   uint32_t timeout);

/** @} */

#endif /* !defined(__STM32F4xx_HAL_H) */

#ifdef __cplusplus
}
#endif

#endif /* STM32_HAL_STUB_H */
