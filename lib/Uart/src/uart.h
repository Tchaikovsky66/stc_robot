#ifndef _UART_H
#define _UART_H

#include <8051.h>

#define FRAME_HEADER1 0x5A
#define FRAME_HEADER2 0xA5
#define DATA_LENGTH 0x04
#define COMMAND 0x80
#define UART_BUFFER_SIZE 10

extern __xdata volatile char rx_buffer[];		//uart 接收缓存buffer
extern volatile unsigned char rx_cnt;		//uart接收计数
extern volatile __bit string_received_flag;		//uart接收标志位

void UART_SendByte(char);
void UART_SendString(char *str);
void UART_Init(void);
void UART_ISR(void) __interrupt (4);
unsigned int HexToDec(char *hex);
void UART_SendFrame(unsigned char address, unsigned char data1, unsigned char data2);

#endif // !_UART_H