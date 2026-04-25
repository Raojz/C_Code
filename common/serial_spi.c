/**
 * @file serial_spi.c
 * @brief SPI (串行外设接口) 通信协议驱动 —— 实现文件
 * @details 本文件实现了 SPI 通信驱动的核心功能，包括初始化配置、全双工数据交换、
 *          仅发送、仅接收和片选信号管理。每个函数都配有详尽的中文注释，解释
 *          SPI 协议的工作原理、时序特性和关键参数含义。
 *
 * @section spi_init_sequence SPI 初始化流程
 *
 * 完整的 SPI 初始化需要以下步骤（通常由 STM32CubeMX 自动生成）：
 *
 * 1. 使能 SPI 外设时钟
 *    @code
 *    __HAL_RCC_SPI1_CLK_ENABLE();  // 使能 SPI1 时钟
 *    @endcode
 *    SPI1 挂载在 APB2 总线上（高速总线，最高84MHz），
 *    SPI2/SPI3 挂载在 APB1 总线上（低速总线，最高42MHz）。
 *
 * 2. 配置 GPIO 引脚为 SPI 复用功能
 *    @code
 *    // SCLK: PA5  → 复用推挽输出
 *    // MOSI: PA7  → 复用推挽输出
 *    // MISO: PA6  → 复用输入
 *    // CS:   PA4  → 通用推挽输出（软件管理）
 *    GPIO_InitTypeDef gpio = {0};
 *
 *    // SPI 功能引脚
 *    gpio.Pin   = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
 *    gpio.Mode  = GPIO_MODE_AF_PP;       // 复用推挽
 *    gpio.Pull  = GPIO_NOPULL;           // SPI 不需要上下拉
 *    gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH; // 高速（SPI可达数十MHz）
 *    HAL_GPIO_Init(GPIOA, &gpio);
 *
 *    // CS 片选引脚（软件控制）
 *    gpio.Pin   = GPIO_PIN_4;
 *    gpio.Mode  = GPIO_MODE_OUTPUT_PP;   // 通用推挽输出
 *    gpio.Pull  = GPIO_PULLUP;           // 默认高电平（未选中）
 *    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
 *    HAL_GPIO_Init(GPIOA, &gpio);
 *    @endcode
 *    @note SPI 的 SCLK、MOSI 使用推挽输出（不是开漏！），这与 I2C 不同。
 *    推挽输出可以主动驱动高/低电平，支持更高的切换速度。
 *    MISO 引脚由从机驱动，主机配置为输入。
 *
 * 3. 配置 SPI 参数
 *    @code
 *    hspi1.Instance           = SPI1;
 *    hspi1.Init.Mode          = SPI_MODE_MASTER;         // 主机模式
 *    hspi1.Init.Direction     = SPI_DIRECTION_2LINES;    // 全双工
 *    hspi1.Init.DataSize      = SPI_DATASIZE_8BIT;       // 8位数据帧
 *    hspi1.Init.CLKPolarity   = SPI_POLARITY_LOW;        // CPOL=0
 *    hspi1.Init.CLKPhase      = SPI_PHASE_1EDGE;         // CPHA=0
 *    hspi1.Init.NSS           = SPI_NSS_SOFT;            // 软件管理CS
 *    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8; // 预分频8
 *    hspi1.Init.FirstBit      = SPI_FIRSTBIT_MSB;        // MSB先发
 *    hspi1.Init.TIMode        = SPI_TIMODE_DISABLE;      // 禁用TI模式
 *    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE; // 禁用CRC
 *    HAL_SPI_Init(&hspi1);
 *    @endcode
 *
 *    关键参数说明：
 *    - Mode: 主机模式(MASTER)或从机模式(SLAVE)
 *    - Direction:
 *      - 2LINES: 全双工（MOSI+MISO同时使用）
 *      - 2LINES_RXONLY: 半双工（仅接收，MOSI不使用）
 *      - 1LINE: 单线双向（MOSI复用为双向数据线）
 *    - DataSize: 8位或16位数据帧
 *    - CLKPolarity/CLKPhase: 决定 SPI 模式（Mode0~Mode3）
 *    - NSS: 片选管理方式
 *      - SPI_NSS_SOFT: 软件管理（推荐，灵活控制CS时序）
 *      - SPI_NSS_HARD_INPUT: 硬件输入（从机模式）
 *      - SPI_NSS_HARD_OUTPUT: 硬件输出（主机模式，仅支持1个从机）
 *    - BaudRatePrescaler: SCLK 预分频系数
 *      - SPI_BAUDRATEPRESCALER_2:   APB时钟/2
 *      - SPI_BAUDRATEPRESCALER_4:   APB时钟/4
 *      - SPI_BAUDRATEPRESCALER_8:   APB时钟/8
 *      - SPI_BAUDRATEPRESCALER_16:  APB时钟/16
 *      - SPI_BAUDRATEPRESCALER_32:  APB时钟/32
 *      - SPI_BAUDRATEPRESCALER_64:  APB时钟/64
 *      - SPI_BAUDRATEPRESCALER_128: APB时钟/128
 *      - SPI_BAUDRATEPRESCALER_256: APB时钟/256
 *    - FirstBit: MSB先发（标准SPI）或 LSB先发（罕见）
 *
 * @section spi_cs_detail CS 片选信号管理
 *
 * CS 信号在 SPI 通信中扮演关键角色，其作用远不止"选择从机"：
 *
 * 1. 通信边界标记：
 *    - CS 下降沿：标志一次通信事务的开始
 *    - CS 上升沿：标志一次通信事务的结束
 *    - 从机通过 CS 边沿来同步自己的状态机
 *
 * 2. 从机复位：
 *    - 许多 SPI 从机在 CS 上升沿会复位内部命令解析器
 *    - 如果一次命令序列被中断（CS 拉高），从机会丢弃不完整的命令
 *    - 这提供了一种错误恢复机制：拉高 CS 再拉低，重新开始
 *
 * 3. 数据帧定界：
 *    - 某些从机（如 SPI Flash）允许 CS 持续保持低电平，
 *      连续读取多字节数据（Burst Read）
 *    - 某些从机要求每字节之间 CS 必须拉高
 *
 * @code
 * 典型 SPI 通信时序（读取 Flash JEDEC ID）：
 *
 *  CS:   ────┐                                         ┌────
 *            └─────────────────────────────────────────┘
 *  SCLK: ────┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐...
 *            └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘
 *  MOSI: ────┤ 9 │ F │ X │ X │ X │ X │ X │ X │ X │ X │ X │...
 *            └─命令0x9F─┘└── 虚拟字节(不关心) ──────────┘
 *  MISO: ────┤ X │ X │Mfr│Mem│Cap│ X │ X │ X │ X │ X │ X │...
 *            └──不关心──┘└── JEDEC ID 数据 ───────────┘
 *
 *  主机发送命令 0x9F（读JEDEC ID），同时接收3字节ID数据
 * @endcode
 *
 * @author Embedded C Learning Project
 * @date 2026-04-21
 * @version 1.0.0
 */

