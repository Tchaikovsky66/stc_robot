#include "isr.h"
#include "stc15.h"
#include "delay.h"
#include "gpio.h"
#include "dwin.h"

/*=== INT0 (P3.2) 急停按钮 ===*/
void Interrupt0_Init(void)
{
    IT0 = 0;  /* 低电平触发 */
    EX0 = 1;  /* 使能外部中断 0 */
    EA  = 1;  /* 使能全局中断 */
}

/*=== 低压检测 ===*/
void LVD_Init(void)
{
    PCON &= ~0x20;  /* 先清 LVDF 标志 */
    ELVD = 1;       /* 使能低压中断 */
    EA   = 1;
}

/*=== INT0 ISR：按住 P3.2 重启系统 ===*/
void int0_isr(void) __interrupt(0)
{
    if (P32 == 0)
    {
        DelayMs(2);
        if (P32 == 0)
            IAP_CONTR = 0x20;  /* 软复位到 AP 区 */
    }
}

/*=== LVD ISR：低压切到页面60，停机 ===*/
void lvd_isr(void) __interrupt(6)
{
    PCON &= ~0x20;  /* 清 LVDF 标志 */
    GoToPage(60);
    while (1)
        ;
}
