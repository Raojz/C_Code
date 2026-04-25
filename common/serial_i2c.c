/**
 * @file serial_i2c.c
 * @brief I2C (内部集成电路) 通信协议驱动 —— 实现文件
 * @details 本文件实现了 I2C 通信驱动的核心功能，包括初始化配置、主机发送与接收、
 *          写后读操作和总线设备扫描。每个函数都配有详尽的中文注释，解释
 *          I2C 协议的工作原理、时序特性和关键参数含义。
 *
 * @section i2c_init_sequence I2C 初始化流程
 *
 * 完整的 I2C 初始化需要以下步骤（通常由 STM32CubeMX 自动生成）：
 *
 * 1. 使能 I2C 外设时钟
 *    @code
 *    __HAL_RCC_I2C1_CLK_ENABLE();  // 使能 I2C1 时钟
 *    @endcode
 *    I2C1 通常挂载在 APB1 总线上。
 *
 * 2. 配置 GPIO 引脚为 I2C 复用开漏输出
 *    @code
 *    // SCL: PB6  → 复用开漏输出
 *    // SDA: PB7  → 复用开漏输出
 *    GPIO_InitTypeDef gpio = {0};
 *    gpio.Pin   = GPIO_PIN_6 | GPIO_PIN_7;
 *    gpio.Mode  = GPIO_MODE_AF_OD;       // 复用开漏（I2C 必须！）
 *    gpio.Pull  = GPIO_PULLUP;           // 内部上拉（外部也需上拉电阻）
 *    gpio.Speed = GPIO_SPEED_FREQ_HIGH;  // 快速模式需要高速 GPIO
 *    HAL_GPIO_Init(GPIOB, &gpio);
 *    @endcode
 *    @note I2C 必须使用开漏输出！推挽输出会导致总线冲突和器件损坏。
 *    开漏输出配合上拉电阻实现"线与"逻辑，允许多设备共享总线。
 *
 * 3. 配置 I2C 参数
 *    @code
 *    hi2c1.Instance             = I2C1;
 *    hi2c1.Init.ClockSpeed      = 400000U;            // 400kHz 快速模式
 *    hi2c1.Init.DutyCycle       = I2C_DUTYCYCLE_2;    // 时钟占空比
 *    hi2c1.Init.OwnAddress1     = 0x00;               // 主机模式，无自身地址
 *    hi2c1.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT; // 7位地址
 *    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE; // 禁用双地址
 *    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;  // 禁用广播
 *    hi2c1.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;   // 允许时钟拉伸
 *    HAL_I2C_Init(&hi2c1);
 *    @endcode
 *
 *    关键参数说明：
 *    - ClockSpeed: SCL 时钟频率。标准模式 ≤100kHz，快速模式 ≤400kHz
 *    - DutyCycle: 快速模式下的时钟占空比
 *      - I2C_DUTYCYCLE_2:  Tlow/Thigh = 2（默认，兼容性好）
 *      - I2C_DUTYCYCLE_16_9: Tlow/Thigh = 16/9（快速模式+专用）
 *    - NoStretchMode: 时钟拉伸(Clock Stretching)控制
 *      - 从机可以通过拉低 SCL 来暂停通信（给自己处理时间）
 *      - 禁用拉伸可能导致某些从机通信失败
 *
 * 4. I2C 时钟源配置（STM32F4 特有）
 *    @code
 *    // I2C 时钟必须来自 APB1 或特定时钟源，不能超过 50MHz
 *    // 如果 APB1 时钟 > 50MHz，需要使用 I2C 时钟源分频
 *    __HAL_RCC_I2C1_CONFIG(RCC_I2C1CLKSOURCE_PCLK1);
 *    @endcode
 *
 * @section i2c_timing_detail I2C 时序详解
 *
 * 一个完整的 I2C 字节传输时序（以写操作为例）：
 *
 * @code
 *  主机发送地址字节 0xA0（设备地址 0x50，写操作）：
 *
 *  SCL:  ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐
 *        ┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘
 *  SDA: ─┐ ┌─┐ ┌─┐ ┌─┘ └─┐ ┌─┘ └─┐ ┌─┘ └─┐ ┌───
 *        └─┘ └─┘ └───┘ └───┘ └───┘ └───┘ └───┘ └
 *  bit:  S  1   0   1   0   0   0   0   0   ACK
 *            A6  A5  A4  A3  A2  A1  A0  R/W  从机
 *            ↑                              ↑    应答
 *          MSB先发                        W=0
 *
 *  S = START 条件（SCL高时SDA下降沿）
 *  ACK = 从机应答（第9个时钟，从机拉低SDA）
 * @endcode
 *
 * @section i2c_common_issues I2C 常见问题与注意事项
 *
 * 1. 总线挂死（Bus Stuck）：
 *    如果从机在发送数据过程中复位，SDA 可能被拉低而无法释放。
 *    恢复方法：主机手动发送9个 SCL 时钟脉冲，然后发送 STOP 条件。
 *
 * 2. 上拉电阻选择：
 *    - 电阻过小：功耗增大，低电平可能超标
 *    - 电阻过大：上升沿变慢，限制通信速度
 *    - 经验值：100kHz → 4.7kΩ，400kHz → 2.2kΩ
 *
 * 3. 总线电容限制：
 *    I2C 规范要求总线电容 ≤ 400pF（标准模式）或 ≤ 550pF（快速模式）
 *    过长的走线或过多的设备会导致电容超标
 *
 * @author Embedded C Learning Project
 * @date 2026-04-21
 * @version 1.0.0
 */