#include "serial_spi.h"

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
/*                           函数实现                                           */
/*============================================================================*/

/**
 * @brief  初始化 SPI 上下文
 * @param  ctx      SPI 上下文指针
 * @param  hspi     HAL 库 SPI 句柄指针
 * @param  cs_port  CS 片选引脚所在 GPIO 端口
 * @param  cs_pin   CS 片选引脚编号
 * @retval STATUS_OK       成功
 * @retval STATUS_NULL_PTR 参数为空指针
 *
 * @details 初始化流程：
 *   1. 参数合法性检查
 *   2. 保存 HAL 句柄和 CS 引脚信息
 *   3. 将 CS 引脚设为高电平（未选中状态）
 *
 * @note CS 初始化为高电平是 SPI 协议的要求：
 *   - CS 是低电平有效（Active Low）
 *   - 空闲时 CS 必须为高电平，表示没有选中任何从机
 *   - 如果初始化时 CS 为低电平，从机可能误以为通信已开始
 *     导致状态机进入错误状态
 */
Status_t spi_init(SpiContext_t *ctx, SPI_HandleTypeDef *hspi,
                  GPIO_TypeDef *cs_port, uint16_t cs_pin)
{
    if ((NULL == ctx) || (NULL == hspi) || (NULL == cs_port)) {
        return STATUS_NULL_PTR;
    }

    ctx->handle  = hspi;
    ctx->cs_port = cs_port;
    ctx->cs_pin  = cs_pin;

    HAL_GPIO_WritePin(ctx->cs_port, ctx->cs_pin, GPIO_PIN_SET);

    return STATUS_OK;
}

