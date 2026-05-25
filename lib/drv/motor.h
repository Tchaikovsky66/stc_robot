#ifndef _MOTOR_H
#define _MOTOR_H

#include "config.h"

/*=== 电机状态（ISR 共享，用 char 代替 __bit 避免 SDCC 跨文件问题） ===*/
extern volatile char motor_busy;
extern volatile char limit_hit;
extern volatile int pulse_count;

extern char homing_mode;
extern __data unsigned int x轴脉冲总值;
extern __data unsigned int y1轴脉冲总值;
extern __data unsigned int y2轴脉冲总值;

extern int 每10ms计数;
extern int 计算速度;
extern int 实时测量夹取位置_脉冲数;

/* ISR 访问 */
extern __data int _motor_target;
extern __data char _motor_axis;
extern __data int _motor_isr_cnt;

/*=== API ===*/
void motor_init(void);
void Timer1_10ms_Init(void);
char motor_go(char axis, char dir, int distance, int speed);
void 电机复位(void);

/*=== ISR 原型（必须声明在 main.c 可见的头文件中，SDCC 才会生成中断向量） ===*/
void timer0_isr(void) __interrupt(1);
void timer1_isr(void) __interrupt(3);

#endif
