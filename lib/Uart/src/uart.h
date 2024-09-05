#ifndef _UART_H
#define _UART_H

#include <8051.h>

#define motor_left 1
#define motor_right 0
#define y1_up 0
#define y1_down 1
#define y2_up 1
#define y2_down 0

extern __xdata volatile char rx_buffer[];		//uart 接收缓存buffer
extern volatile unsigned char rx_cnt;		//uart接收计数
extern volatile __bit string_received_flag;		//uart接收标志位

void UART_SendChar(char);
void UART_SendString(char *str);
void UART_Init(void);
void UART_ISR(void) __interrupt (4);

#endif // !_UART_H