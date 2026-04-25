/**
 * @file serial_i2c.h
 * @brief I2C (内部集成电路) 通信协议驱动 —— 头文件
 * @details 本模块实现了基于 STM32 HAL 库的 I2C 通信驱动，提供完整的初始化配置、
 *          主机发送与接收功能，以及总线设备扫描功能。
 *
 * @section i2c_protocol I2C 协议概述
 *
 * I2C (Inter-Integrated Circuit) 是由 Philips（现 NXP）公司开发的一种半双工、
 * 同步、多主多从的串行总线协议。"同步"意味着它使用专用时钟线来同步数据传输，
 * 这与 UART 的异步方式不同。"多主多从"意味着总线上可以有多个主机和多个从机，
 * 通过地址来区分不同的设备。
 *
 * @subsection i2c_signals 信号定义
 * I2C 仅使用两根信号线（加上地线 GND）：
 * - SCL (Serial Clock Line): 串行时钟线，由主机驱动，同步数据传输
 * - SDA (Serial Data Line):  串行数据线，双向数据传输，开漏输出
 *
 * @note I2C 的关键电气特性 —— 开漏输出（Open-Drain）：
 *   I2C 的 SCL 和 SDA 线都采用开漏输出结构，这意味着设备只能将线拉低（逻辑0），
 *   不能主动驱动高电平。高电平由外部上拉电阻实现。这种设计带来以下特性：
 *   1. 线与逻辑：任何设备拉低 = 总线为低，实现天然的仲裁机制
 *   2. 多设备安全：多个设备同时输出不会造成短路损坏
 *   3. 需要上拉电阻：典型值 4.7kΩ（标准模式）或 2.2kΩ（快速模式）
 *
 * @code
 *   VCC
 *    │
 *   [R] 上拉电阻 (4.7kΩ)
 *    │
 *    ├──── SCL/SDA 总线 ────┬────┬────
 *    │                      │    │    │
 *   ┌┴┐   ┌┴┐   ┌┴┐
 *   │ │   │ │   │ │   设备开漏输出
 *   └┬┘   └┬┘   └┬┘
 *    │      │    │
 *   GND    GND  GND
 *  主机    从机1 从机2
 * @endcode
 *
 * @subsection i2c_address 设备地址
 * I2C 使用7位或10位地址来寻址从机设备：
 * - 7位地址（最常用）: 地址范围 0x00~0x7F，但部分地址保留，可用地址 0x08~0x77
 * - 10位地址: 扩展地址空间，可支持更多设备
 *
 * 在实际通信中，地址字节格式如下（7位地址模式）：
 * @code
 *  地址字节 = [A6 A5 A4 A3 A2 A1 A0 R/W]
 *             |--- 7位设备地址 ---| 读写位
 *                                  0 = 写(主机→从机)
 *                                  1 = 读(从机→主机)
 *
 *  例如: 设备地址 0x50, 写操作 → 地址字节 = 0xA0
 *        设备地址 0x50, 读操作 → 地址字节 = 0xA1
 * @endcode
 *
 * @subsection i2c_transaction 通信事务格式
 * I2C 的每次通信称为一个"事务"(Transaction)，基本格式如下：
 *
 * @code
 *  主机写操作（主机向从机发送数据）:
 *  ┌─────┐ ┌──────────┐ ┌──────────┐ ┌──────────┐ ┌─────┐
 *  │START│ │ADDR + W  │ │  DATA 0  │ │  DATA N  │ │STOP │
 *  └─────┘ └──────────┘ └──────────┘ └──────────┘ └─────┘
 *           从机ACK      从机ACK      从机ACK
 *
 *  主机读操作（主机从从机接收数据）:
 *  ┌─────┐ ┌──────────┐ ┌──────────┐ ┌──────────┐ ┌─────┐
 *  │START│ │ADDR + R  │ │  DATA 0  │ │  DATA N  │ │STOP │
 *  └─────┘ └──────────┘ └──────────┘ └──────────┘ └─────┘
 *           从机ACK      主机ACK      主机NACK
 *                                    (最后一字节NACK
 *                                     通知从机停止发送)
 * @endcode
 *
 * @subsection i2c_signals_detail 关键信号定义
 *
 * 1. START 条件（起始信号）:
 *    SCL 为高电平时，SDA 从高电平跳变到低电平
 *    @code
 *         ┌───
 *    SDA ─┘        ← SDA 在 SCL 高时下降沿 = START
 *         ┌───
 *    SCL ─┘
 *    @endcode
 *
 * 2. STOP 条件（停止信号）:
 *    SCL 为高电平时，SDA 从低电平跳变到高电平
 *    @code
 *    ────┐
 *    SDA └─        ← SDA 在 SCL 高时上升沿 = STOP
 *    ────┐
 *    SCL └─
 *    @endcode
 *
 * 3. 数据有效性:
 *    SCL 为高电平时，SDA 上的数据必须稳定
 *    SCL 为低电平时，SDA 才可以改变
 *    @code
 *         ┌───┐   ┌───┐   ┌───┐
 *    SCL ─┘   └───┘   └───┘   └──
 *         ┌───────────────┐
 *    SDA ─┘  数据稳定有效  └────── 数据可变
 *    @endcode
 *
 * 4. ACK (应答): 接收方在第9个时钟周期将 SDA 拉低
 * 5. NACK (非应答): 接收方在第9个时钟周期保持 SDA 为高
 *
 * @subsection i2c_speed 速度模式
 * - 标准模式 (Standard):  100 kbit/s — 早期设备，速度最慢
 * - 快速模式 (Fast):      400 kbit/s — 最常用，多数传感器支持
 * - 快速模式+ (Fast+):    1 Mbit/s  — 高速设备
 * - 高速模式 (HS):        3.4 Mbit/s — 特殊应用
 *
 * @section i2c_usage 使用方法
 * @code
 * // 1. 初始化 I2C
 * I2cContext_t i2c_ctx;
 * i2c_init(&i2c_ctx, &hi2c1);
 *
 * // 2. 扫描总线上的设备
 * i2c_scan_bus(&i2c_ctx);
 *
 * // 3. 向从机写入数据（设备地址 0x50，寄存器地址 0x00）
 * uint8_t tx_data[] = {0x00, 0x01, 0x02, 0x03};
 * i2c_write(&i2c_ctx, 0x50, tx_data, sizeof(tx_data));
 *
 * // 4. 从从机读取数据（设备地址 0x50，寄存器地址 0x00）
 * uint8_t rx_data[4];
 * i2c_write_then_read(&i2c_ctx, 0x50, &reg_addr, 1, rx_data, 4);
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

#ifndef SERIAL_I2C_H
#define SERIAL_I2C_H

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

/** @brief I2C 操作超时时间（毫秒） */
#define I2C_TIMEOUT_MS          (100U)

