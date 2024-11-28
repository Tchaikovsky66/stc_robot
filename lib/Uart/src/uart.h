#ifndef _UART_H
#define _UART_H

#include <8051.h>

#define FRAME_HEADER1 0x5A
#define FRAME_HEADER2 0xA5
#define DATA_LENGTH 0x04
#define COMMAND 0x80
#define UART_BUFFER_SIZE 16

extern __xdata volatile char rx_buffer[];		//uart 接收缓存buffer
extern volatile unsigned char rx_cnt;		//uart接收计数
extern volatile __bit string_received_flag;		//uart接收标志位
extern volatile __bit all_data_flag;

extern __xdata unsigned char CFGBUF[100]; //接收到的数据缓存
extern __xdata unsigned char RCVOK;

extern volatile int model;
extern volatile int core_diameter;
extern volatile int end_face_distance;
extern volatile int up_down_speed;
extern volatile int up_down_distance;
extern volatile int left_right_speed;
extern volatile int left_right_distance;

void UART_SendByte(unsigned char byte);
void UART2_SendByte(unsigned char byte);
void UART_SendString(unsigned char *str);
void UART2_SendString(unsigned char *str);   
void UART_Init(void);
void UART2_Init(void);
void UART_ISR(void) __interrupt (4);
unsigned int HexToDec(char *hex);
void UART_SendFrame(unsigned char address, unsigned char data1, unsigned char data2);
void get_dwin_data(void);
void delete_data_flag(void);

#endif // !_UART_H