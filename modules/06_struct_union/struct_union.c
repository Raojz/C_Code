/**
 * @file struct_union.c
 * @brief 结构体与联合体模块实现
 * @details 演示C语言结构体与联合体的使用方法。
 *          本模块涵盖以下核心知识点：
 *          - 结构体定义与初始化
 *          - 结构体成员访问
 *          - 结构体指针
 *          - 结构体数组
 *          - 嵌套结构体
 *          - 联合体
 *          - 枚举类型
 *          - 位域
 *          - 内存对齐
 * 
 * @section embedded_importance 嵌入式开发重要性
 * - 结构体是数据组织的核心方式
 * - 联合体用于数据复用和协议解析
 * - 位域用于硬件寄存器映射
 * - 内存对齐影响性能和兼容性
 * 
 * @author Embedded C Learning Project
 * @date 2026-03-22
 * @version 1.0.0
 * 
 * @note 本文件遵循 MISRA-C:2012 编码规范
 */

#include "struct_union.h"
#include "common/common_utils.h"
#include <stdio.h>
#include <string.h>

/*============================================================================*/
/*                           结构体基础演示                                     */
/*============================================================================*/

/**
 * @brief 结构体基础演示
 * @details 展示结构体的基本概念：
 *          - 结构体定义
 *          - 结构体变量声明
 *          - 结构体初始化
 *          - 成员访问
 * 
 * @note 结构体是用户自定义的复合数据类型
 * @note 结构体成员在内存中按定义顺序存储
 */
void demo_struct_basics(void)
{
    print_separator("结构体基础演示");

    printf("[结构体定义]\n");
    printf("  typedef struct {\n");
    printf("      char name[32];\n");
    printf("      int32_t age;\n");
    printf("      float score;\n");
    printf("  } Student_t;\n\n");

    printf("[结构体初始化]\n");
    Student_t stu1 = {"张三", 20, 85.5f};
    Student_t stu2 = {
        .name = "李四",
        .age = 21,
        .score = 90.0f
    };
    
    printf("  方式1: Student_t stu1 = {\"张三\", 20, 85.5};\n");
    printf("  方式2: Student_t stu2 = {.name=\"李四\", .age=21, .score=90.0};\n");

    printf("\n[成员访问]\n");
    printf("  stu1.name = \"%s\"\n", stu1.name);
    printf("  stu1.age = %d\n", stu1.age);
    printf("  stu1.score = %.1f\n", stu1.score);

    printf("\n[结构体赋值]\n");
    Student_t stu3;
    stu3 = stu1;
    printf("  stu3 = stu1; // 结构体可以直接赋值\n");
    printf("  stu3.name = \"%s\"\n", stu3.name);

    printf("\n[结构体大小]\n");
    printf("  sizeof(Student_t) = %zu 字节\n", sizeof(Student_t));
    printf("  sizeof(stu1) = %zu 字节\n", sizeof(stu1));
}

/*============================================================================*/
/*                           结构体指针演示                                     */
/*============================================================================*/

/**
 * @brief 结构体指针演示
 * @details 展示结构体指针的使用：
 *          - 结构体指针声明
 *          - 通过指针访问成员
 *          - 结构体指针作为函数参数
 *          - 动态分配结构体
 * 
 * @note 使用->运算符通过指针访问成员
 * @note 结构体指针传递效率高于结构体值传递
 */
