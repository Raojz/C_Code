/**
 * @file serial_uart.c
 * @brief UART (通用异步收发传输器) 通信协议驱动 —— 实现文件
 * @details 本文件实现了 UART 通信驱动的核心功能，包括初始化配置、数据发送、
 *          中断接收和环形缓冲区管理。每个函数都配有详尽的中文注释，解释
 *          UART 协议的工作原理、时序特性和关键参数含义。
 *
 * @section uart_init_sequence UART 初始化流程
 *
 * 完整的 UART 初始化需要以下步骤（通常由 STM32CubeMX 自动生成）：
 *
 * 1. 使能 UART 外设时钟
 *    @code
 *    __HAL_RCC_USART1_CLK_ENABLE();  // 使能 USART1 时钟
 *    @endcode
 *    STM32 的所有外设默认时钟是关闭的，以节省功耗。必须先使能时钟才能
 *    访问外设寄存器。USART1 通常挂载在 APB2 总线上（高速总线），
 *    而 USART2/3/4/5 挂载在 APB1 总线上（低速总线）。
 *
 * 2. 配置 GPIO 引脚为 UART 复用功能
 *    @code
 *    // TX 引脚: PA9  → 复用推挽输出
 *    // RX 引脚: PA10 → 复用输入
 *    GPIO_InitTypeDef gpio = {0};
 *    gpio.Pin   = GPIO_PIN_9;
 *    gpio.Mode  = GPIO_MODE_AF_PP;       // 复用推挽
 *    gpio.Pull  = GPIO_PULLUP;           // 上拉（空闲高电平）
 *    gpio.Speed = GPIO_SPEED_FREQ_HIGH;  // 高速
 *    HAL_GPIO_Init(GPIOA, &gpio);
 *    @endcode
 *    TX 引脚配置为推挽输出，因为 UART 空闲状态为高电平，推挽输出可以
 *    主动驱动高电平和低电平。RX 引脚配置为输入，通常加上拉电阻确保
 *    空闲时为高电平，防止误检测起始位。
 *
 * 3. 配置 UART 参数
 *    @code
 *    huart1.Instance        = USART1;
 *    huart1.Init.BaudRate   = 115200U;          // 波特率
 *    huart1.Init.WordLength = UART_WORDLENGTH_8B; // 8位数据
 *    huart1.Init.StopBits   = UART_STOPBITS_1;    // 1位停止位
 *    huart1.Init.Parity     = UART_PARITY_NONE;   // 无校验
 *    huart1.Init.Mode       = UART_MODE_TX_RX;    // 收发模式
 *    huart1.Init.HwFlowCtl  = UART_HWCONTROL_NONE;// 无硬件流控
 *    huart1.Init.OverSampling = UART_OVERSAMPLING_16; // 16倍过采样
 *    HAL_UART_Init(&huart1);
 *    @endcode
 *
 * 4. 配置 NVIC 中断优先级并使能
 *    @code
 *    HAL_NVIC_SetPriority(USART1_IRQn, 5U, 0U);  // 抢占优先级5, 子优先级0
 *    HAL_NVIC_EnableIRQ(USART1_IRQn);
 *    @endcode
 *
 * @section uart_interrupt UART 中断接收机制
 *
 * STM32 HAL 库的中断接收流程如下：
 *
 * 1. 应用调用 HAL_UART_Receive_IT()，指定接收缓冲区和数据长度
 * 2. UART 硬件接收到一个字节后，触发 USARTx_IRQn 中断
 * 3. 在中断服务程序 USARTx_IRQHandler() 中调用 HAL_UART_IRQHandler()
 * 4. HAL 库处理中断标志，将接收到的字节存入缓冲区
 * 5. 当接收完指定数量的字节后，调用 HAL_UART_RxCpltCallback() 回调
 * 6. 在回调中，我们将数据存入环形缓冲区，并重新启动下一次接收
 *
 * 本驱动使用"逐字节中断"模式：每次只启动1个字节的接收，回调中重新启动，
 * 这样可以实时处理每个接收到的字节，而不需要等待固定长度的数据包。
 *
 * @author Embedded C Learning Project
 * @date 2026-04-21
 * @version 1.0.0
 */

#include "serial_uart.h"

/*============================================================================*/
/*                           HAL 库依赖处理                                     */
/*============================================================================*/

/**
 * @brief HAL 库头文件包含策略
 * @note 实现文件需要完整的 HAL 库定义（包括函数声明和寄存器定义）
 */
#if defined(__STM32F4xx_HAL_H)
    /* 已包含真实 HAL 库 */
#elif defined(_WIN32) || defined(_WIN64)
    /* Windows 环境：使用 HAL 存根（仅用于语法检查） */
    #include "stm32_hal_stub.h"
#else
    /* STM32 硬件环境：包含完整 HAL 库 */
    #include "stm32f4xx_hal.h"
#endif

/*============================================================================*/
/*                           私有变量                                           */
/*============================================================================*/

