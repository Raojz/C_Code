/**
 * @file serial_spi.h
 * @brief SPI (串行外设接口) 通信协议驱动 —— 头文件
 * @details 本模块实现了基于 STM32 HAL 库的 SPI 通信驱动，提供完整的初始化配置、
 *          全双工数据发送与接收功能，支持片选管理和多种 SPI 模式。
 *
 * @section spi_protocol SPI 协议概述
 *
 * SPI (Serial Peripheral Interface) 是由 Motorola 公司开发的一种全双工、同步、
 * 主从式串行通信协议。"全双工"意味着数据可以同时双向传输，"同步"意味着使用
 * 专用时钟线同步数据，"主从式"意味着总线上有一个主机控制所有通信。
 *
 * @subsection spi_signals 信号定义
 * SPI 使用四根信号线（相比 UART 的2线和 I2C 的2线，SPI 需要更多引脚）：
 *
 * - SCLK (Serial Clock): 串行时钟线，由主机驱动，同步所有数据传输
 * - MOSI (Master Out Slave In): 主机输出从机输入数据线
 *   主机通过此线向从机发送数据，从机通过此线接收数据
 * - MISO (Master In Slave Out): 主机输入从机输出数据线
 *   从机通过此线向主机发送数据，主机通过此线接收数据
 * - CS/SS (Chip Select / Slave Select): 片选信号线，低电平有效
 *   主机通过拉低对应从机的 CS 引脚来选中该从机，开始通信
 *
 * @code
 *   SPI 总线连接示意图：
 *
 *   ┌──────────┐          ┌──────────┐
 *   │          │──SCLK────│          │
 *   │   主机   │──MOSI───→│  从机1   │
 *   │  (MCU)   │←──MISO───│ (传感器) │
 *   │          │──CS1────→│          │
 *   │          │          └──────────┘
 *   │          │          ┌──────────┐
 *   │          │──MOSI───→│  从机2   │
 *   │          │←──MISO───│ (Flash)  │
 *   │          │──CS2────→│          │
 *   └──────────┘          └──────────┘
 *
 *   注意：SCLK、MOSI、MISO 是所有设备共享的，
 *         每个从机需要独立的 CS 片选信号！
 * @endcode
 *
 * @subsection spi_full_duplex 全双工通信原理
 *
 * SPI 的全双工特性源于其移位寄存器结构：
 * - 主机和从机各有一个移位寄存器
 * - 每个时钟周期，主机和从机同时交换1位数据
 * - 主机通过 MOSI 发送1位，同时从 MISO 接收1位
 * - 从机通过 MISO 发送1位，同时从 MOSI 接收1位
 * - 经过8个时钟周期，完成1字节的交换
 *
 * @code
 *   SPI 全双工数据交换示意图：
 *
 *   主机移位寄存器:  [D7][D6][D5][D4][D3][D2][D1][D0]
 *                    MOSI →→→→→→→→→→→→→→→→→→→→→→→→→→
 *   从机移位寄存器:  [d7][d6][d5][d4][d3][d2][d1][d0]
 *                    ←←←←←←←←←←←←←←←←←←←←←←←←←← MISO
 *
 *   8个SCLK周期后:
 *   主机获得: [d7][d6][d5][d4][d3][d2][d1][d0] (从从机接收的数据)
 *   从机获得: [D7][D6][D5][D4][D3][D2][D1][D0] (从主机接收的数据)
 * @endcode
 *
 * @subsection spi_clock_polarity_phase SPI 时钟极性与相位（4种模式）
 *
 * SPI 有4种工作模式，由两个参数决定：
 * - CPOL (Clock Polarity): 时钟极性，决定空闲时 SCLK 的电平
 *   - CPOL=0: 空闲时 SCLK 为低电平
 *   - CPOL=1: 空闲时 SCLK 为高电平
 * - CPHA (Clock Phase): 时钟相位，决定数据在哪个边沿采样
 *   - CPHA=0: 第一个边沿采样，第二个边沿移位
 *   - CPHA=1: 第一个边沿移位，第二个边沿采样
 *
 * @code
 *  Mode 0 (CPOL=0, CPHA=0) — 最常用:
 *       ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐
 *  SCLK ┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘
 *       空闲低  ↑   ↑   ↑   ↑   ↑   ↑   ↑   ↑
 *             采样 采样 采样 采样 采样 采样 采样 采样
 *  MISO ────┤D7├───┤D6├───┤D5├───┤D4├───┤D3├───...
 *  MOSI ────┤D7├───┤D6├───┤D5├───┤D4├───┤D3├───...
 *
 *  Mode 1 (CPOL=0, CPHA=1):
 *       ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐
 *  SCLK ┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘
 *           ↑   ↑   ↑   ↑   ↑   ↑   ↑   ↑
 *         采样 采样 采样 采样 采样 采样 采样 采样
 *
 *  Mode 2 (CPOL=1, CPHA=0):
 *  ──┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐
 *     ┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘
 *   空闲高↑   ↑   ↑   ↑   ↑   ↑   ↑   ↑
 *       采样 采样 采样 采样 采样 采样 采样 采样
 *
 *  Mode 3 (CPOL=1, CPHA=1):
 *  ──┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐
 *     ┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘
 *         ↑   ↑   ↑   ↑   ↑   ↑   ↑   ↑
 *       采样 采样 采样 采样 采样 采样 采样 采样
 * @endcode
 *
 * 4种模式总结：
 * | 模式  | CPOL | CPHA | 空闲电平 | 采样边沿     | 常见应用          |
 * |-------|------|------|----------|-------------|-------------------|
 * | Mode0 |  0   |  0   | 低       | 上升沿       | 最常用，多数设备支持 |
 * | Mode1 |  0   |  1   | 低       | 下降沿       | 部分传感器         |
 * | Mode2 |  1   |  0   | 高       | 下降沿       | 较少使用           |
 * | Mode3 |  1   |  1   | 高       | 上升沿       | SD卡、部分Flash    |
 *
 * @subsection spi_cs 片选(CS)信号管理
 *
 * SPI 没有像 I2C 那样的地址机制，而是通过 CS 片选信号来选择通信对象：
 * - CS 通常是低电平有效（Active Low）
 * - 主机将某个从机的 CS 拉低，选中该从机
 * - 通信期间 CS 必须保持低电平
 * - 通信结束后 CS 拉高，释放从机
 * - 同一时刻只能有一个从机的 CS 为低电平
 *
 * @subsection spi_speed SPI 速度与预分频
 *
 * SPI 时钟频率由外设时钟和预分频系数决定：
 * - SPI 时钟源 = APB2 时钟（对于 SPI1）或 APB1 时钟（对于 SPI2/SPI3）
 * - 实际 SCLK 频率 = 时钟源 / 预分频系数
 * - 常用预分频值：2, 4, 8, 16, 32, 64, 128, 256
 * - 例如：APB2 = 84MHz，预分频8 → SCLK = 10.5MHz
 *
 * @subsection spi_comparison SPI vs UART vs I2C 对比
 *
 * | 特性       | UART          | I2C           | SPI            |
 * |-----------|---------------|---------------|----------------|
 * | 通信方式   | 异步          | 同步          | 同步           |
 * | 双工方式   | 全双工        | 半双工        | 全双工         |
 * | 信号线数   | 2 (TX, RX)   | 2 (SCL, SDA) | 4+ (SCLK,MOSI,MISO,CS) |
 * | 设备寻址   | 无            | 7/10位地址    | 片选线         |
 * | 多主支持   | 否            | 是            | 否             |
 * | 最大速度   | ~5Mbit/s     | 3.4Mbit/s    | 50+Mbit/s     |
 * | 应答机制   | 无            | ACK/NACK     | 无             |
 * | 适用场景   | 调试/长距离   | 传感器/低速   | Flash/显示屏/高速|
 *
 * @section spi_usage 使用方法
 * @code
 * // 1. 初始化 SPI
 * SpiContext_t spi_ctx;
 * spi_init(&spi_ctx, &hspi1, CS_GPIO_Port, CS_Pin);
 *
 * // 2. 选中从机并发送数据
 * spi_cs_low(&spi_ctx);
 * uint8_t tx[] = {0x9F};  // 读取 Flash JEDEC ID 命令
 * uint8_t rx[4];
 * spi_transceive(&spi_ctx, tx, rx, sizeof(rx));
 * spi_cs_high(&spi_ctx);
 *
 * // 3. 仅发送数据（不关心接收）
 * spi_cs_low(&spi_ctx);
 * spi_write(&spi_ctx, data, len);
 * spi_cs_high(&spi_ctx);
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

#ifndef SERIAL_SPI_H
#define SERIAL_SPI_H

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

/** @brief SPI 操作超时时间（毫秒） */
#define SPI_TIMEOUT_MS          (1000U)