#include "serial_i2c.h"

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

#include "common/common_utils.h"

/*============================================================================*/
/*                           私有辅助函数                                       */
/*============================================================================*/

/**
 * @brief  将 HAL 库错误码转换为本模块状态码
 * @param  hal_ret HAL 库返回值
 * @return 对应的 Status_t 状态码
 * @details HAL 库的返回值含义：
 *   - HAL_OK:      操作成功 (0x00)
 *   - HAL_ERROR:   操作失败 (0x01) — 总线错误、NACK等
 *   - HAL_BUSY:    外设忙   (0x02) — 上次操作尚未完成
 *   - HAL_TIMEOUT: 操作超时 (0x03) — 从机无响应
 */
static Status_t i2c_hal_to_status(HAL_StatusTypeDef hal_ret)
{
    Status_t status;

    switch (hal_ret) {
        case HAL_OK:
            status = STATUS_OK;
            break;
        case HAL_TIMEOUT:
            status = STATUS_TIMEOUT;
            break;
        case HAL_BUSY:
            status = STATUS_BUSY;
            break;
        default:
            status = STATUS_ERROR;
            break;
    }

    return status;
}

/*============================================================================*/
/*                           函数实现                                           */
/*============================================================================*/

/**
 * @brief  初始化 I2C 上下文
 * @param  ctx  I2C 上下文指针
 * @param  hi2c HAL 库 I2C 句柄指针
 * @retval STATUS_OK       成功
 * @retval STATUS_NULL_PTR 参数为空指针
 */
Status_t i2c_init(I2cContext_t *ctx, I2C_HandleTypeDef *hi2c)
{
    if ((NULL == ctx) || (NULL == hi2c)) {
        return STATUS_NULL_PTR;
    }

    ctx->handle      = hi2c;
    ctx->error_count = 0U;

    return STATUS_OK;
}

/**
 * @brief  I2C 主机发送数据（阻塞方式）
 * @param  ctx       I2C 上下文指针
 * @param  dev_addr  7位设备地址
 * @param  data      发送数据缓冲区指针
 * @param  len       发送数据长度
 * @retval STATUS_OK       发送成功
 * @retval STATUS_NULL_PTR 参数为空指针
 * @retval STATUS_ERROR    发送失败
 * @retval STATUS_TIMEOUT  操作超时
 *
 * @details I2C 写操作完整流程（硬件层面）：
 *
 * 1. 主机生成 START 条件
 *    - SCL 为高电平时，SDA 从高→低（下降沿）
 *    - 通知所有从机："通信开始，请注意后续地址"
 *
 * 2. 主机发送地址字节（8位 = 7位地址 + R/W位）
 *    - 地址位 A6~A0 指定目标从机
 *    - R/W = 0 表示写操作
 *    - 每位在 SCL 上升沿被从机采样
 *
 * 3. 从机发送 ACK（第9个时钟周期）
 *    - 地址匹配的从机将 SDA 拉低
 *    - 若无从机应答（NACK），主机应发送 STOP 退出
 *
 * 4. 主机发送数据字节
 *    - 每字节8位，MSB先发
 *    - 每字节后从机返回 ACK
 *
 * 5. 主机生成 STOP 条件
 *    - SCL 为高电平时，SDA 从低→高（上升沿）
 *    - 通知所有从机："通信结束，释放总线"
 *
 * @code
 * 完整写事务时序（写2字节数据到地址0x50的设备）：
 *
 *  SDA: ─┐  ┌──┐┌──┐┌──┐┌──┐┌──┐┌──┐┌──┐┌──┐┌─┐  ┌──┐┌──┐...┌─┐  ┌──
 *        └──┘  └┘  └┘  └┘  └┘  └┘  └┘  └┘  └┘ └──┘  └┘  └...  └──┘
 *        START  A6  A5  A4  A3  A2  A1  A0  W  ACK  D7  D6 ... ACK STOP
 *              └────── 0x50 地址 ──────┘ 0      └── 数据 ──┘
 * @endcode
 */
