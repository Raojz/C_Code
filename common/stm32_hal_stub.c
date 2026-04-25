/**
 * @file stm32_hal_stub.c
 * @brief STM32 HAL 库函数存根实现（用于 Windows 学习环境）
 * @details 在 Windows/PC 环境下学习嵌入式 C 语言时，
 *          无法访问真实的 STM32 HAL 库。
 *          本文件提供了 HAL 库核心函数的空实现（Stub），
 *          使串口通信驱动代码可以在 Windows 上完成编译和链接。
 *
 * @section usage 使用说明
 * - 本文件仅供 **Windows 学习环境** 使用
 * - 所有函数返回 HAL_OK（表示成功），不执行任何实际操作
 * - 实际的 STM32 硬件开发应使用 CubeMX 生成的真实 HAL 库
 *
 * @section stub_behavior 存根行为
 * - GPIO 函数: 无操作（NOP）
 * - I2C 函数: 返回 HAL_OK，不进行实际通信
 * - UART 函数: 返回 HAL_OK，不进行实际传输
 * - SPI 函数: 返回 HAL_OK，不进行实际传输
 *
 * @warning 本文件中的函数仅为存根实现，不具备真实功能！
 *          切勿在真实硬件上使用此存根！
 *
 * @author Embedded C Learning Project
 * @date 2026-04-21
 * @version 1.0.0
 */

#include "stm32_hal_stub.h"

/*============================================================================*/
/*                           GPIO 函数存根实现                                 */
/*============================================================================*/

/**
 * @brief 初始化 GPIO 引脚（存根实现）
 * @note Windows 环境下无操作
 */
void HAL_GPIO_Init(GPIO_TypeDef *port, const GPIO_InitTypeDef *gpio_init)
{
    (void)port;
    (void)gpio_init;
}

/**
 * @brief 设置 GPIO 引脚电平（存根实现）
 * @note Windows 环境下无操作
 */
void HAL_GPIO_WritePin(GPIO_TypeDef *port, uint16_t pin, uint8_t pin_state)
{
    (void)port;
    (void)pin;
    (void)pin_state;
}

/*============================================================================*/
/*                           I2C 函数存根实现                                  */
/*============================================================================*/

/**
 * @brief 初始化 I2C 外设（存根实现）
 * @retval 始终返回 HAL_OK
 */
HAL_StatusTypeDef HAL_I2C_Init(I2C_HandleTypeDef *hi2c)
{
    (void)hi2c;
    return HAL_OK;
}

/**
 * @brief I2C 主机发送数据（存根实现）
 * @retval 始终返回 HAL_OK
 * @note 不执行实际 I2C 通信
 */
HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c,
                                           uint16_t dev_addr,
                                           uint8_t *data,
                                           uint16_t size,
                                           uint32_t timeout)
{
    (void)hi2c;
    (void)dev_addr;
    (void)data;
    (void)size;
    (void)timeout;
    return HAL_OK;
}

/**
 * @brief I2C 主机接收数据（存根实现）
 * @retval 始终返回 HAL_OK
 * @note 不执行实际 I2C 通信
 */
HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef *hi2c,
                                          uint16_t dev_addr,
                                          uint8_t *data,
                                          uint16_t size,
                                          uint32_t timeout)
{
    (void)hi2c;
    (void)dev_addr;
    (void)data;
    (void)size;
    (void)timeout;

    if ((NULL != data) && (size > 0U)) {
        for (uint16_t i = 0U; i < size; i++) {
            data[i] = 0U;  /* 清零接收缓冲区 */
        }
    }

    return HAL_OK;
}

/**
 * @brief I2C 内存读取操作（存根实现）
 * @retval 始终返回 HAL_OK
 */
HAL_StatusTypeDef HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c,
                                    uint16_t dev_addr,
                                    uint16_t mem_addr,
                                    uint16_t mem_addr_size,
                                    uint8_t *data,
                                    uint16_t size,
                                    uint32_t timeout)
{
    (void)hi2c;
    (void)dev_addr;
    (void)mem_addr;
    (void)mem_addr_size;
    (void)size;
    (void)timeout;

    if ((NULL != data) && (size > 0U)) {
        for (uint16_t i = 0U; i < size; i++) {
            data[i] = 0U;
        }
    }

    return HAL_OK;
}

