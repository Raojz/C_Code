/**
 * @file serial_uart.h
 * @brief UART (通用异步收发传输器) 通信协议驱动 —— 头文件
 * @details 本模块实现了基于 STM32 HAL 库的 UART 通信驱动，提供完整的初始化配置、
 *          数据发送与接收功能，支持中断接收和环形缓冲区管理。
 *
 * @section uart_protocol UART 协议概述
 *
 * UART (Universal Asynchronous Receiver/Transmitter) 是一种通用的异步串行通信协议，
 * "异步"意味着它不需要额外的时钟线来同步数据传输，收发双方必须事先约定相同的
 * 波特率（Baud Rate）才能正确解码数据。
 *
 * @subsection uart_signals 信号定义
 * UART 仅使用两根信号线：
 * - TX (Transmit): 发送数据线，数据从本端 TX 输出到对端 RX
 * - RX (Receive):  接收数据线，数据从对端 TX 输入到本端 RX
 * 通信双方需交叉连接：本端 TX → 对端 RX，本端 RX ← 对端 TX
 * 此外，还有可选的硬件流控信号：
 * - RTS (Request To Send): 请求发送，本端通知对端可以接收数据
 * - CTS (Clear To Send):   清除发送，对端通知本端可以发送数据
 *
 * @subsection uart_frame 数据帧格式
 * 一个完整的 UART 数据帧由以下部分组成：
 *
 * @code
 *  空闲状态(高电平)
 *  ──────┐   ┌───┌───┌───┬───┬───┬───┬───┬───┬───┐   ┌──────┐   ┌──────
 *        │   │D0 │D1 │D2 │D3 │D4 │D5 │D6 │D7 │P  │   │ 停止  │   │
 *        │起始│   │   │   │   │   │   │   │   │校验│   │ 位   │   │
 *  ──────┘   └───└───└───└───└───└───└───└───└───┘   └──────┘   │
 *        ↑                                                         ↑
 *     起始位(0)                                               空闲(1)或
 *                                                           下一个起始位
 * @endcode
 *
 * 1. 空闲状态 (Idle): 信号线保持高电平（逻辑1），表示无数据传输
 * 2. 起始位 (Start Bit): 固定为1位低电平（逻辑0），标志一帧数据的开始。
 *    接收端检测到下降沿后，开始按约定波特率采样数据位
 * 3. 数据位 (Data Bits): 5~9位有效数据，低位(LSB)先发送。
 *    常用8位（1字节），即数据范围 0x00~0xFF
 * 4. 校验位 (Parity Bit): 可选，用于检测数据传输中的单比特错误
 *    - 无校验 (None): 不使用校验位，节省带宽
 *    - 奇校验 (Odd):  数据位+校验位中"1"的个数为奇数
 *    - 偶校验 (Even): 数据位+校验位中"1"的个数为偶数
 * 5. 停止位 (Stop Bit): 固定为高电平（逻辑1），标志一帧数据结束。
 *    可配置为1位、1.5位或2位，位数越多容错性越强但效率越低
 *
 * @subsection uart_baud 波特率
 * 波特率表示每秒传输的符号数（bit/s）。收发双方必须使用相同的波特率，
 * 否则接收端采样时刻会偏移，导致数据错误。常用波特率：
 * - 9600:    低速，长距离通信，抗干扰能力强
 * - 115200:  高速，短距离通信，嵌入式开发中最常用
 * - 460800:  更高速，需确保时钟精度足够
 *
 * 波特率与传输效率的关系（以8N1格式为例）：
 * - 每帧 = 1起始位 + 8数据位 + 1停止位 = 10位
 * - 115200 baud → 每秒传输 115200/10 = 11520 字节 ≈ 11.25 KB/s
 *
 * @subsection uart_timing 时序特性
 * - 采样策略: 接收端在每位数据的中间位置采样，以获得最大容错裕度
 * - 过采样:   STM32 USART 支持8倍或16倍过采样，提高采样精度
 * - 帧错误:   若停止位不是高电平，说明数据帧同步丢失（噪声/波特率不匹配）
 * - 溢出错误: 接收数据寄存器未被及时读取，新数据覆盖旧数据
 *
 * @section uart_usage 使用方法
 * @code
 * // 1. 初始化 UART
 * UartContext_t uart_ctx;
 * uart_init(&uart_ctx, &huart1, 115200U);
 *
 * // 2. 发送数据
 * uint8_t msg[] = "Hello UART!";
 * uart_send(&uart_ctx, msg, sizeof(msg));
 *
 * // 3. 在主循环中检查接收数据
 * uint8_t rx_byte;
 * while (uart_read_byte(&uart_ctx, &rx_byte) == STATUS_OK) {
 *     process_byte(rx_byte);
 * }
 * @endcode
 *
 * @author Embedded C Learning Project
 * @date 2026-04-21
 * @version 1.0.0
 *
 * @note 遵循 MISRA-C:2012 编码规范
 * @note 所有函数参数均进行空指针检查
 * @note 使用固定宽度整数类型确保跨平台一致性
 */

