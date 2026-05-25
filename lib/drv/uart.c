#include "uart.h"
#include "stc15.h"
#include "gpio.h"

/*=== 全局缓冲区 ===*/
volatile unsigned char __xdata CFGBUF[CFGBUF_SIZE];
volatile unsigned char __xdata 型号数组[MODEL_BUF_SIZE];
volatile char 接收到数据 = 0;

/*=== ISR 内部状态 ===*/
__xdata unsigned char RX5A, RXA5, RXLEN, RXCMD, RXADRH, RXADRL, RXDLEN;
__xdata unsigned int RX_P = 0;
volatile char rx_mode = 0;

/*=== UART1 初始化 57600bps @11.0592MHz ===*/
void Uart1_Init(void)
{
    SCON = 0x50;  // 8位数据, 可变波特率
    AUXR |= 0x01; // 串口1选择定时器2为波特率发生器
    AUXR |= 0x04; // 定时器2时钟1T模式
    T2L = 0xD0;
    T2H = 0xFF;
    AUXR |= 0x10; // 定时器2开始计时
    EA = 1;
    ES = 1;
    REN = 1;
}

char Uart1_SendByte(char byte)
{
    SBUF = byte;
    while (!TI)
        ;
    TI = 0;
    return byte;
}

void Uart1_SendBuffer(const unsigned char *buffer, unsigned int length)
{
    unsigned int i;
    EN_485 = 1;
    for (i = 0; i < length; i++)
    {
        Uart1_SendByte(buffer[i]);
    }
    EN_485 = 0;
}

void Uart1_SendString(const char *str)
{
    EN_485 = 1;
    while (*str)
    {
        Uart1_SendByte(*str++);
    }
    EN_485 = 0;
}

/*=== UART1 ISR: 迪文屏协议解析 ===*/
void Uart1_Isr(void) __interrupt(4)
{
    unsigned int i;
    if (RI)
    {
        i = SBUF;
        RI = 0;

        if (rx_mode == 0)
        {
            RX5A = RXA5;
            RXA5 = RXLEN;
            RXLEN = RXCMD;
            RXCMD = RXADRH;
            RXADRH = RXADRL;
            RXADRL = RXDLEN;
            RXDLEN = i;

            if ((RX5A == 0x5A) && (RXA5 == 0xA5) && RXLEN &&
                (RXCMD == 0x83) && (RXADRH == 0x01) &&
                (RXADRL == 0x00) && RXDLEN)
            {
                rx_mode = 2;
                RX_P = RXADRL * 2;
                RXDLEN = RXDLEN * 2;
            }
            else if ((RX5A == 0x5A) && (RXA5 == 0xA5) && RXLEN &&
                     (RXCMD == 0x83) && (RXADRH == 0x00) &&
                     RXADRL && RXDLEN)
            {
                rx_mode = 1;
                RX_P = RXADRL * 2;
                RXDLEN = RXDLEN * 2 + RX_P;
            }
        }
        else if (rx_mode == 1)
        {
            if (RX_P < CFGBUF_SIZE)
                CFGBUF[RX_P] = i;
            RX_P++;
            if (RX_P >= RXDLEN)
            {
                rx_mode = 0;
                接收到数据 = 1;
            }
        }
        else if (rx_mode == 2)
        {
            if (RX_P < MODEL_BUF_SIZE)
                型号数组[RX_P] = i;
            RX_P++;
            if (RX_P >= RXDLEN)
            {
                rx_mode = 0;
                接收到数据 = 1;
            }
        }
    }
}