/**
 * @brief I2C 内存写入操作（存根实现）
 * @retval 始终返回 HAL_OK
 */
HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c,
                                     uint16_t dev_addr,
                                     uint16_t mem_addr,
                                     uint16_t mem_addr_size,
                                     const uint8_t *data,
                                     uint16_t size,
                                     uint32_t timeout)
{
    (void)hi2c;
    (void)dev_addr;
    (void)mem_addr;
    (void)mem_addr_size;
    (void)data;
    (void)size;
    (void)timeout;
    return HAL_OK;
}

/**
 * @brief 检查 I2C 设备是否就绪（存根实现）
 * @retval 始终返回 HAL_OK
 */
HAL_StatusTypeDef HAL_I2C_IsDeviceReady(I2C_HandleTypeDef *hi2c,
                                         uint16_t dev_addr,
                                         uint32_t trials,
                                         uint32_t timeout)
{
    (void)hi2c;
    (void)dev_addr;
    (void)trials;
    (void)timeout;
    return HAL_OK;
}

/*============================================================================*/
/*                           UART 函数存根实现                                 */
/*============================================================================*/

/**
 * @brief 初始化 UART 外设（存根实现）
 * @retval 始终返回 HAL_OK
 */
HAL_StatusTypeDef HAL_UART_Init(UART_HandleTypeDef *huart)
{
    (void)huart;
    return HAL_OK;
}

/**
 * @brief UART 阻塞方式发送数据（存根实现）
 * @retval 始终返回 HAL_OK
 */
HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart,
                                     const uint8_t *data,
                                     uint16_t size,
                                     uint32_t timeout)
{
    (void)huart;
    (void)data;
    (void)size;
    (void)timeout;
    return HAL_OK;
}

/**
 * @brief UART 中断方式接收数据（存根实现）
 * @retval 始终返回 HAL_OK
 */
HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *huart,
                                       uint8_t *data,
                                       uint16_t size)
{
    (void)huart;
    (void)data;
    (void)size;
    return HAL_OK;
}

#if defined(__GNUC__) || defined(__clang__)
__attribute__((weak))
#endif
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    (void)huart;
}

/*============================================================================*/
/*                           SPI 函数存根实现                                  */
/*============================================================================*/

/**
 * @brief 初始化 SPI 外设（存根实现）
 * @retval 始终返回 HAL_OK
 */
HAL_StatusTypeDef HAL_SPI_Init(SPI_HandleTypeDef *hspi)
{
    (void)hspi;
    return HAL_OK;
}

/**
 * @brief SPI 全双工同时发送和接收数据（存根实现）
 * @retval 始终返回 HAL_OK
 */
HAL_StatusTypeDef HAL_SPI_TransmitReceive(SPI_HandleTypeDef *hspi,
                                          const uint8_t *tx_data,
                                          uint8_t *rx_data,
                                          uint16_t size,
                                          uint32_t timeout)
{
    (void)hspi;
    (void)tx_data;
    (void)size;
    (void)timeout;

    if ((NULL != rx_data) && (size > 0U)) {
        for (uint16_t i = 0U; i < size; i++) {
            rx_data[i] = 0U;  /* 清零接收缓冲区 */
        }
    }

    return HAL_OK;
}

/**
 * @brief SPI 仅发送数据（存根实现）
 * @retval 始终返回 HAL_OK
 */
HAL_StatusTypeDef HAL_SPI_Transmit(SPI_HandleTypeDef *hspi,
                                    const uint8_t *data,
                                    uint16_t size,
                                    uint32_t timeout)
{
    (void)hspi;
    (void)data;
    (void)size;
    (void)timeout;
    return HAL_OK;
}

/**
 * @brief SPI 仅接收数据（存根实现）
 * @retval 始终返回 HAL_OK
 */
HAL_StatusTypeDef HAL_SPI_Receive(SPI_HandleTypeDef *hspi,
                                   uint8_t *data,
                                   uint16_t size,
                                   uint32_t timeout)
{
    (void)hspi;
    (void)size;
    (void)timeout;

    if ((NULL != data) && (size > 0U)) {
        for (uint16_t i = 0U; i < size; i++) {
            data[i] = 0U;
        }
    }

    return HAL_OK;
}
