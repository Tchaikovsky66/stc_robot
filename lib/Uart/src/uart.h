#ifndef _UART_H
#define _UART_H

#include <8051.h>

extern __xdata volatile char rx_buffer[];		//uart 接收缓存buffer
extern volatile unsigned char rx_index;		//uart接收计数
extern volatile __bit string_received_flag;		//uart接收标志位

void UART_SendChar(char);
void UART_SendString(char *str);
void UART_Init(void);
void UART_ISR(void) __interrupt (4);

#endif // !_UART_H