/** @brief SPI 仅发送时的虚拟接收字节（用于全双工交换） */
#define SPI_DUMMY_BYTE          (0xFFU)

/*============================================================================*/
/*                           类型定义                                           */
/*============================================================================*/

/**
 * @brief SPI 上下文结构体
 * @details 封装 SPI 驱动运行时所需的所有状态信息，包括 HAL 句柄引用和
 *          片选(CS)引脚的 GPIO 配置。每个 SPI 从设备对应一个上下文实例。
 *
 * @note CS 引脚管理说明：
 *   SPI 协议本身没有设备地址机制，需要通过 CS 片选信号来选择从机。
 *   本驱动将 CS 引脚信息封装在上下文中，提供 spi_cs_low() 和 spi_cs_high()
 *   函数来简化片选管理。如果总线上有多个从机，每个从机需要独立的上下文实例。
 */
typedef struct {
    SPI_HandleTypeDef *handle;     /**< HAL 库 SPI 句柄指针 */
    GPIO_TypeDef      *cs_port;    /**< CS 片选引脚所在 GPIO 端口 */
    uint16_t           cs_pin;     /**< CS 片选引脚编号 */
} SpiContext_t;

/*============================================================================*/
/*                           函数声明                                           */
/*============================================================================*/

/**
 * @brief  初始化 SPI 上下文
 * @param  ctx      SPI 上下文指针（不能为 NULL）
 * @param  hspi     HAL 库 SPI 句柄指针（不能为 NULL，需已由 CubeMX 初始化）
 * @param  cs_port  CS 片选引脚所在 GPIO 端口（如 GPIOA）
 * @param  cs_pin   CS 片选引脚编号（如 GPIO_PIN_4）
 * @retval STATUS_OK       初始化成功
 * @retval STATUS_NULL_PTR 参数为空指针
 * @note   初始化后 CS 引脚默认为高电平（未选中状态）。
 *         调用前必须确保：
 *         1. HAL 库已初始化
 *         2. SPI 时钟已使能（如 __HAL_RCC_SPI1_CLK_ENABLE()）
 *         3. GPIO 引脚已配置为 SPI 复用功能
 *         4. CS 引脚已配置为通用推挽输出
 */
