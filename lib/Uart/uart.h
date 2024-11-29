#ifndef _UART_H
#define _UART_H

extern __xdata unsigned char RCVOK, RCVDATA, RX5A, RXA5, RXLEN, RXCMD, RXADRH, RXADRL, RXDLEN, TXTIME, RXDATA1, RXDATA2;
extern unsigned char TX_P, RX_P;    // 收发缓冲区的指针位置
extern volatile __xdata unsigned char CFGBUF[100]; // 接收到的数据缓存

void WriteData(unsigned char address,unsigned char data1,unsigned char data2);
void GoToPage(unsigned char roll);
void GetAllData(void);
void SendAllData(void);
void Uart1_Init(void);
void Uart1_SendByte(char byte);
void Uart1_SendString(char *str); //串口1发送不限长度字符串
void Uart1_Isr(void) __interrupt(4);  //串口1中断

#endif // !_UART_H
