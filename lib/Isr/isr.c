#include <isr.h>
#include <delay.h>
#include <../../include/stc15.h> 
#include <../../include/main.h>
extern volatile __bit interruptButtonFlag = 0; //按键中断

//初始化按键中断
void Interrupt0_Init(void)
{
	IT0 = 0;  // 0低电平触发 1高电平触发
    EX0 = 1;  // 使能外部中断 0
    EA = 1;   // 使能全局中断
}
void int0_restart(void)
{
    if (P32 == 0) // 检测按键是否按下
    {
        DelayMs(2);   // 消抖
        if (P32 == 0) // 再次检测按键是否按下
        {
            IAP_CONTR = 0x20; // 重启
        }
    }
}

/*INT0	P3^2*/
void my_ISR(void) __interrupt (0) 
{

    int0_restart();
}