/**
 * @brief  SPI 全双工数据交换（阻塞方式）
 * @param  ctx      SPI 上下文指针
 * @param  tx_data  发送数据缓冲区
 * @param  rx_data  接收数据缓冲区
 * @param  len      数据长度
 * @retval STATUS_OK       交换成功
 * @retval STATUS_NULL_PTR 参数为空指针
 * @retval STATUS_ERROR    交换失败
 * @retval STATUS_TIMEOUT  操作超时
 *
 * @details SPI 全双工交换的硬件工作原理：
 *
 * 1. 主机将 tx_data[0] 写入发送数据寄存器 (DR)
 * 2. 数据从 DR 移入发送移位寄存器
 * 3. 移位寄存器通过 MOSI 逐位发送，同时从 MISO 逐位接收
 * 4. 接收到的数据移入接收移位寄存器
 * 5. 接收移位寄存器满8位后，数据转入接收数据寄存器 (DR)
 * 6. 应用从 DR 读取接收到的字节到 rx_data[0]
 * 7. 重复步骤1~6，直到所有字节交换完成
 *
 * @code
 * 全双工交换时序（交换1字节，Mode0: CPOL=0, CPHA=0）：
 *
 *  CS:   ──────┐                                    ┌──────
 *              └────────────────────────────────────┘
 *  SCLK:       ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐
 *        ──────┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘──────
 *  MOSI: ──────┤D7 │D6 │D5 │D4 │D3 │D2 │D1 │D0 │──────
 *              └── 主机发送 ──────────────────────┘
 *  MISO: ──────┤d7 │d6 │d5 │d4 │d3 │d2 │d1 │d0 │──────
 *              └── 从机发送 ──────────────────────┘
 *              ↑                                    ↑
 *            采样时刻(上升沿)                     通信结束
 * @endcode
 *
 * @note HAL_SPI_TransmitReceive() 内部处理了 TXE/RXNE 标志的等待，
 *       确保发送和接收的同步。应用层无需手动管理这些标志。
 */
Status_t spi_transceive(SpiContext_t *ctx, const uint8_t *tx_data,
                        uint8_t *rx_data, uint16_t len)
{
    if ((NULL == ctx) || (NULL == tx_data) || (NULL == rx_data)) {
        return STATUS_NULL_PTR;
    }

    if (NULL == ctx->handle) {
        return STATUS_NULL_PTR;
    }

    if (0U == len) {
        return STATUS_INVALID_PARAM;
    }

    HAL_StatusTypeDef hal_ret = HAL_SPI_TransmitReceive(
        ctx->handle,
        (uint8_t *)tx_data,
        rx_data,
        len,
        SPI_TIMEOUT_MS);

    if (HAL_OK == hal_ret) {
        return STATUS_OK;
    }

    if (HAL_TIMEOUT == hal_ret) {
        return STATUS_TIMEOUT;
    }

    return STATUS_ERROR;
}