/** @brief I2C 总线扫描起始地址（7位地址，0x00~0x07 保留） */
#define I2C_SCAN_ADDR_START     (0x08U)

/** @brief I2C 总线扫描结束地址（7位地址，0x78~0x7F 保留） */
#define I2C_SCAN_ADDR_END       (0x77U)

/**
 * @brief I2C 设备地址转换为 HAL 库左移格式
 * @param addr 7位设备地址（如 0x50）
 * @note  STM32 HAL 库的 I2C 函数要求地址左移1位（即 addr << 1），
 *        因为 HAL 库内部不再自动移位。使用此宏可避免混淆。
 * @code
 *   // 设备地址 0x50
 *   HAL_I2C_Mem_Write(&hi2c1, I2C_ADDR(0x50), ...);
 *   // 等效于 HAL_I2C_Mem_Write(&hi2c1, 0xA0, ...);
 * @endcode
 */
#define I2C_ADDR(addr)          ((addr) << 1)

/*============================================================================*/
/*                           类型定义                                           */
/*============================================================================*/

/**
 * @brief I2C 上下文结构体
 * @details 封装 I2C 驱动运行时所需的状态信息。
 *          目前仅保存 HAL 句柄引用，后续可扩展错误统计等功能。
 */
typedef struct {
    I2C_HandleTypeDef *handle;    /**< HAL 库 I2C 句柄指针 */
    uint32_t           error_count; /**< 累计通信错误次数 */
} I2cContext_t;

/*============================================================================*/
/*                           函数声明                                           */
/*============================================================================*/

/**
 * @brief  初始化 I2C 上下文
 * @param  ctx  I2C 上下文指针（不能为 NULL）
 * @param  hi2c HAL 库 I2C 句柄指针（不能为 NULL，需已由 CubeMX 初始化）
 * @retval STATUS_OK       初始化成功
 * @retval STATUS_NULL_PTR 参数为空指针
 * @note   调用前必须确保：
 *         1. HAL 库已初始化
 *         2. I2C 时钟已使能（如 __HAL_RCC_I2C1_CLK_ENABLE()）
 *         3. GPIO 引脚已配置为 I2C 复用开漏输出
 *         4. 外部上拉电阻已正确安装
 */