/**
 * @brief 全局 UART 上下文指针，用于中断回调中查找对应的上下文
 * @details 由于 HAL 库的回调函数只传递 UART 句柄指针，我们需要一种方式
 *          从句柄指针反查到我们的上下文结构体。这里使用一个简单的全局指针，
 *          适用于单 UART 实例的场景。如果需要多实例，可改为数组或链表。
 */
static UartContext_t *s_active_ctx = NULL;

/*============================================================================*/
/*                           函数实现                                           */
/*============================================================================*/

/**
 * @brief  初始化 UART 上下文并启动中断接收
 * @param  ctx   UART 上下文指针
 * @param  huart HAL 库 UART 句柄指针
 * @retval STATUS_OK       成功
 * @retval STATUS_NULL_PTR 参数为空指针
 * @retval STATUS_ERROR    启动中断接收失败
 *
 * @details 初始化流程：
 *   1. 参数合法性检查（空指针保护）
 *   2. 保存 HAL 句柄引用到上下文中
 *   3. 清零环形缓冲区的读写指针
 *   4. 注册为活跃上下文（供中断回调使用）
 *   5. 启动第一次中断接收（接收1个字节到缓冲区头部）
 *
 * @note 环形缓冲区初始化时 head = tail = 0，表示缓冲区为空。
 *       首次启动接收时，数据将写入 rx_buffer[0] 的位置。
 */
Status_t uart_init(UartContext_t *ctx, UART_HandleTypeDef *huart)
{
    if ((NULL == ctx) || (NULL == huart)) {
        return STATUS_NULL_PTR;
    }

    ctx->handle  = huart;
    ctx->rx_head = 0U;
    ctx->rx_tail = 0U;

    s_active_ctx = ctx;

    HAL_StatusTypeDef hal_ret = HAL_UART_Receive_IT(huart,
                                                     &ctx->rx_buffer[0U],
                                                     1U);
    if (HAL_OK != hal_ret) {
        return STATUS_ERROR;
    }

    return STATUS_OK;
}

/**
 * @brief  通过 UART 发送数据（阻塞方式）
 * @param  ctx  UART 上下文指针
 * @param  data 发送数据缓冲区指针
 * @param  len  发送数据长度
 * @retval STATUS_OK       发送成功
 * @retval STATUS_NULL_PTR 参数为空指针
 * @retval STATUS_ERROR    发送失败或超时
 *
 * @details UART 发送过程（硬件层面）：
 *
 * 1. 数据写入发送数据寄存器 (TDR)
 *    应用调用 HAL_UART_Transmit()，数据被逐字节写入 TDR
 *
 * 2. 数据从 TDR 移入移位寄存器
 *    当移位寄存器空闲时，TDR 中的数据自动移入移位寄存器，
 *    同时 TXE (Transmit Data Register Empty) 标志置位，
 *    表示 TDR 已空，可以写入下一个字节
 *
 * 3. 移位寄存器按位发送
 *    移位寄存器按照 UART 帧格式，依次发送：
 *    起始位(0) → 数据位(LSB first) → 校验位(可选) → 停止位(1)
 *    每一位的持续时间 = 1 / 波特率（秒）
 *    例如 115200 baud 时，每位持续约 8.68 μs
 *
 * 4. 发送完成
 *    当移位寄存器中最后一帧的停止位发送完毕后，
 *    TC (Transmission Complete) 标志置位
 *
 * @code
 * UART 发送时序（8N1 格式，发送 0x55 = 01010101b）：
 *
 *  空闲    起始  D0  D1  D2  D3  D4  D5  D6  D7  停止  空闲
 *  ────┐   ┌   ┌   ┌   ┌   ┌   ┌   ┌   ┌   ┌   ┌   ┌────
 *      │   │   │   │   │   │   │   │   │   │   │   │
 *  ────┘   └───┘   └───┘   └───┘   └───┘   └───┘   └
 *       1   0   1   0   1   0   1   0   1   1
 *       ↑   ↑                                   ↑   ↑
 *     空闲 起始位                              停止位 空闲
 *          (0x55 的 LSB 先发: 1,0,1,0,1,0,1,0)
 * @endcode
 */
Status_t uart_send(UartContext_t *ctx, const uint8_t *data, uint16_t len)
{
    if ((NULL == ctx) || (NULL == data) || (0U == len)) {
        return STATUS_NULL_PTR;
    }

    if (NULL == ctx->handle) {
        return STATUS_NULL_PTR;
    }

    HAL_StatusTypeDef hal_ret = HAL_UART_Transmit(ctx->handle,
                                                   (uint8_t *)data,
                                                   (uint16_t)len,
                                                   UART_TX_TIMEOUT_MS);

    return (HAL_OK == hal_ret) ? STATUS_OK : STATUS_ERROR;
}

