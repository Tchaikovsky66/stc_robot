# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## 项目概述

三轴架式机械手控制系统，基于 STC15W4K48S4 (8051架构) 单片机，用于转子精车机/开槽机的上下料。使用 SDCC 编译器通过 PlatformIO 构建。

## 构建命令

```bash
pio run                # 编译
pio run -t upload      # 烧录（通过串口/STC-ISP）
pio run -t clean       # 清理构建产物
```

## 架构

代码使用中文标识符（变量名、函数名），这是项目约定，修改代码时应保持一致。

### 核心数据流

迪文触摸屏 <--UART1--> STC15 MCU <--脉冲/方向--> 步进电机驱动器

MCU 同时通过 P2 端口的数字IO与加工设备通信（X1/X2输出，Y1/Y2/Y3输入）。

### 关键模块

- `src/main.c` — 主循环，基于标志位的事件处理（保存、复位并运行、参数调试、测试运行等）
- `lib/Motor/motor.c` — 步进电机底层驱动，Timer0 产生脉冲，使用预计算的 S曲线速度表 `速度表[101]` 实现加减速
- `lib/Motor/position.c` — 三轴坐标运动控制（`MoveToPosition`），运行/调试流程
- `lib/Uart/uart.c` — UART1 收发，接收迪文屏数据帧到 `CFGBUF[96]` 缓冲区
- `lib/Uart/dwin.c` — 迪文屏协议封装（读写变量、切页、数据同步）
- `lib/Common/common.c` — GPIO 定义、图标标志位管理、所有运动参数的 extern 变量
- `lib/Common/eeprom.c` — STC15 IAP/EEPROM 读写，参数持久化
- `lib/Isr/isr.c` — 外部中断0（P3.2急停）

### 内存布局

- `CFGBUF[96]` (`__xdata`) — 运动参数缓冲区，地址以 `0xNN * 2` 格式定义在 `common.h`，与迪文屏变量地址对应
- `型号数组[0x88]` (`__xdata`) — 8种工件型号名称（GBK编码），每种16字节
- EEPROM `0x2400` — 参数数据区，`0x2600` — 型号数据区
- 首次启动标记：EEPROM `0x0000` 处为 `0xCC` 表示已初始化

### 迪文屏通信

帧格式：`5A A5 len cmd addr data`。主要命令：`0x82`写变量、`0x83`读变量、`0x80`写寄存器（切页）。屏幕通过 UART1 触发标志位（`保存`、`复位并运行`、`参数调试`等），主循环轮询处理。

### 电机控制

三轴：X（水平左右）、Y1（右侧上下+夹爪）、Y2（左侧上下+夹爪）。步进电机 1.8°/步 × 10细分 = 2000步/圈，100mm/圈。S曲线加减速：加速100步 + 匀速 + 减速100步，短距离对称处理。

## 编码约定

- 编译器为 SDCC，使用 `__sfr`、`__sbit`、`__xdata`、`__data`、`__code`、`__bit`、`__interrupt` 等关键字
- 引脚映射在 `include/main.h`（电机控制引脚）和 `lib/Common/common.h`（传感器/IO引脚）中用 `__sbit __at(addr)` 定义
- STC15 寄存器定义在 `include/stc15.h`
- 限位开关逻辑：0 = 到达限位，1 = 未到达
- 电机使能逻辑：0 = 使能，1 = 不使能
