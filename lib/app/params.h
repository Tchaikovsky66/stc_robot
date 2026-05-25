#ifndef _PARAMS_H
#define _PARAMS_H

#include "config.h"

/*=== 迪文屏标志位（从 CFGBUF[0x14*2] 解析） ===*/
extern __bit 测试运行;
extern __bit 保存工件名称;
extern __bit 开始调试;
extern __bit 复位并运行;
extern __bit 参数调试;
extern __bit 恢复出厂设置标志位;
extern __bit 保存;
extern __bit 自动端面检测;
extern __bit 实时测量夹取位置;
extern __bit 模拟来料;

/*=== 运动参数 ===*/
extern __xdata int 两端上下距离;
extern __xdata int 加工位置上下距离;
extern __xdata int 未加工到加工距离;
extern __xdata int 未加工到结束距离;
extern __xdata int 未加工到右限位距离;
extern __xdata int 左右速度;
extern __xdata int 上下速度;
extern __xdata int 加减速系数;
extern __xdata int 夹取位置;
extern __xdata int 铁芯直径;
extern __xdata int 等待处位置;
extern __xdata int F20;
extern __xdata int E80;
extern __xdata int H170;
extern volatile __xdata int 工件序号;

/*=== 状态/显示变量（非 ISR 共享的） ===*/
extern int 当前X位置;
extern int 当前Y位置;
extern int 计算速度;
extern __xdata int 开机初始化bar;

/*=== 图标 ===*/
extern int 图标标志位;
extern char 图标标志位H;
extern char 图标标志位L;

/*=== 工件名称 ===*/
extern __xdata char 工件名称[8];
extern __xdata char 型号1[8];
extern __xdata char 型号2[8];
extern __xdata char 型号3[8];
extern __xdata char 型号4[8];
extern __xdata char 型号5[8];
extern __xdata char 型号6[8];
extern __xdata char 型号7[8];
extern __xdata char 型号8[8];

#endif /* _PARAMS_H */
