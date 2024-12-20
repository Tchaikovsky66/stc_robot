#include <uart.h>
#include <8051.h>
#include <delay.h>
#include "../../include/stc15.h"

// 将所有变量定义为片外RAM
__xdata unsigned char RCVOK = 0x00;
__xdata unsigned char RCVDATA = 0x00;
__xdata unsigned char RX5A, RXA5, RXLEN, RXCMD, RXADRH, RXADRL, RXDLEN, TXTIME, RXDATA1, RXDATA2;
__xdata unsigned char TX_P = 0x00;
__xdata unsigned char RX_P = 0x00;

// CFGBUF已经在片外RAM
volatile unsigned char __xdata CFGBUF[100];

/**
 * @brief 向显示屏写入数据
 * @param address 数据地址
 * @param data1 高字节数据
 * @param data2 低字节数据
 * @note 使用串口发送数据到显示屏
 */
void WriteData(unsigned char address, unsigned char data1, unsigned char data2)
{

    Uart1_SendByte(0x5A);
    Uart1_SendByte(0xA5);
    Uart1_SendByte(0x05);
    Uart1_SendByte(0x82);
    Uart1_SendByte(0x00);
    Uart1_SendByte(address);
    Uart1_SendByte(data1);
    Uart1_SendByte(data2);
}

/**
 * @brief 切换显示页面
 * @param roll 目标页面编号
 */
void GoToPage(unsigned char roll)
{

    Uart1_SendByte(0x5A);
    Uart1_SendByte(0xA5);
    Uart1_SendByte(0x04);
    Uart1_SendByte(0x80);
    Uart1_SendByte(0x03);
    Uart1_SendByte(0x00);
    Uart1_SendByte(roll);
}

/**
 * @brief 获取显示屏所有数据
 * @note 发送获取数据命令到显示屏
 */
void GetAllData(void)
{

    Uart1_SendByte(0x5A);
    Uart1_SendByte(0xA5);
    Uart1_SendByte(0x04);
    Uart1_SendByte(0x83);
    Uart1_SendByte(0x00);
    Uart1_SendByte(0x10);
    Uart1_SendByte(0x2B);
}

/**
 * @brief 发送所有数据到显示屏
 * @note 将CFGBUF中的数据发送到显示屏
 */
void SendAllData(void)
{
    static int i;

    for (i = 0; i < 0x2B * 2; i++)
    {
        Uart1_SendByte(CFGBUF[i]);
    }
    DelayMs(10);
}

void UartInit(void) // 9600bps@24.000MHz
{
    S2CON = 0x50; // 8位数据,可变波特率
    AUXR |= 0x04; // 定时器时钟1T模式
    T2L = 0x8F;   // 设置定时初始值
    T2H = 0xFD;   // 设置定时初始值
    AUXR |= 0x10; // 定时器2开始计时
    IE2 |= 0x04;  // 使能定时器2中断 ***
}

void Uart1_Init(void)
{
    SCON = 0x50;  // 8位数据,可变波特率
    AUXR |= 0x01; // 串口1选择定时器2为波特率发生器
    AUXR |= 0x04; // 定时器时钟1T模式
    T2L = 0x98;   // 设置定时初始值
    T2H = 0xFF;   // 设置定时初始值
    AUXR |= 0x10; // 定时器2开始计时

    EA = 1;
    ES = 1;

    REN = 1;

    IP = 0x10; // 串行口为高优先级中断
}

void Uart1_SendByte(char byte)
{
    SBUF = byte;
    while (!TI)
        ;   // 等待发送完成
    TI = 0; // 清除发送中断标志
}

void Uart1_SendString(const char *str) // 串口1发送不限长度字符串
{
    while (*str)
    {
        Uart1_SendByte(*str++);
    }
}

void Uart1_Isr(void) __interrupt(4) // 串口1中
{
    static unsigned char i;
    if (RI) // 接收中断
    {
        i = SBUF;
        RI = 0;
        if (RCVDATA == 0)
        {
            RX5A = RXA5;
            RXA5 = RXLEN;
            RXLEN = RXCMD;
            RXCMD = RXADRH;
            RXADRH = RXADRL;
            RXADRL = RXDLEN;
            RXDLEN = i;
            // 检查帧同步数据 5A A5 24 83 00 10 10 +32B DATA
            if ((RX5A == 0x5A) && (RXA5 == 0xA5) && (RXLEN) && (RXCMD == 0x83) && (RXADRH == 0x00) && (RXADRL) && (RXDLEN))
            {
                RCVDATA = 0xff;
                RX_P = RXADRL * 2;
                RXDLEN = RXDLEN * 2 + RX_P;
            }
        }
        else // 保存所有数据
        {
            CFGBUF[RX_P] = i;
            RX_P++;
            if (RX_P == RXDLEN)
            {
                RCVDATA = 0x00;
                RCVOK = 0xff;
            }
        }
    }
}