void demo_struct_pointer(void)
{
    print_separator("结构体指针演示");

    printf("[结构体指针声明与初始化]\n");
    Student_t stu = {"王五", 22, 88.0f};
    Student_t *ptr = &stu;
    
    printf("  Student_t stu = {\"王五\", 22, 88.0};\n");
    printf("  Student_t *ptr = &stu;\n\n");
    
    printf("  ptr->name = \"%s\"\n", ptr->name);
    printf("  ptr->age = %d\n", ptr->age);
    printf("  ptr->score = %.1f\n", ptr->score);

    printf("\n[两种访问方式]\n");
    printf("  (*ptr).name 等价于 ptr->name\n");
    printf("  (*ptr).age = %d\n", (*ptr).age);
    printf("  ptr->age = %d\n", ptr->age);

    printf("\n[结构体指针作为函数参数]\n");
    printf("  void print_student(const Student_t *stu);\n");
    printf("  优点: 避免结构体复制, 提高效率\n");
    print_student(&stu);

    printf("\n[动态分配结构体]\n");
    Student_t *dynamic_stu = (Student_t *)malloc(sizeof(Student_t));
    if (dynamic_stu != NULL) {
        strcpy(dynamic_stu->name, "赵六");
        dynamic_stu->age = 23;
        dynamic_stu->score = 92.5f;
        
        printf("  动态分配: %s, %d岁, %.1f分\n", 
               dynamic_stu->name, dynamic_stu->age, dynamic_stu->score);
        
        free(dynamic_stu);
        dynamic_stu = NULL;
    }
}

/**
 * @brief 打印学生信息
 * @param[in] stu 学生结构体指针
 */
void print_student(const Student_t *stu)
{
    if (stu != NULL) {
        printf("  学生: %s, %d岁, %.1f分\n", stu->name, stu->age, stu->score);
    }
}

/*============================================================================*/
/*                           结构体数组演示                                     */
/*============================================================================*/

/**
 * @brief 结构体数组演示
 * @details 展示结构体数组的使用：
 *          - 结构体数组声明与初始化
 *          - 遍历结构体数组
 *          - 结构体数组排序
 *          - 查找特定元素
 * 
 * @note 结构体数组常用于管理多个同类数据
 * @note 嵌入式中常用于设备表、配置表等
 */
void demo_struct_array(void)
{
    print_separator("结构体数组演示");

    printf("[结构体数组初始化]\n");
    Student_t class[] = {
        {"张三", 20, 85.5f},
        {"李四", 21, 90.0f},
        {"王五", 19, 78.5f},
        {"赵六", 22, 92.0f},
        {"钱七", 20, 88.0f}
    };
    size_t count = ARRAY_SIZE(class);
    
    printf("  班级人数: %zu\n", count);

    printf("\n[遍历结构体数组]\n");
    for (size_t i = 0; i < count; i++) {
        printf("  [%zu] %s, %d岁, %.1f分\n", i, class[i].name, class[i].age, class[i].score);
    }

    printf("\n[按成绩排序]\n");
    for (size_t i = 0; i < count - 1; i++) {
        for (size_t j = 0; j < count - i - 1; j++) {
            if (class[j].score < class[j + 1].score) {
                Student_t temp = class[j];
                class[j] = class[j + 1];
                class[j + 1] = temp;
            }
        }
    }
    
    printf("  排序后:\n");
    for (size_t i = 0; i < count; i++) {
        printf("  第%zu名: %s, %.1f分\n", i + 1, class[i].name, class[i].score);
    }

    printf("\n[嵌入式应用]\n");
    printf("  - 设备表: Device_t devices[MAX_DEVICES];\n");
    printf("  - 配置表: Config_t configs[MAX_CONFIGS];\n");
    printf("  - 传感器数据: SensorData_t sensor_data[MAX_SAMPLES];\n");
}

/*============================================================================*/
/*                           嵌套结构体演示                                     */
/*============================================================================*/

/**
 * @brief 嵌套结构体演示
 * @details 展示结构体嵌套的使用：
 *          - 结构体作为另一个结构体的成员
 *          - 多层嵌套
 *          - 访问嵌套成员
 *          - 实际应用场景
 * 
 * @note 嵌套结构体用于表示复杂数据结构
 * @note 嵌入式常用于描述硬件模块、通信协议等
 */
