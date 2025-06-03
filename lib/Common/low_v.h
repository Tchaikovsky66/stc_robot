__sbit __at(0xAE) ELVD; // 低压检测中断使能位

#define LVDF 0x20 // PCON.5,低压检测中断标志位

void LVD_ISR(void) __interrupt(6);
void LVD_Init(void);