/**
 * @brief  SPI 仅发送数据（阻塞方式）
 * @param  ctx      SPI 上下文指针
 * @param  tx_data  发送数据缓冲区
 * @param  len      发送数据长度
 * @retval STATUS_OK       发送成功
 * @retval STATUS_NULL_PTR 参数为空指针
 * @retval STATUS_ERROR    发送失败
 * @retval STATUS_TIMEOUT  操作超时
 *
 * @details SPI "仅发送"的本质：
 *   由于 SPI 是全双工协议，发送数据时必然同时接收数据。
 *   HAL_SPI_Transmit() 内部会忽略接收到的数据（不读取 RXNE 标志），
 *   但硬件层面，每个发送的字节都会触发一次接收操作。
 *
 *   这意味着：
 *   - 主机发送时，从机可能同时在 MISO 上输出数据
 *   - 如果从机不需要响应（如写命令），从机输出的是无效数据
 *   - 如果从机需要响应，应该使用 spi_transceive() 来同时获取接收数据
 *
 * @code
 * 仅发送时序（向 Flash 写入使能命令 WREN = 0x06）：
 *
 *  CS:   ──────┐                         ┌──────
 *              └─────────────────────────┘
 *  SCLK:       ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐
 *        ──────┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘──────
 *  MOSI: ──────┤0  │0  │0  │0  │0  │1  │1  │0  │──────
 *              └─────── 0x06 (WREN) ────────┘
 *  MISO: ──────┤ X │ X │ X │ X │ X │ X │ X │ X │──────
 *              └── 从机输出（不关心） ──────┘
 * @endcode
 */
Status_t spi_write(SpiContext_t *ctx, const uint8_t *tx_data, uint16_t len)
{
    if ((NULL == ctx) || (NULL == tx_data)) {
        return STATUS_NULL_PTR;
    }

    if (NULL == ctx->handle) {
        return STATUS_NULL_PTR;
    }

    if (0U == len) {
        return STATUS_INVALID_PARAM;
    }

    HAL_StatusTypeDef hal_ret = HAL_SPI_Transmit(
        ctx->handle,
        (uint8_t *)tx_data,
        len,
        SPI_TIMEOUT_MS);

    if (HAL_OK == hal_ret) {
        return STATUS_OK;
    }

    if (HAL_TIMEOUT == hal_ret) {
        return STATUS_TIMEOUT;
    }

    return STATUS_ERROR;
}

/**
 * @brief  SPI 仅接收数据（阻塞方式）
 * @param  ctx      SPI 上下文指针
 * @param  rx_data  接收数据缓冲区
 * @param  len      接收数据长度
 * @retval STATUS_OK       接收成功
 * @retval STATUS_NULL_PTR 参数为空指针
 * @retval STATUS_ERROR    接收失败
 * @retval STATUS_TIMEOUT  操作超时
 *
 * @details SPI "仅接收"的关键原理：
 *
 *   SPI 是主机驱动时钟的协议，从机无法主动发送数据。
 *   主机必须产生 SCLK 时钟信号，从机才能在 MISO 上输出数据。
 *   因此，"仅接收"时主机仍需发送数据来驱动时钟。
 *
 *   发送的虚拟字节(Dummy Byte)通常为 0xFF，原因：
 *   - 0xFF 的所有位都是1，不会与大多数从机命令冲突
 *   - 某些从机将 0xFF 解释为"无操作"(NOP)
 *   - 发送0xFF时，MOSI 保持高电平，不会干扰从机
 *
 * @code
 * 仅接收时序（从 Flash 读取数据，假设已发送读命令和地址）：
 *
 *  CS:   ──────┐                                         ┌──────
 *              └─────────────────────────────────────────┘
 *  SCLK:       ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐...
 *        ──────┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘──────
 *  MOSI: ──────┤1  │1  │1  │1  │1  │1  │1  │1  │1  │──────
 *              └─────── 0xFF (虚拟字节) ────────┘
 *  MISO: ──────┤D7 │D6 │D5 │D4 │D3 │D2 │D1 │D0 │D7 │──────
 *              └─────── 从机输出数据 ──────────────┘
 * @endcode
 *
 * @note 本函数内部使用 spi_transceive() 实现，发送缓冲区填充为 0xFF。
 *       对于大数据量的接收，可以考虑使用 DMA 方式以提高效率。
 */