void demo_nested_struct(void)
{
    print_separator("嵌套结构体演示");

    printf("[嵌套结构体定义]\n");
    printf("  typedef struct { uint16_t x, y; } Point_t;\n");
    printf("  typedef struct {\n");
    printf("      Point_t start;\n");
    printf("      Point_t end;\n");
    printf("      uint32_t color;\n");
    printf("  } Line_t;\n\n");

    Line_t line = {
        .start = {10, 20},
        .end = {100, 200},
        .color = 0xFF0000
    };

    printf("[访问嵌套成员]\n");
    printf("  line.start.x = %d\n", line.start.x);
    printf("  line.start.y = %d\n", line.start.y);
    printf("  line.end.x = %d\n", line.end.x);
    printf("  line.end.y = %d\n", line.end.y);
    printf("  line.color = 0x%06X\n", line.color);

    printf("\n[嵌入式应用]\n");
    Device_t device = {
        .info = {"STM32F407", "v1.0.0", 2026},
        .status = {true, 25.5f, 0},
        .config = {1000, 9600, true}
    };
    
    printf("  设备: %s\n", device.info.name);
    printf("  版本: %s\n", device.info.version);
    printf("  状态: %s\n", device.status.is_running ? "运行中" : "停止");
    printf("  温度: %.1f°C\n", device.status.temperature);
    printf("  采样率: %d Hz\n", device.config.sample_rate);
}

/*============================================================================*/
/*                           联合体演示                                         */
/*============================================================================*/

/**
 * @brief 联合体演示
 * @details 展示联合体的使用：
 *          - 联合体定义与特点
 *          - 联合体大小
 *          - 联合体应用场景
 *          - 联合体与结构体对比
 * 
 * @note 联合体所有成员共享同一内存空间
 * @note 联合体大小等于最大成员的大小
 * @note 嵌入式常用于数据复用和协议解析
 */
void demo_union(void)
{
    print_separator("联合体演示");

    printf("[联合体定义]\n");
    printf("  typedef union {\n");
    printf("      uint8_t bytes[4];\n");
    printf("      uint16_t words[2];\n");
    printf("      uint32_t dword;\n");
    printf("  } DataConverter_t;\n\n");

    DataConverter_t data;
    data.dword = 0x12345678;

    printf("[联合体成员共享内存]\n");
    printf("  data.dword = 0x%08X\n", data.dword);
    printf("  data.bytes[0] = 0x%02X\n", data.bytes[0]);
    printf("  data.bytes[1] = 0x%02X\n", data.bytes[1]);
    printf("  data.bytes[2] = 0x%02X\n", data.bytes[2]);
    printf("  data.bytes[3] = 0x%02X\n", data.bytes[3]);
    printf("  data.words[0] = 0x%04X\n", data.words[0]);
    printf("  data.words[1] = 0x%04X\n", data.words[1]);

    printf("\n[联合体大小]\n");
    printf("  sizeof(DataConverter_t) = %zu (最大成员大小)\n", sizeof(DataConverter_t));
    printf("  sizeof(uint32_t) = %zu\n", sizeof(uint32_t));
    printf("  sizeof(uint8_t[4]) = %zu\n", sizeof(uint8_t[4]));

    printf("\n[联合体应用 - 协议数据]\n");
    ProtocolData_t proto;
    proto.raw = 0x12345678;
    
    printf("  原始数据: 0x%08X\n", proto.raw);
    printf("  字节0: 0x%02X\n", proto.byte0);
    printf("  字节1: 0x%02X\n", proto.byte1);
    printf("  字节2: 0x%02X\n", proto.byte2);
    printf("  字节3: 0x%02X\n", proto.byte3);

    printf("\n[联合体与结构体对比]\n");
    printf("  结构体: 成员各自占用内存, 大小为所有成员之和\n");
    printf("  联合体: 成员共享内存, 大小为最大成员\n");
    printf("  结构体用于组织数据, 联合体用于数据复用\n");
}

/*============================================================================*/
/*                           枚举类型演示                                       */
/*============================================================================*/

/**
 * @brief 枚举类型演示
 * @details 展示枚举类型的使用：
 *          - 枚举定义
 *          - 枚举值
 *          - 枚举在switch中的应用
 *          - 枚举与整数的关系
 * 
 * @note 枚举本质是整数常量
 * @note 枚举提高代码可读性
 * @note 嵌入式常用于状态机、命令定义
 */
