#ifndef _GPIO_H
#define _GPIO_H

#include "stc15.h"

/*=== 电机控制引脚 (P0) ===*/
__sbit __at(0x80) X_DIR;     // P0.0  1=左, 0=右
__sbit __at(0x81) X_STEP;    // P0.1  脉冲输出
__sbit __at(0x82) X_EN;      // P0.2  0=使能, 1=不使能
__sbit __at(0x83) Y1_DIR;    // P0.3  方向
__sbit __at(0x84) Y1_STEP;   // P0.4  脉冲输出
__sbit __at(0x85) Y2_DIR;    // P0.5  方向
__sbit __at(0x86) Y2_STEP;   // P0.6  脉冲输出

/*=== 限位开关 & 传感器 (P1) ===*/
__sbit __at(0x93) X_R_LIMIT;       // P1.3  X轴右限位 (0=到达)
__sbit __at(0x94) X_L_LIMIT;       // P1.4  X轴左限位 (0=到达)
__sbit __at(0x95) X_MID_SENSOR;    // P1.5  X中间传感器
__sbit __at(0x96) PRODUCT_PRESENT; // P1.6  成品位置有工件
__sbit __at(0x97) RAW_PRESENT;     // P1.7  物料位置有工件

/*=== 加工设备接口 (P2) ===*/
__sbit __at(0xA0) MACHINE_HAS_PART; // P2.0  Y3 加工设备有无工件
__sbit __at(0xA1) MACHINE_ERROR;    // P2.1  Y2 加工设备异常
__sbit __at(0xA2) MACHINE_HOME;     // P2.2  Y1 加工设备原位置
__sbit __at(0xA3) MACHINE_RESET;    // P2.3  X1 加工设备复位
__sbit __at(0xA4) MACHINE_RUN;      // P2.4  X2 加工设备运行

/*=== Y轴限位 & 工件检测 (P3) ===*/
__sbit __at(0xB3) Y1_PART_DETECT;   // P3.3  Y1右工件检测
__sbit __at(0xB4) Y1_LIMIT;         // P3.4  Y1零位 (0=到达)
__sbit __at(0xB5) Y2_LIMIT;         // P3.5  Y2零位 (0=到达)
__sbit __at(0xB6) Y2_PART_DETECT;   // P3.6  Y2左工件检测

/*=== 通信 (P4) ===*/
__sbit __at(0xC1) EN_485;           // P4.1  RS485使能

/*=== 吸爪 (P4) ===*/
__sbit __at(0xC2) LEFT_GRIPPER;     // P4.2  Y2轴左吸爪
__sbit __at(0xC4) RIGHT_GRIPPER;    // P4.4  Y1轴右吸爪

#endif /* _GPIO_H */