Status_t spi_read(SpiContext_t *ctx, uint8_t *rx_data, uint16_t len)
{
    if ((NULL == ctx) || (NULL == rx_data)) {
        return STATUS_NULL_PTR;
    }

    if (NULL == ctx->handle) {
        return STATUS_NULL_PTR;
    }

    if (0U == len) {
        return STATUS_INVALID_PARAM;
    }

    HAL_StatusTypeDef hal_ret = HAL_SPI_Receive(
        ctx->handle,
        rx_data,
        len,
        SPI_TIMEOUT_MS);

    if (HAL_OK == hal_ret) {
        return STATUS_OK;
    }

    if (HAL_TIMEOUT == hal_ret) {
        return STATUS_TIMEOUT;
    }

    return STATUS_ERROR;
}

/**
 * @brief  拉低 CS 片选信号（选中从机）
 * @param  ctx SPI 上下文指针
 * @retval STATUS_OK       操作成功
 * @retval STATUS_NULL_PTR 参数为空指针
 *
 * @details CS 下降沿对从机的影响：
 *
 * 1. 选中从机：CS 拉低后，从机的 MISO 输出使能，开始响应主机
 * 2. 状态机复位：许多从机在 CS 下降沿复位命令解析器
 * 3. 时序要求：CS 下降沿到第一个 SCLK 边沿之间需要一定的建立时间
 *    （具体值查阅从机数据手册，通常为几十纳秒）
 *
 * @code
 * CS 与 SCLK 的时序关系：
 *
 *  CS:   ──────┐                    ┌──────
 *              └────────────────────┘
 *              ← t_ssetup →
 *  SCLK:              ┌─┐ ┌─┐ ┌─┐
 *        ─────────────┘ └─┘ └─┘ └─┘──────
 *              ↑        ↑
 *           CS下降沿  第一个SCLK边沿
 *           t_ssetup = CS建立时间（从机要求的最小值）
 * @endcode
 *
 * @note 使用 HAL_GPIO_WritePin() 而非 HAL_GPIO_TogglePin()，
 *       确保操作是确定性的（无论当前状态如何，都设为低电平）。
 */
Status_t spi_cs_low(SpiContext_t *ctx)
{
    if (NULL == ctx) {
        return STATUS_NULL_PTR;
    }

    HAL_GPIO_WritePin(ctx->cs_port, ctx->cs_pin, GPIO_PIN_RESET);

    return STATUS_OK;
}

/**
 * @brief  拉高 CS 片选信号（释放从机）
 * @param  ctx SPI 上下文指针
 * @retval STATUS_OK       操作成功
 * @retval STATUS_NULL_PTR 参数为空指针
 *
 * @details CS 上升沿对从机的影响：
 *
 * 1. 释放从机：CS 拉高后，从机的 MISO 输出变为高阻态
 * 2. 触发操作：某些从机在 CS 上升沿执行挂起操作
 *    - SPI Flash: CS↑ 触发页编程(Page Program)或扇区擦除
 *    - ADC: CS↑ 触发转换开始
 * 3. 时序要求：最后一个 SCLK 边沿到 CS 上升沿之间需要保持时间
 *    （具体值查阅从机数据手册）
 *
 * @code
 * SCLK 与 CS 的时序关系：
 *
 *  SCLK:       ┌─┐ ┌─┐ ┌─┐
 *        ──────┘ └─┘ └─┘ └─┘──────
 *                          ← t_shold →
 *  CS:   ──────┐                    ┌──────
 *              └────────────────────┘
 *              ↑                    ↑
 *        最后一个SCLK边沿       CS上升沿
 *        t_shold = CS保持时间（从机要求的最小值）
 * @endcode
 *
 * @warning CS 拉高后，必须等待从机完成内部操作才能再次拉低 CS。
 *          例如 SPI Flash 在页编程期间，CS 拉低后发送读状态寄存器命令
 *          检查 BUSY 位，直到编程完成才能进行下一次操作。
 */
Status_t spi_cs_high(SpiContext_t *ctx)
{
    if (NULL == ctx) {
        return STATUS_NULL_PTR;
    }

    HAL_GPIO_WritePin(ctx->cs_port, ctx->cs_pin, GPIO_PIN_SET);

    return STATUS_OK;
}