void demo_enum(void)
{
    print_separator("枚举类型演示");

    printf("[枚举定义]\n");
    printf("  typedef enum {\n");
    printf("      STATE_IDLE = 0,\n");
    printf("      STATE_RUNNING = 1,\n");
    printf("      STATE_PAUSED = 2,\n");
    printf("      STATE_ERROR = 3\n");
    printf("  } SystemState_t;\n\n");

    SystemState_t state = STATE_RUNNING;
    
    printf("[枚举使用]\n");
    printf("  当前状态: %d\n", state);
    
    switch (state) {
        case STATE_IDLE:
            printf("  系统空闲\n");
            break;
        case STATE_RUNNING:
            printf("  系统运行中\n");
            break;
        case STATE_PAUSED:
            printf("  系统暂停\n");
            break;
        case STATE_ERROR:
            printf("  系统错误\n");
            break;
        default:
            printf("  未知状态\n");
            break;
    }

    printf("\n[枚举与整数]\n");
    printf("  STATE_IDLE = %d\n", STATE_IDLE);
    printf("  STATE_RUNNING = %d\n", STATE_RUNNING);
    printf("  STATE_PAUSED = %d\n", STATE_PAUSED);
    printf("  STATE_ERROR = %d\n", STATE_ERROR);
    printf("  枚举本质是整数常量\n");

    printf("\n[嵌入式应用]\n");
    printf("  - 状态机状态定义\n");
    printf("  - 错误码定义\n");
    printf("  - 命令类型定义\n");
    printf("  - 设备类型定义\n");
}

/*============================================================================*/
/*                           位域演示                                           */
/*============================================================================*/

/**
 * @brief 位域演示
 * @details 展示位域的使用：
 *          - 位域定义
 *          - 位域大小
 *          - 位域应用
 *          - 位域注意事项
 * 
 * @note 位域用于精确控制成员占用的位数
 * @note 嵌入式常用于硬件寄存器映射
 * @note 位域的内存布局依赖于编译器
 */
void demo_bit_field(void)
{
    print_separator("位域演示");

    printf("[位域定义]\n");
    printf("  typedef struct {\n");
    printf("      uint32_t enable : 1;\n");
    printf("      uint32_t mode : 2;\n");
    printf("      uint32_t speed : 3;\n");
    printf("      uint32_t reserved : 26;\n");
    printf("  } ConfigReg_t;\n\n");

    ConfigReg_t config = {0};
    config.enable = 1;
    config.mode = 2;
    config.speed = 5;

    printf("[位域访问]\n");
    printf("  enable = %u (1位)\n", config.enable);
    printf("  mode = %u (2位, 范围0-3)\n", config.mode);
    printf("  speed = %u (3位, 范围0-7)\n", config.speed);

    printf("\n[位域大小]\n");
    printf("  sizeof(ConfigReg_t) = %zu 字节\n", sizeof(ConfigReg_t));
    printf("  位域总大小: 1+2+3+26 = 32位 = 4字节\n");

    printf("\n[嵌入式应用 - 硬件寄存器映射]\n");
    printf("  typedef struct {\n");
    printf("      volatile uint32_t ENABLE : 1;\n");
    printf("      volatile uint32_t MODE : 2;\n");
    printf("      volatile uint32_t SPEED : 3;\n");
    printf("      volatile uint32_t : 26; // 保留位\n");
    printf("  } HardwareReg_t;\n");
    printf("  HardwareReg_t *reg = (HardwareReg_t*)0x40000000;\n");

    printf("\n[注意事项]\n");
    printf("  - 位域成员不能取地址\n");
    printf("  - 位域布局依赖编译器\n");
    printf("  - 跨平台代码应避免使用位域\n");
    printf("  - 建议使用位操作替代位域\n");
}

/*============================================================================*/
/*                           内存对齐演示                                       */
/*============================================================================*/

