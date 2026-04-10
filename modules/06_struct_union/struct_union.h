/**
 * @file struct_union.h
 * @brief 结构体与联合体模块头文件
 * @details 涵盖结构体、联合体、枚举、位域等复合数据类型知识点。
 *          掌握这些类型是进行复杂数据组织和硬件映射的基础。
 * 
 * @section main_features 主要内容
 * - 结构体：定义、初始化、成员访问、指针、数组
 * - 嵌套结构体：多层嵌套、实际应用
 * - 联合体：共享内存、数据复用、协议解析
 * - 枚举类型：状态定义、命令类型、错误码
 * - 位域：硬件寄存器映射、精确位控制
 * - 内存对齐：对齐规则、填充、紧凑结构体
 * 
 * @section embedded_importance 嵌入式开发重要性
 * - 结构体是数据组织的核心方式
 * - 联合体用于数据复用和协议解析
 * - 位域用于硬件寄存器映射
 * - 内存对齐影响性能和兼容性
 * 
 * @section best_practices 最佳实践
 * - 使用typedef定义结构体类型
 * - 结构体指针作为函数参数
 * - 使用枚举提高代码可读性
 * - 注意结构体内存对齐
 * 
 * @author Embedded C Learning Project
 * @date 2026-03-22
 * @version 1.0.0
 * 
 * @note 本文件遵循 MISRA-C:2012 编码规范
 */

#ifndef STRUCT_UNION_H
#define STRUCT_UNION_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common/common_types.h"

/*============================================================================*/
/*                           结构体定义                                         */
/*============================================================================*/

/**
 * @brief 学生结构体
 */
typedef struct {
    char name[32];      /**< 姓名 */
    int32_t age;        /**< 年龄 */
    float score;        /**< 分数 */
} Student_t;

/**
 * @brief 点结构体
 */
typedef struct {
    uint16_t x;         /**< X坐标 */
    uint16_t y;         /**< Y坐标 */
} Point_t;

/**
 * @brief 线段结构体
 */
typedef struct {
    Point_t start;      /**< 起点 */
    Point_t end;        /**< 终点 */
    uint32_t color;     /**< 颜色 */
} Line_t;

/**
 * @brief 设备信息结构体
 */
typedef struct {
    char name[32];      /**< 设备名称 */
    char version[16];   /**< 版本号 */
    uint16_t year;      /**< 生产年份 */
} DeviceInfo_t;

/**
 * @brief 设备状态结构体
 */
typedef struct {
    bool is_running;        /**< 运行状态 */
    float temperature;      /**< 温度 */
    uint32_t error_code;    /**< 错误码 */
} DeviceStatus_t;

/**
 * @brief 设备配置结构体
 */
typedef struct {
    uint32_t sample_rate;   /**< 采样率 */
    uint32_t baud_rate;     /**< 波特率 */
    bool enabled;           /**< 使能标志 */
} DeviceConfig_t;

/**
 * @brief 设备结构体（嵌套示例）
 */
typedef struct {
    DeviceInfo_t info;      /**< 设备信息 */
    DeviceStatus_t status;  /**< 设备状态 */
    DeviceConfig_t config;  /**< 设备配置 */
} Device_t;

/*============================================================================*/
/*                           联合体定义                                         */
/*============================================================================*/

/**
 * @brief 数据转换联合体
 */
typedef union {
    uint8_t bytes[4];   /**< 字节数组 */
    uint16_t words[2];  /**< 字数组 */
    uint32_t dword;     /**< 双字 */
} DataConverter_t;

/**
 * @brief 协议数据联合体
 */
typedef union {
    uint32_t raw;           /**< 原始数据 */
    struct {
        uint8_t byte0;      /**< 字节0 */
        uint8_t byte1;      /**< 字节1 */
        uint8_t byte2;      /**< 字节2 */
        uint8_t byte3;      /**< 字节3 */
    };
} ProtocolData_t;

/*============================================================================*/
/*                           枚举定义                                           */
/*============================================================================*/

/**
 * @brief 系统状态枚举
 */
typedef enum {
    STATE_IDLE = 0,     /**< 空闲状态 */
    STATE_RUNNING = 1,  /**< 运行状态 */
    STATE_PAUSED = 2,   /**< 暂停状态 */
    STATE_ERROR = 3     /**< 错误状态 */
} SystemState_t;

/**
 * @brief 设备类型枚举
 */
typedef enum {
    DEVICE_TYPE_SENSOR = 0,     /**< 传感器 */
    DEVICE_TYPE_ACTUATOR = 1,   /**< 执行器 */
    DEVICE_TYPE_CONTROLLER = 2  /**< 控制器 */
} DeviceType_t;

/**
 * @brief 传感器模式枚举
 */
typedef enum {
    SENSOR_MODE_SINGLE = 0,     /**< 单次模式 */
    SENSOR_MODE_CONTINUOUS = 1  /**< 连续模式 */
} SensorMode_t;

/**
 * @brief 设备状态枚举
 */
typedef enum {
    DEVICE_STATUS_OK = 0,       /**< 正常 */
    DEVICE_STATUS_ERROR = 1,    /**< 错误 */
    DEVICE_STATUS_BUSY = 2      /**< 忙碌 */
} DeviceStatusEnum_t;

/*============================================================================*/
/*                           位域定义                                           */
/*============================================================================*/

/**
 * @brief 配置寄存器位域
 */
typedef struct {
    uint32_t enable : 1;        /**< 使能位 */
    uint32_t mode : 2;          /**< 模式位 */
    uint32_t speed : 3;         /**< 速度位 */
    uint32_t reserved : 26;     /**< 保留位 */
} ConfigReg_t;

/*============================================================================*/
/*                           版本结构体                                         */
/*============================================================================*/

/**
 * @brief 版本号结构体
 */
typedef struct {
    uint8_t major;      /**< 主版本号 */
    uint8_t minor;      /**< 次版本号 */
    uint8_t patch;      /**< 补丁版本号 */
} Version_t;

/**
 * @brief 传感器配置结构体
 */
typedef struct {
    uint32_t sample_rate;   /**< 采样率 */
    uint8_t resolution;     /**< 分辨率 */
    SensorMode_t mode;      /**< 模式 */
} SensorConfig_t;

/**
 * @brief 设备描述符结构体
 */
typedef struct {
    uint16_t device_id;         /**< 设备ID */
    DeviceType_t device_type;   /**< 设备类型 */
    Version_t version;          /**< 版本号 */
    SensorConfig_t config;      /**< 配置 */
    DeviceStatusEnum_t status;  /**< 状态 */
} DeviceDescriptor_t;

/*============================================================================*/
/*                           演示函数声明                                       */
/*============================================================================*/

/**
 * @brief 结构体基础演示
 */
void demo_struct_basics(void);

/**
 * @brief 结构体指针演示
 */
void demo_struct_pointer(void);

/**
 * @brief 打印学生信息
 * @param[in] stu 学生结构体指针
 */
void print_student(const Student_t *stu);

/**
 * @brief 结构体数组演示
 */
void demo_struct_array(void);

/**
 * @brief 嵌套结构体演示
 */
void demo_nested_struct(void);

/**
 * @brief 联合体演示
 */
void demo_union(void);

/**
 * @brief 枚举类型演示
 */
void demo_enum(void);

/**
 * @brief 位域演示
 */
void demo_bit_field(void);

/**
 * @brief 内存对齐演示
 */
void demo_memory_alignment(void);

/**
 * @brief 综合应用演示
 */
void demo_comprehensive(void);

#ifdef __cplusplus
}
#endif

#endif