Status_t i2c_init(I2cContext_t *ctx, I2C_HandleTypeDef *hi2c);

/**
 * @brief  I2C 主机发送数据（阻塞方式）
 * @param  ctx       I2C 上下文指针（不能为 NULL）
 * @param  dev_addr  7位设备地址（如 0x50，无需左移）
 * @param  data      发送数据缓冲区指针（不能为 NULL）
 * @param  len       发送数据长度（字节）
 * @retval STATUS_OK          发送成功
 * @retval STATUS_NULL_PTR    参数为空指针
 * @retval STATUS_ERROR       发送失败（从机无应答或总线错误）
 * @retval STATUS_TIMEOUT     操作超时
 * @note   此函数执行完整的 I2C 写事务：
 *         START → 地址+W → 数据[0..N-1] → STOP
 */
Status_t i2c_write(I2cContext_t *ctx, uint8_t dev_addr,
                   const uint8_t *data, uint16_t len);

/**
 * @brief  I2C 主机接收数据（阻塞方式）
 * @param  ctx       I2C 上下文指针（不能为 NULL）
 * @param  dev_addr  7位设备地址（如 0x50，无需左移）
 * @param  data      接收数据缓冲区指针（不能为 NULL）
 * @param  len       接收数据长度（字节）
 * @retval STATUS_OK          接收成功
 * @retval STATUS_NULL_PTR    参数为空指针
 * @retval STATUS_ERROR       接收失败
 * @retval STATUS_TIMEOUT     操作超时
 * @note   此函数执行完整的 I2C 读事务：
 *         START → 地址+R → 数据[0..N-1] → STOP
 *         最后一字节返回 NACK，通知从机停止发送
 */
Status_t i2c_read(I2cContext_t *ctx, uint8_t dev_addr,
                  uint8_t *data, uint16_t len);

/**
 * @brief  I2C 写后读操作（先写寄存器地址，再读数据）
 * @param  ctx       I2C 上下文指针（不能为 NULL）
 * @param  dev_addr  7位设备地址（如 0x50，无需左移）
 * @param  wr_data   写入数据缓冲区（通常为寄存器地址，不能为 NULL）
 * @param  wr_len    写入数据长度
 * @param  rd_data   读取数据缓冲区（不能为 NULL）
 * @param  rd_len    读取数据长度
 * @retval STATUS_OK          操作成功
 * @retval STATUS_NULL_PTR    参数为空指针
 * @retval STATUS_ERROR       操作失败
 * @retval STATUS_TIMEOUT     操作超时
 * @details 这是最常用的 I2C 操作模式，用于读取从机的寄存器数据：
 *          1. 先发送寄存器地址（写操作），告诉从机要读哪个寄存器
 *          2. 再发送重复起始位(ReSTART)，切换为读操作
 *          3. 从机返回寄存器数据
 *          完整时序：
 *          START → ADDR+W → REG_ADDR → ReSTART → ADDR+R → DATA → STOP
 * @note   使用 HAL_I2C_Mem_Read() 实现，该函数内部自动处理 ReSTART
 */
Status_t i2c_write_then_read(I2cContext_t *ctx, uint8_t dev_addr,
                             const uint8_t *wr_data, uint16_t wr_len,
                             uint8_t *rd_data, uint16_t rd_len);

/**
 * @brief  扫描 I2C 总线上的所有设备
 * @param  ctx I2C 上下文指针（不能为 NULL）
 * @return 发现的设备数量；若参数为 NULL 则返回 0
 * @details 依次向地址 0x08~0x77 发送探测信号，若设备应答则表示该地址有设备。
 *          扫描结果通过 LOG_INFO 输出。
 * @note   扫描过程会占用总线，扫描期间不要进行其他 I2C 操作。
 *         某些设备可能不支持地址探测（如需要特定序列才能响应）。
 */
uint16_t i2c_scan_bus(I2cContext_t *ctx);

/**
 * @brief  检查指定地址的 I2C 设备是否在线
 * @param  ctx      I2C 上下文指针（不能为 NULL）
 * @param  dev_addr 7位设备地址
 * @return true 设备在线（应答），false 设备离线或参数无效
 */
bool i2c_is_device_ready(I2cContext_t *ctx, uint8_t dev_addr);

#ifdef __cplusplus
}
#endif

#endif
