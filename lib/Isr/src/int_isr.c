#include <int_isr.h>

extern volatile __bit interruptButtonFlag = 0; //按键中断

//中断0初始化 用作按键中断 INT0	P3^2
void Interrupt0_Init(void)
{
	IT0 = 0;  // 0下降沿触发中断 1都可
    EX0 = 1;  // 使能外部中断 0
    EA = 1;   // 使能全局中断
}

/*按键中断 INT0	P3^2*/
void my_ISR(void) __interrupt (0) 
{
    interruptButtonFlag = 1;  // 设置标志，表示按键已按下
}