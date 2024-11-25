#ifndef _UART_H
#define _UART_H

extern __xdata unsigned char CFGBUF[32]; // 接收到的数据缓存
extern volatile int model;
extern volatile int core_diameter;
extern volatile int end_face_distance;
extern volatile int up_down_speed;
extern volatile int up_down_distance;
extern volatile int left_right_speed;
extern volatile int left_right_distance;

int update_parameters(void);
void SendAllData(void);
void GetAllData(void);
void GoToPage(unsigned char );

void Uart1_Isr(void) __interrupt(4);    //串口1中断
void Uart1_Init(void);	//57600bps@24.000MHz
void Uart1_SendString(char *str);    //串口1发送不限长度字符串
void Uart1_SendByte(char byte);


#endif // !_UART_H