/**
 * @brief  从 UART 接收缓冲区读取一个字节
 * @param  ctx  UART 上下文指针
 * @param  byte 接收字节存放地址
 * @retval STATUS_OK         成功读取
 * @retval STATUS_NULL_PTR   参数为空指针
 * @retval STATUS_NOT_FOUND  缓冲区为空
 *
 * @details 环形缓冲区读取原理：
 *
 *   读取前（head=5, tail=2, 可读3字节）:
 *   ┌───┬───┬───┬───┬───┬───┬───┬───┐
 *   │   │   │ T │ D │ D │ H │   │   │
 *   └───┴───┴───┴───┴───┴───┴───┴───┘
 *           ↑               ↑
 *         tail=2          head=5
 *         (读位置)        (写位置)
 *
 *   读取1字节后（head=5, tail=3, 可读2字节）:
 *   ┌───┬───┬───┬───┬───┬───┬───┬───┐
 *   │   │   │   │ T │ D │ H │   │   │
 *   └───┴───┴───┴───┴───┴───┴───┴───┘
 *               ↑           ↑
 *             tail=3      head=5
 *
 *   当 head == tail 时，缓冲区为空，无数据可读。
 *   tail 只在读取时由应用程序更新，head 只在接收中断中更新，
 *   两者不存在写入冲突，因此不需要加锁保护。
 */
Status_t uart_read_byte(UartContext_t *ctx, uint8_t *byte)
{
    if ((NULL == ctx) || (NULL == byte)) {
        return STATUS_NULL_PTR;
    }

    if (ctx->rx_head == ctx->rx_tail) {
        return STATUS_NOT_FOUND;
    }

    *byte = ctx->rx_buffer[ctx->rx_tail];

    ctx->rx_tail = (ctx->rx_tail + 1U) % UART_RX_BUFFER_SIZE;

    return STATUS_OK;
}

/**
 * @brief  获取 UART 接收缓冲区中的可读数据量
 * @param  ctx UART 上下文指针
 * @return 可读取的字节数
 *
 * @details 计算公式：
 *   当 head >= tail 时: 可读量 = head - tail
 *   当 head <  tail 时: 可读量 = (缓冲区大小 - tail) + head
 *   两种情况可统一为: 可读量 = (head - tail + 缓冲区大小) % 缓冲区大小
 *
 *   示例（缓冲区大小=8）：
 *   head=5, tail=2 → 可读 = (5-2+8)%8 = 3 字节
 *   head=1, tail=6 → 可读 = (1-6+8)%8 = 3 字节（环形回绕）
 */
uint16_t uart_available(UartContext_t *ctx)
{
    if (NULL == ctx) {
        return 0U;
    }

    return (uint16_t)((ctx->rx_head - ctx->rx_tail + UART_RX_BUFFER_SIZE)
                       % UART_RX_BUFFER_SIZE);
}

/**
 * @brief  清空 UART 接收缓冲区
 * @param  ctx UART 上下文指针
 * @retval STATUS_OK       操作成功
 * @retval STATUS_NULL_PTR 参数为空指针
 *
 * @details 将 tail 指针对齐到 head，等效于丢弃所有未读数据。
 *   注意：此操作不会停止中断接收，新数据仍会继续写入缓冲区。
 */
Status_t uart_flush_rx(UartContext_t *ctx)
{
    if (NULL == ctx) {
        return STATUS_NULL_PTR;
    }

    ctx->rx_tail = ctx->rx_head;

    return STATUS_OK;
}

/**
 * @brief  UART 接收完成回调函数
 * @param  huart 触发回调的 UART 句柄指针
 *
 * @details 此函数在以下时机被调用：
 *   1. UART 硬件接收到一个完整字节
 *   2. HAL_UART_IRQHandler() 处理中断标志
 *   3. HAL 库调用 HAL_UART_RxCpltCallback()
 *
 *   回调处理流程：
 *   1. 检查是否是我们的 UART 实例（通过句柄指针匹配）
 *   2. 推进 head 指针到下一个位置
 *   3. 检查缓冲区是否已满（head+1 == tail），满则丢弃最旧数据
 *   4. 重新启动下一次中断接收
 *
 * @note  此函数运行在中断上下文中！必须遵守以下规则：
 *   - 函数体必须尽可能短小快速
 *   - 禁止调用任何阻塞式 API（如 HAL_Delay、printf）
 *   - 禁止调用非 ISR 安全的 RTOS API
 *   - 共享变量（head/tail）已用 volatile 修饰，确保可见性
 *
 * @warning 如果在此函数中执行耗时操作，将导致：
 *   - 系统中断延迟增大，影响实时性
 *   - 可能丢失后续 UART 接收数据（溢出错误 ORE）
 *   - 看门狗超时复位（如果使用了看门狗）
 */
void uart_rx_callback(UART_HandleTypeDef *huart)
{
    if (NULL == huart) {
        return;
    }

    if (NULL == s_active_ctx) {
        return;
    }

    if (huart != s_active_ctx->handle) {
        return;
    }

    uint16_t next_head = (s_active_ctx->rx_head + 1U) % UART_RX_BUFFER_SIZE;

    if (next_head != s_active_ctx->rx_tail) {
        s_active_ctx->rx_head = next_head;
    }

    HAL_UART_Receive_IT(huart,
                        &s_active_ctx->rx_buffer[s_active_ctx->rx_head],
                        1U);
}
