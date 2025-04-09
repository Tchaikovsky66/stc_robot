#ifndef _MOTOR_H
#define _MOTOR_H

// 变量声明 - SDCC 方式
// extern __data int inter_num;
extern __data int 当前x轴最大脉冲数;
extern __data unsigned int x轴脉冲总值;
extern __data volatile unsigned int 当前x轴脉冲数;
extern volatile __bit x_flag; // bit 变量

// 速度表声明
extern __code const unsigned int 速度表[101];

// 函数声明
void Timer1_10ms_Init(void);
void motor_init(void);
char motor_go(char num, char 方向, int 距离, int 速度);
void 电机复位(void);

// 添加中断函数声明
void timer0_isr(void) __interrupt(1);
void timer1_isr(void) __interrupt(3);

#endif // !_MOTOR_H