Status_t i2c_write(I2cContext_t *ctx, uint8_t dev_addr,
                   const uint8_t *data, uint16_t len)
{
    if ((NULL == ctx) || (NULL == data)) {
        return STATUS_NULL_PTR;
    }

    if (NULL == ctx->handle) {
        return STATUS_NULL_PTR;
    }

    if (0U == len) {
        return STATUS_INVALID_PARAM;
    }

    HAL_StatusTypeDef hal_ret = HAL_I2C_Master_Transmit(
        ctx->handle,
        I2C_ADDR(dev_addr),
        (uint8_t *)data,
        len,
        I2C_TIMEOUT_MS);

    if (HAL_OK != hal_ret) {
        ctx->error_count++;
    }

    return i2c_hal_to_status(hal_ret);
}

/**
 * @brief  I2C 主机接收数据（阻塞方式）
 * @param  ctx       I2C 上下文指针
 * @param  dev_addr  7位设备地址
 * @param  data      接收数据缓冲区指针
 * @param  len       接收数据长度
 * @retval STATUS_OK       接收成功
 * @retval STATUS_NULL_PTR 参数为空指针
 * @retval STATUS_ERROR    接收失败
 * @retval STATUS_TIMEOUT  操作超时
 *
 * @details I2C 读操作的关键细节：
 *
 * 1. 地址字节中 R/W = 1，表示读操作
 *
 * 2. 数据方向反转：
 *    写操作时，主机驱动 SDA 发送数据；
 *    读操作时，从机驱动 SDA 发送数据，主机在 SCL 上升沿采样
 *
 * 3. ACK/NACK 策略：
 *    - 主机每接收一个字节后发送 ACK（拉低 SDA），表示"继续发送"
 *    - 主机接收最后一个字节后发送 NACK（保持 SDA 高），表示"停止发送"
 *    - NACK 是通知从机释放 SDA 控制权的信号
 *
 * 4. 时序图（读2字节数据）：
 *    @code
 *    START → ADDR+R → ACK → DATA0 → ACK → DATA1 → NACK → STOP
 *            主机发    从机   从机发   主机   从机发   主机
 *                     ACK    数据0   ACK    数据1   NACK
 *    @endcode
 */
Status_t i2c_read(I2cContext_t *ctx, uint8_t dev_addr,
                  uint8_t *data, uint16_t len)
{
    if ((NULL == ctx) || (NULL == data)) {
        return STATUS_NULL_PTR;
    }

    if (NULL == ctx->handle) {
        return STATUS_NULL_PTR;
    }

    if (0U == len) {
        return STATUS_INVALID_PARAM;
    }

    HAL_StatusTypeDef hal_ret = HAL_I2C_Master_Receive(
        ctx->handle,
        I2C_ADDR(dev_addr),
        data,
        len,
        I2C_TIMEOUT_MS);

    if (HAL_OK != hal_ret) {
        ctx->error_count++;
    }

    return i2c_hal_to_status(hal_ret);
}

/**
 * @brief  I2C 写后读操作（先写寄存器地址，再读数据）
 * @param  ctx       I2C 上下文指针
 * @param  dev_addr  7位设备地址
 * @param  wr_data   写入数据缓冲区（通常为寄存器地址）
 * @param  wr_len    写入数据长度
 * @param  rd_data   读取数据缓冲区
 * @param  rd_len    读取数据长度
 * @retval STATUS_OK       操作成功
 * @retval STATUS_NULL_PTR 参数为空指针
 * @retval STATUS_ERROR    操作失败
 * @retval STATUS_TIMEOUT  操作超时
 *
 * @details 写后读是 I2C 最常用的操作模式，用于读取从机的寄存器数据。
 *   大多数 I2C 从机设备（如 EEPROM、传感器）内部都有寄存器映射，
 *   读取流程为：
 *   1. 先写入目标寄存器地址，设置从机内部指针
 *   2. 再发送重复起始位(Repeated START)，切换为读模式
 *   3. 从机从指定寄存器开始返回数据
 *
 *   Repeated START 与普通 START 的信号形式完全相同，区别在于：
 *   - 普通 START：总线从空闲状态开始
 *   - Repeated START：在已占用的总线中发送，不释放总线
 *   使用 Repeated START 而非 STOP+START 的好处：
 *   - 保证操作的原子性，不会被其他主机抢占
 *   - 避免某些从机在 STOP 后复位内部寄存器指针
 *
 * @code
 * 写后读时序（从设备0x50的寄存器0x00读取2字节数据）：
 *
 *  ┌──────── 写阶段 ────────┐┌──────── 读阶段 ────────┐
 *  │                        ││                        │
 *  S → ADDR+W → ACK → REG → ACK → Sr → ADDR+R → ACK → D0 → ACK → D1 → NACK → P
 *       主机    从机   主机   从机      主机    从机  从机   主机  从机   主机
 *       发送    应答   发送   应答      发送    应答  发送   应答  发送   应答
 *
 *  S  = START
 *  Sr = Repeated START
 *  P  = STOP
 * @endcode
 *
 * @note HAL_I2C_Mem_Read() 内部自动处理了 Repeated START，
 *       应用层无需手动管理时序。
 */
