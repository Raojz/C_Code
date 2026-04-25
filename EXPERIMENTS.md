# 嵌入式C语言学习工程 - 实验设计

## 目录

1. [实验概述](#实验概述)
2. [基础实验](#基础实验)
3. [进阶实验](#进阶实验)
4. [综合实验](#综合实验)
5. [实验评估标准](#实验评估标准)

---

## 实验概述

本实验设计旨在通过一系列渐进式的实践任务，帮助学习者深入理解C语言的核心概念和嵌入式开发技能。每个实验都包含具体的目标、任务步骤和预期成果。

### 实验目标
- 掌握C语言的基本语法和概念
- 理解嵌入式开发的特性
- 培养问题分析和解决能力
- 提高代码质量和调试能力

### 实验环境
- **操作系统**：Windows/Linux/macOS
- **开发工具**：Visual Studio/Code::Blocks/GCC
- **硬件要求**：STM32开发板（可选，用于串口通信实验）
- **调试工具**：GDB/调试器

### 实验流程
1. **准备阶段**：阅读相关理论知识
2. **设计阶段**：设计解决方案
3. **编码阶段**：实现代码
4. **调试阶段**：测试和修复错误
5. **评估阶段**：评估实验结果

---

## 基础实验

### 实验1：温度转换程序

#### 实验目标
- 掌握变量的声明和使用
- 理解算术运算符
- 学会基本的输入输出

#### 实验任务
1. 编写程序实现摄氏度与华氏度的转换
2. 从用户输入读取摄氏温度
3. 计算华氏温度：F = C × 9/5 + 32
4. 输出转换结果，保留两位小数

#### 代码框架
```c
#include <stdio.h>

int main() {
    float celsius, fahrenheit;
    
    // 输入摄氏温度
    printf("请输入摄氏温度: ");
    scanf("%f", &celsius);
    
    // 转换计算
    fahrenheit = celsius * 9.0f / 5.0f + 32.0f;
    
    // 输出结果
    printf("%.2f°C = %.2f°F\n", celsius, fahrenheit);
    
    return 0;
}
```

#### 实验要求
- 正确处理输入输出
- 确保计算精度
- 添加适当的注释

#### 扩展任务
- 添加温度范围检查（-273.15°C以上）
- 支持批量转换（输入多个温度值）
- 添加图形化显示（使用字符画）

---

### 实验2：学生成绩管理系统

#### 实验目标
- 掌握数组和结构体的使用
- 理解循环和条件语句
- 学会数据处理和排序

#### 实验任务
1. 定义学生结构体（学号、姓名、成绩）
2. 实现以下功能：
   - 添加学生信息
   - 显示所有学生
   - 按成绩排序
   - 查找学生
   - 计算平均成绩

#### 代码框架
```c
#include <stdio.h>
#include <string.h>

#define MAX_STUDENTS 100

typedef struct {
    int id;
    char name[50];
    float score;
} Student;

Student students[MAX_STUDENTS];
int count = 0;

void add_student() {
    // 实现添加学生功能
}

void display_students() {
    // 实现显示学生功能
}

void sort_students() {
    // 实现按成绩排序
}

void search_student() {
    // 实现查找学生功能
}

void calculate_average() {
    // 实现计算平均成绩
}

int main() {
    int choice;
    
    do {
        printf("\n学生成绩管理系统\n");
        printf("1. 添加学生\n");
        printf("2. 显示学生\n");
        printf("3. 排序学生\n");
        printf("4. 查找学生\n");
        printf("5. 计算平均分\n");
        printf("0. 退出\n");
        printf("请选择: ");
        scanf("%d", &choice);
        
        switch(choice) {
            // 调用相应函数
        }
    } while(choice != 0);
    
    return 0;
}
```

#### 实验要求
- 使用结构体存储学生信息
- 实现完整的菜单系统
- 支持基本的CRUD操作
- 数据持久化（保存到文件）

#### 扩展任务
- 添加删除学生功能
- 支持按姓名查找
- 实现多科目成绩管理
- 添加数据验证

---

### 实验3：迷宫游戏

#### 实验目标
- 掌握二维数组的使用
- 理解递归算法
- 学会文件操作

#### 实验任务
1. 实现一个迷宫游戏
2. 从文件读取迷宫地图
3. 使用递归算法实现寻路
4. 实现玩家移动功能

#### 代码框架
```c
#include <stdio.h>
#include <stdlib.h>

#define SIZE 10

char maze[SIZE][SIZE];
int visited[SIZE][SIZE];

void load_maze(const char *filename) {
    // 从文件加载迷宫
}

void print_maze() {
    // 打印迷宫
}

int solve_maze(int x, int y) {
    // 递归解决迷宫
}

int main() {
    load_maze("maze.txt");
    print_maze();
    
    if (solve_maze(0, 0)) {
        printf("找到路径！\n");
        print_maze();
    } else {
        printf("没有路径！\n");
    }
    
    return 0;
}
```

#### 实验要求
- 迷宫格式（0表示通路，1表示墙壁）
- 支持不同的迷宫大小
- 递归实现深度优先搜索
- 显示解决路径

#### 扩展任务
- 使用广度优先搜索
- 添加时间限制
- 实现自动生成迷宫
- 支持图形化界面

---

## 进阶实验

### 实验4：简单的操作系统内核

#### 实验目标
- 理解内存管理
- 掌握进程调度
- 学习中断处理

#### 实验任务
1. 实现一个简单的任务调度器
2. 实现内存分配器
3. 添加基本的系统调用

#### 代码结构
```c
// 任务控制块
typedef struct {
    int id;
    void (*task_func)();
    int priority;
    int state;
} TaskControlBlock;

// 内存管理
void *malloc(size_t size);
void free(void *ptr);

// 进程调度
void schedule();
void create_task(void (*func)(), int priority);

// 系统调用
void system_call(int call, void *arg);
```

#### 实验要求
- 实现基本的任务调度
- 支持多任务并发
- 内存池管理
- 最少实现3个系统调用

#### 扩展任务
- 添加互斥锁
- 实现信号量
- 添加文件系统
- 支持实时任务

---

### 实验5：串口通信测试

#### 实验目标
- 掌握UART通信
- 理解中断处理
- 学会协议解析

#### 实验任务
1. 实现基本的UART收发
2. 实现中断接收
3. 解析简单的协议
4. 实现数据校验

#### 硬件连接
- STM32 UART1 → USB转串口模块
- PC → 串口调试助手

#### 代码框架
```c
#include "stm32f4xx.h"

// UART初始化
void uart_init(uint32_t baudrate);

// 发送数据
void uart_send(uint8_t data);

// 接收中断
void USART1_IRQHandler(void);

// 协议解析
void parse_protocol(uint8_t *data, uint16_t len);

int main() {
    uart_init(115200);
    
    while(1) {
        // 主循环
    }
}
```

#### 实验要求
- 实现环形缓冲区
- 支持中断接收
- 实现简单的数据帧协议
- 添加CRC校验

#### 扩展任务
- 支持DMA传输
- 实现Modbus协议
- 添加自动重连
- 支持多路UART

---

### 实验6：I2C设备驱动

#### 实验目标
- 掌握I2C协议
- 理解设备寻址
- 学会驱动开发

#### 实验任务
1. 实现I2C主机驱动
2. 驱动EEPROM芯片
3. 实现数据读写
4. 添加错误处理

#### 硬件要求
- STM32开发板
- EEPROM芯片（如24C02）
- 面包板和导线

#### 代码框架
```c
// I2C初始化
void i2c_init(void);

// I2C写操作
uint8_t i2c_write(uint8_t addr, uint8_t *data, uint16_t len);

// I2C读操作
uint8_t i2c_read(uint8_t addr, uint8_t *data, uint16_t len);

// EEPROM操作
uint8_t eeprom_write(uint16_t addr, uint8_t data);
uint8_t eeprom_read(uint16_t addr, uint8_t *data);

int main() {
    i2c_init();
    
    // 写入数据
    eeprom_write(0, 0xAA);
    
    // 读取数据
    uint8_t data;
    eeprom_read(0, &data);
    
    return 0;
}
```

#### 实验要求
- 实现基本的I2C协议
- 支持地址自动递增
- 添加超时处理
- 实现重试机制

#### 扩展任务
- 驱动温度传感器
- 实现多设备扫描
- 添加时钟拉伸
- 支持快速模式

---

## 综合实验

### 实验7：智能家居控制系统

#### 实验目标
- 综合运用所学知识
- 理解系统架构设计
- 掌握模块化编程

#### 系统功能
1. 传感器数据采集
   - 温度传感器（I2C）
   - 湿度传感器（I2C）
   - 光照传感器（ADC）
2. 执行器控制
   - LED控制（GPIO）
   - 继电器控制
   - 舵机控制（PWM）
3. 通信模块
   - WiFi模块（UART）
   - 本地显示（LCD）
4. 数据处理
   - 数据滤波
   - 阈值判断
   - 历史记录

#### 硬件架构
```
STM32F4
├── I2C1: 温湿度传感器
├── I2C2: EEPROM
├── UART1: WiFi模块
├── UART2: LCD显示
├── ADC: 光照传感器
├── GPIO: LED/继电器
└── TIM: PWM/舵机
```

#### 软件架构
```
main
├── sensor.c/h
├── actuator.c/h
├── communication.c/h
├── data_process.c/h
├── config.c/h
└── task.c/h
```

#### 实验要求
- 模块化设计
- 完整的功能实现
- 良好的错误处理
- 实时响应能力

#### 扩展任务
- 添加Web界面
- 实现远程控制
- 添加数据统计
- 支持OTA升级

---

### 实验8：RTOS应用开发

#### 实验目标
- 掌握实时操作系统
- 理解任务管理
- 学会资源同步

#### 系统设计
1. 任务划分
   - 传感器采集任务（高优先级）
   - 数据处理任务（中优先级）
   - 通信任务（中优先级）
   - 用户界面任务（低优先级）
2. 资源管理
   - 信号量
   - 队列
   - 互斥锁
3. 调度策略
   - 优先级调度
   - 时间片轮转
   - 空闲任务

#### 代码框架
```c
// 任务定义
#define TASK_SENSOR_STACK_SIZE 256
#define TASK_DATA_STACK_SIZE 256
#define TASK_COMM_STACK_SIZE 256

// 任务优先级
#define TASK_PRIORITY_SENSOR 1
#define TASK_PRIORITY_DATA 2
#define TASK_PRIORITY_COMM 3
#define TASK_PRIORITY_UI 4

// 信号量
SemaphoreHandle_t xSensorSemaphore;
SemaphoreHandle_t xDataSemaphore;

// 队列
QueueHandle_t xDataQueue;

void vSensorTask(void *pvParameters) {
    while(1) {
        // 读取传感器数据
        // 释放信号量
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void vDataTask(void *pvParameters) {
    while(1) {
        // 获取信号量
        // 处理数据
        // 发送到队列
    }
}

void vCommTask(void *pvParameters) {
    while(1) {
        // 从队列接收数据
        // 发送到远程
    }
}

int main() {
    // 创建任务
    xTaskCreate(vSensorTask, "Sensor", TASK_SENSOR_STACK_SIZE, NULL, 
                TASK_PRIORITY_SENSOR, NULL);
    
    // 启动调度器
    vTaskStartScheduler();
    
    return 0;
}
```

#### 实验要求
- 使用FreeRTOS
- 实现多任务管理
- 正确使用同步机制
- 系统稳定运行

#### 扩展任务
- 实现任务监控
- 添加看门狗
- 支持动态优先级
- 实现任务挂起/恢复

---

## 实验评估标准

### 代码质量（40%）
1. **代码规范**（10分）
   - 遵循编码规范
   - 命名清晰
   - 注释完整

2. **结构设计**（15分）
   - 模块化设计
   - 接口清晰
   - 可扩展性好

3. **错误处理**（15分）
   - 异常处理完善
   - 资源管理正确
   - 日志记录完整

### 功能实现（40%）
1. **基本功能**（20分）
   - 完成所有基本要求
   - 功能正确
   - 运行稳定

2. **扩展功能**（10分）
   - 实现扩展任务
   - 功能丰富
   - 创新性

3. **性能表现**（10分）
   - 响应及时
   - 资源占用合理
   - 效率高

### 文档报告（20%）
1. **实验报告**（10分）
   - 结构清晰
   - 内容完整
   - 分析深入

2. **代码注释**（5分）
   - 注释充分
   - 解释清楚
   - 可读性好

3. **演示效果**（5分）
   - 演示流畅
   - 交互友好
   - 界面美观

### 加分项
- 代码获奖（+5分）
- 创新突出（+5分）
- 性能优化（+3分）
- 社区贡献（+3分）

---

## 实验注意事项

### 安全注意事项
1. **硬件安全**
   - 正确连接电路
   - 注意电压等级
   - 防止短路

2. **软件安全**
   - 检查输入合法性
   - 防止缓冲区溢出
   - 避免死循环

### 开发建议
1. **增量开发**
   - 先实现核心功能
   - 逐步添加功能
   - 保持代码整洁

2. **测试策略**
   - 单元测试
   - 集成测试
   - 系统测试

3. **版本控制**
   - 使用Git管理代码
   - 定期提交代码
   - 记录变更日志

### 调试技巧
1. **调试工具**
   - 使用调试器
   - 打印日志
   - 静态分析

2. **常见问题**
   - 内存泄漏
   - 竞态条件
   - 死锁

---

## 实验总结

通过这些实验，学习者将能够：
1. 掌握C语言的核心概念
2. 理解嵌入式开发的特点
3. 培养问题解决能力
4. 提高代码质量意识

每个实验都设计得循序渐进，从简单的控制台程序到复杂的嵌入式系统。建议学习者：
- 认真准备实验内容
- 独立完成实验任务
- 善于总结经验
- 持续改进代码

祝实验顺利！