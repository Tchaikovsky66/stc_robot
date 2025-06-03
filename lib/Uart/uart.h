#ifndef _UART_H
#define _UART_H

extern __xdata unsigned char RCVOK, RCVDATA;
extern volatile unsigned char __xdata CFGBUF[96];
extern volatile unsigned char __xdata 型号数组[0x88];
extern __xdata unsigned char RX5A, RXA5, RXLEN, RXCMD, RXADRH, RXADRL, RXDLEN;
extern __xdata unsigned int RX_P;

void WriteData(unsigned char address, unsigned char data1, unsigned char data2);
void GoToPage(unsigned char roll);
void GetAllData(void);
void SendAllData(void);
void Uart1_Init(void);
char Uart1_SendByte(char byte);
void Uart1_SendString(const char *str); // 串口1发送不限长度字符串
void Uart1_Isr(void) __interrupt(4);    // 串口1中断
void Uart1_SendBuffer(const unsigned char *buffer, unsigned int length);

// void Uart1_SendIntBuffer(const unsigned int *buffer, unsigned int length);

#endif // !_UART_H