#ifndef SERIAL_UART_H
#define SERIAL_UART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common/common_types.h"

/*============================================================================*/
/*                           HAL 库依赖处理                                     */
/*============================================================================*/

/**
 * @brief HAL 库头文件包含策略
 * @note 1. 如果是 STM32 硬件环境，使用真实的 stm32f4xx_hal.h
 *       2. 如果是 Windows 学习环境，使用 HAL 存根定义
 *       3. 存根定义仅提供类型声明，不包含功能实现
 */
#if defined(__STM32F4xx_HAL_H)
    /* 已包含真实 HAL 库，无需额外操作 */
#elif defined(_WIN32) || defined(_WIN64)
    /* Windows 环境：使用 HAL 存根（用于学习/测试） */
    #include "stm32_hal_stub.h"
#else
    /* 其他平台：尝试包含标准 HAL 库路径 */
    #include "stm32f4xx_hal.h"
#endif

/*============================================================================*/
/*                           宏定义                                             */
/*============================================================================*/

/** @brief UART 接收环形缓冲区大小（字节） */
#define UART_RX_BUFFER_SIZE   (256U)

/** @brief UART 发送超时时间（毫秒） */
#define UART_TX_TIMEOUT_MS    (1000U)

/**
 * @brief UART 默认波特率
 * @note 115200 是嵌入式开发中最常用的波特率
 */
#define UART_DEFAULT_BAUDRATE (115200U)

/*============================================================================*/
/*                           类型定义                                           */
/*============================================================================*/

/**
 * @brief UART 上下文结构体
 * @details 封装 UART 驱动运行时所需的所有状态信息，包括 HAL 句柄引用和
 *          接收环形缓冲区管理变量。每个 UART 实例对应一个上下文。
 *
 * @note 环形缓冲区（Ring Buffer / Circular Buffer）工作原理：
 *       - 缓冲区是一段固定大小的连续内存，逻辑上首尾相连形成环形
 *       - head 指针：由中断服务程序更新，指向下一个写入位置
 *       - tail 指针：由应用程序更新，指向下一个读取位置
 *       - 当 head == tail 时，缓冲区为空
 *       - 当 (head + 1) % SIZE == tail 时，缓冲区已满
 *       - 牺牲一个存储单元来区分"空"和"满"状态
 *       - volatile 修饰确保编译器不会优化掉对共享变量的访问
 */
typedef struct {
    UART_HandleTypeDef *handle;                        /**< HAL 库 UART 句柄指针 */
    uint8_t             rx_buffer[UART_RX_BUFFER_SIZE]; /**< 接收环形缓冲区 */
    volatile uint16_t   rx_head;                       /**< 缓冲区写入位置（ISR 更新） */
    volatile uint16_t   rx_tail;                       /**< 缓冲区读取位置（应用更新） */
} UartContext_t;

