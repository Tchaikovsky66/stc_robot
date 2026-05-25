#ifndef _UART_H
#define _UART_H

#include "config.h"

/*=== CFGBUF 和型号数组（全局共享） ===*/
extern volatile unsigned char __xdata CFGBUF[CFGBUF_SIZE];
extern volatile unsigned char __xdata 型号数组[MODEL_BUF_SIZE];
extern volatile char 接收到数据;

/*=== ISR 内部状态 ===*/
extern __xdata unsigned char RX5A, RXA5, RXLEN, RXCMD, RXADRH, RXADRL, RXDLEN;
extern __xdata unsigned int RX_P;
extern volatile char rx_mode;

/*=== 函数声明 ===*/
void Uart1_Init(void);
char Uart1_SendByte(char byte);
void Uart1_SendBuffer(const unsigned char *buffer, unsigned int length);
void Uart1_SendString(const char *str);

/*=== ISR 原型（必须声明在 main.c 可见的头文件中，SDCC 才会生成中断向量） ===*/
void Uart1_Isr(void) __interrupt(4);

#endif /* _UART_H */
