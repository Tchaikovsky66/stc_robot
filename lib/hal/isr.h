#ifndef _ISR_H
#define _ISR_H

/*=== 外部中断初始化 ===*/
void Interrupt0_Init(void);  /* P3.2 急停按钮 */
void LVD_Init(void);         /* 低压检测中断 */

/*=== ISR 原型（必须声明在 main.c 可见的头文件中，SDCC 才会生成中断向量） ===*/
void int0_isr(void) __interrupt(0);
void lvd_isr(void) __interrupt(6);

#endif /* _ISR_H */