/*============================================================================*/
/*                           函数声明                                           */
/*============================================================================*/

/**
 * @brief  初始化 UART 上下文并启动中断接收
 * @param  ctx  UART 上下文指针（不能为 NULL）
 * @param  huart HAL 库 UART 句柄指针（不能为 NULL，需已由 CubeMX 初始化）
 * @retval STATUS_OK          初始化成功
 * @retval STATUS_NULL_PTR    参数为空指针
 * @retval STATUS_ERROR       启动中断接收失败
 * @note   调用前必须确保：
 *         1. HAL 库已初始化（HAL_Init()）
 *         2. UART 时钟已使能（如 __HAL_RCC_USART1_CLK_ENABLE()）
 *         3. GPIO 引脚已配置为 UART 复用功能
 *         4. NVIC 中断已使能（HAL_NVIC_EnableIRQ()）
 */
Status_t uart_init(UartContext_t *ctx, UART_HandleTypeDef *huart);

/**
 * @brief  通过 UART 发送数据（阻塞方式）
 * @param  ctx   UART 上下文指针（不能为 NULL）
 * @param  data  发送数据缓冲区指针（不能为 NULL）
 * @param  len   发送数据长度（字节）
 * @retval STATUS_OK          发送成功
 * @retval STATUS_NULL_PTR    参数为空指针
 * @retval STATUS_ERROR       发送失败或超时
 * @note   此函数为阻塞式调用，会等待所有数据发送完成或超时。
 *         在实时性要求高的场景中，应考虑使用 DMA 或中断发送方式。
 * @warning 不可在中断服务程序中调用此函数（会阻塞系统）
 */
Status_t uart_send(UartContext_t *ctx, const uint8_t *data, uint16_t len);

/**
 * @brief  从 UART 接收缓冲区读取一个字节
 * @param  ctx  UART 上下文指针（不能为 NULL）
 * @param  byte 接收字节存放地址（不能为 NULL）
 * @retval STATUS_OK          成功读取一个字节
 * @retval STATUS_NULL_PTR    参数为空指针
 * @retval STATUS_NOT_FOUND   缓冲区为空，无数据可读
 * @note   此函数从环形缓冲区中读取数据，不会阻塞。
 *         数据由 UART 接收中断自动填充到缓冲区中。
 */
Status_t uart_read_byte(UartContext_t *ctx, uint8_t *byte);

/**
 * @brief  获取 UART 接收缓冲区中的可读数据量
 * @param  ctx UART 上下文指针（不能为 NULL）
 * @return 缓冲区中可读取的字节数；若参数为 NULL 则返回 0
 * @note   返回值为近似值，因为 head 指针可能在中断中持续更新
 */
uint16_t uart_available(UartContext_t *ctx);

/**
 * @brief  清空 UART 接收缓冲区
 * @param  ctx UART 上下文指针（不能为 NULL）
 * @retval STATUS_OK       操作成功
 * @retval STATUS_NULL_PTR 参数为空指针
 * @note   将 tail 指针对齐到 head，丢弃所有未读数据
 */
Status_t uart_flush_rx(UartContext_t *ctx);

/**
 * @brief  UART 接收完成回调函数（需在 HAL 库回调中调用）
 * @param  huart 触发回调的 UART 句柄指针
 * @details 当 HAL 库完成一个字节的接收后，会调用此函数。
 *          本函数将接收到的字节存入环形缓冲区，并重新启动下一次中断接收。
 * @note   此函数运行在中断上下文中，应尽量简短快速。
 *         不要在此函数中调用阻塞式 API 或进行耗时操作。
 * @code
 * // 在 stm32f4xx_it.c 中重定义回调
 * void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
 * {
 *     uart_rx_callback(huart);
 * }
 * @endcode
 */
void uart_rx_callback(UART_HandleTypeDef *huart);

#ifdef __cplusplus
}
#endif

#endif