Status_t spi_init(SpiContext_t *ctx, SPI_HandleTypeDef *hspi,
                  GPIO_TypeDef *cs_port, uint16_t cs_pin);

/**
 * @brief  SPI 全双工数据交换（阻塞方式）
 * @param  ctx      SPI 上下文指针（不能为 NULL）
 * @param  tx_data  发送数据缓冲区（不能为 NULL）
 * @param  rx_data  接收数据缓冲区（不能为 NULL）
 * @param  len      数据长度（字节）
 * @retval STATUS_OK          交换成功
 * @retval STATUS_NULL_PTR    参数为空指针
 * @retval STATUS_ERROR       交换失败
 * @retval STATUS_TIMEOUT     操作超时
 * @details SPI 全双工交换：同时发送 tx_data 中的数据并接收数据到 rx_data。
 *          发送和接收同时进行，每个时钟周期交换1位数据。
 * @note   调用前需先调用 spi_cs_low() 选中从机，完成后调用 spi_cs_high()。
 */
Status_t spi_transceive(SpiContext_t *ctx, const uint8_t *tx_data,
                        uint8_t *rx_data, uint16_t len);

/**
 * @brief  SPI 仅发送数据（阻塞方式）
 * @param  ctx      SPI 上下文指针（不能为 NULL）
 * @param  tx_data  发送数据缓冲区（不能为 NULL）
 * @param  len      发送数据长度（字节）
 * @retval STATUS_OK          发送成功
 * @retval STATUS_NULL_PTR    参数为空指针
 * @retval STATUS_ERROR       发送失败
 * @retval STATUS_TIMEOUT     操作超时
 * @details 仅发送数据，忽略接收到的数据。
 *          由于 SPI 是全双工协议，发送时必然同时接收，
 *          本函数内部丢弃接收到的数据。
 * @note   调用前需先调用 spi_cs_low() 选中从机。
 */
Status_t spi_write(SpiContext_t *ctx, const uint8_t *tx_data, uint16_t len);

/**
 * @brief  SPI 仅接收数据（阻塞方式）
 * @param  ctx      SPI 上下文指针（不能为 NULL）
 * @param  rx_data  接收数据缓冲区（不能为 NULL）
 * @param  len      接收数据长度（字节）
 * @retval STATUS_OK          接收成功
 * @retval STATUS_NULL_PTR    参数为空指针
 * @retval STATUS_ERROR       接收失败
 * @retval STATUS_TIMEOUT     操作超时
 * @details 仅接收数据，发送虚拟字节(0xFF)以产生时钟信号。
 *          由于 SPI 是全双工协议，接收数据需要主机产生时钟，
 *          因此主机必须发送数据（即使是无效数据）来驱动 SCLK。
 * @note   调用前需先调用 spi_cs_low() 选中从机。
 */
Status_t spi_read(SpiContext_t *ctx, uint8_t *rx_data, uint16_t len);

/**
 * @brief  拉低 CS 片选信号（选中从机）
 * @param  ctx SPI 上下文指针（不能为 NULL）
 * @retval STATUS_OK       操作成功
 * @retval STATUS_NULL_PTR 参数为空指针
 * @note   CS 低电平有效。拉低 CS 后，从机开始响应 SPI 通信。
 *         某些从机在 CS 下降沿会复位内部状态机。
 */
Status_t spi_cs_low(SpiContext_t *ctx);

/**
 * @brief  拉高 CS 片选信号（释放从机）
 * @param  ctx SPI 上下文指针（不能为 NULL）
 * @retval STATUS_OK       操作成功
 * @retval STATUS_NULL_PTR 参数为空指针
 * @note   CS 拉高后，从机停止响应 SPI 通信。
 *         某些从机在 CS 上升沿会触发内部操作（如 Flash 开始写入）。
 */
Status_t spi_cs_high(SpiContext_t *ctx);

#ifdef __cplusplus
}
#endif

#endif