/**
 * @brief 内存对齐演示
 * @details 展示内存对齐的概念：
 *          - 内存对齐原因
 *          - 结构体对齐规则
 *          - sizeof计算
 *          - 紧凑结构体
 * 
 * @note 内存对齐提高CPU访问效率
 * @note 结构体大小可能大于成员大小之和
 * @note #pragma pack用于控制对齐
 */
void demo_memory_alignment(void)
{
    print_separator("内存对齐演示");

    printf("[内存对齐原因]\n");
    printf("  - 提高CPU访问内存效率\n");
    printf("  - 某些CPU要求特定类型必须对齐\n");
    printf("  - 未对齐访问可能导致异常或性能下降\n");

    printf("\n[结构体对齐示例]\n");
    typedef struct {
        uint8_t a;    // 1字节
        // 3字节填充
        uint32_t b;   // 4字节
        uint8_t c;    // 1字节
        // 3字节填充
    } Unaligned_t;
    
    printf("  struct { uint8_t a; uint32_t b; uint8_t c; }\n");
    printf("  sizeof = %zu 字节 (包含填充)\n", sizeof(Unaligned_t));
    printf("  成员大小之和: 1+4+1 = 6字节\n");
    printf("  实际大小: %zu字节 (对齐到4字节边界)\n", sizeof(Unaligned_t));

    printf("\n[紧凑结构体]\n");
#pragma pack(push, 1)
    typedef struct {
        uint8_t a;    // 1字节
        uint32_t b;   // 4字节
        uint8_t c;    // 1字节
    } Packed_t;
#pragma pack(pop)
    
    printf("  #pragma pack(1)\n");
    printf("  struct { uint8_t a; uint32_t b; uint8_t c; }\n");
    printf("  sizeof = %zu 字节 (无填充)\n", sizeof(Packed_t));

    printf("\n[对齐规则]\n");
    printf("  1. 结构体起始地址对齐到最大成员的对齐值\n");
    printf("  2. 每个成员相对于结构体起始地址的偏移量必须是对齐值的整数倍\n");
    printf("  3. 结构体总大小必须是最宽成员对齐值的整数倍\n");

    printf("\n[嵌入式应用]\n");
    printf("  - 通信协议: 使用紧凑结构体避免填充\n");
    printf("  - 硬件寄存器: 需要严格对齐\n");
    printf("  - 性能优化: 合理排列成员减少填充\n");
}

/*============================================================================*/
/*                           综合应用演示                                       */
/*============================================================================*/

/**
 * @brief 综合应用演示
 * @details 展示结构体、联合体、枚举的综合应用：
 *          - 复杂数据结构设计
 *          - 协议数据包定义
 *          - 设备描述符
 * 
 * @note 综合运用多种数据类型解决实际问题
 * @note 嵌入式开发中常见的设计模式
 */
void demo_comprehensive(void)
{
    print_separator("综合应用演示");

    printf("[设备描述符]\n");
    DeviceDescriptor_t dev = {
        .device_id = 0x1234,
        .device_type = DEVICE_TYPE_SENSOR,
        .version = {1, 0, 0},
        .config = {
            .sample_rate = 1000,
            .resolution = 12,
            .mode = SENSOR_MODE_CONTINUOUS
        },
        .status = DEVICE_STATUS_OK
    };
    
    printf("  设备ID: 0x%04X\n", dev.device_id);
    printf("  设备类型: %d\n", dev.device_type);
    printf("  版本: %d.%d.%d\n", dev.version.major, dev.version.minor, dev.version.patch);
    printf("  采样率: %d Hz\n", dev.config.sample_rate);
    printf("  分辨率: %d 位\n", dev.config.resolution);
    printf("  状态: %d\n", dev.status);

    printf("\n[设计原则]\n");
    printf("  1. 使用枚举定义状态和类型\n");
    printf("  2. 使用结构体组织相关数据\n");
    printf("  3. 使用联合体实现数据复用\n");
    printf("  4. 使用位域映射硬件寄存器\n");
    printf("  5. 注意内存对齐和填充\n");
}