Status_t i2c_write_then_read(I2cContext_t *ctx, uint8_t dev_addr,
                             const uint8_t *wr_data, uint16_t wr_len,
                             uint8_t *rd_data, uint16_t rd_len)
{
    if ((NULL == ctx) || (NULL == wr_data) || (NULL == rd_data)) {
        return STATUS_NULL_PTR;
    }

    if (NULL == ctx->handle) {
        return STATUS_NULL_PTR;
    }

    if ((0U == wr_len) || (0U == rd_len)) {
        return STATUS_INVALID_PARAM;
    }

    HAL_StatusTypeDef hal_ret = HAL_I2C_Mem_Read(
        ctx->handle,
        I2C_ADDR(dev_addr),
        (uint16_t)wr_data[0U],
        I2C_MEMADD_SIZE_8BIT,
        rd_data,
        rd_len,
        I2C_TIMEOUT_MS);

    if (HAL_OK != hal_ret) {
        ctx->error_count++;
    }

    return i2c_hal_to_status(hal_ret);
}

/**
 * @brief  扫描 I2C 总线上的所有设备
 * @param  ctx I2C 上下文指针
 * @return 发现的设备数量
 *
 * @details 扫描原理：
 *   对每个可能的 I2C 地址（0x08~0x77），主机发送该地址的写操作，
 *   如果从机返回 ACK，说明该地址有设备存在；如果返回 NACK，则无设备。
 *
 *   地址范围说明：
 *   - 0x00~0x07: I2C 规范保留地址
 *     - 0x00: 广播地址
 *     - 0x01: CBUS 兼容地址
 *     - 0x02~0x03: 保留给不同总线格式
 *     - 0x04~0x07: 保留给高速模式
 *   - 0x78~0x7F: 10位地址扩展保留
 *   - 0x08~0x77: 用户可用地址（共112个）
 *
 * @note 扫描过程使用 HAL_I2C_IsDeviceReady()，该函数内部会：
 *   1. 发送 START 条件
 *   2. 发送地址字节（写方向）
 *   3. 等待 ACK/NACK
 *   4. 发送 STOP 条件
 *   每个地址尝试多次（默认3次），确保结果可靠。
 */
uint16_t i2c_scan_bus(I2cContext_t *ctx)
{
    if (NULL == ctx) {
        return 0U;
    }

    if (NULL == ctx->handle) {
        return 0U;
    }

    uint16_t found = 0U;

    LOG_INFO("I2C 总线扫描开始...");
    LOG_INFO("扫描地址范围: 0x%02X ~ 0x%02X", I2C_SCAN_ADDR_START, I2C_SCAN_ADDR_END);

    for (uint8_t addr = I2C_SCAN_ADDR_START; addr <= I2C_SCAN_ADDR_END; addr++) {
        HAL_StatusTypeDef hal_ret = HAL_I2C_IsDeviceReady(
            ctx->handle,
            I2C_ADDR(addr),
            3U,
            I2C_TIMEOUT_MS);

        if (HAL_OK == hal_ret) {
            LOG_INFO("  发现设备: 地址 0x%02X", addr);
            found++;
        }
    }

    LOG_INFO("I2C 总线扫描完成，共发现 %u 个设备", (unsigned int)found);

    return found;
}

/**
 * @brief  检查指定地址的 I2C 设备是否在线
 * @param  ctx      I2C 上下文指针
 * @param  dev_addr 7位设备地址
 * @return true 设备在线，false 设备离线或参数无效
 *
 * @details 使用 HAL_I2C_IsDeviceReady() 进行探测。
 *   该函数尝试与指定地址的设备建立通信，若设备应答则返回 HAL_OK。
 *   内部会尝试多次（此处设为3次），以排除偶发干扰。
 *
 * @note 在进行关键操作前，建议先调用此函数确认设备在线，
 *       避免因设备离线导致长时间超时等待。
 */
bool i2c_is_device_ready(I2cContext_t *ctx, uint8_t dev_addr)
{
    if ((NULL == ctx) || (NULL == ctx->handle)) {
        return false;
    }

    HAL_StatusTypeDef hal_ret = HAL_I2C_IsDeviceReady(
        ctx->handle,
        I2C_ADDR(dev_addr),
        3U,
        I2C_TIMEOUT_MS);

    return (HAL_OK == hal_ret);
}
