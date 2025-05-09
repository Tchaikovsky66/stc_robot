#include <uart.h>
#include <8051.h>
#include <delay.h>
#include "../../include/stc15.h"
#include "../../include/main.h"

// 将所有变量定义为片外RAM
__bit 接收到数据 = 0;
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
    EN_485 = 1;
    Uart1_SendByte(0x5A);
    Uart1_SendByte(0xA5);
    Uart1_SendByte(0x05);
    Uart1_SendByte(0x82);
    Uart1_SendByte(0x00);
    Uart1_SendByte(address);
    Uart1_SendByte(data1);
    Uart1_SendByte(data2);
    EN_485 = 0;
}

/**
 * @brief 切换显示页面
 * @param page 目标页面编号
 */
void GoToPage(unsigned char page)
{
    EN_485 = 1;
    Uart1_SendByte(0x5A);
    Uart1_SendByte(0xA5);
    Uart1_SendByte(0x04);
    Uart1_SendByte(0x80);
    Uart1_SendByte(0x03);
    Uart1_SendByte(0x00);
    Uart1_SendByte(page);
    EN_485 = 0;
}

/**
 * @brief 获取显示屏所有数据
 * @note 发送获取数据命令到显示屏
 */
void GetAllData(void)
{
    static const unsigned char get_all_adata[] = {0x5A, 0xA5, 0x04, 0x83, 0x00, 0x10, 0x2B};
    Uart1_SendBuffer(get_all_adata, sizeof(get_all_adata));
    // Uart1_SendBuffer(get_all_data,7)
}

/**
 * @brief 发送指定长度的数据缓冲区
 * @param buffer 数据缓冲区指针
 * @param length 要发送的数据长度
 */
void Uart1_SendBuffer(const unsigned char *buffer, unsigned int length)
{
    EN_485 = 1;
    for (unsigned int i = 0; i < length; i++)
    {
        Uart1_SendByte(buffer[i]);
    }
    EN_485 = 0;
}

/**
 * @brief 发送所有数据到显示屏
 * @note 将CFGBUF中的数据发送到显示屏
 */
void SendAllData(void)
{
    Uart1_SendBuffer(CFGBUF, 0x2B * 2);
}

void Uart1_Init(void) // 57600bps@11.0592MHz
{
    SCON = 0x50;  // 8位数据,可变波特率
    AUXR |= 0x01; // 串口1选择定时器2为波特率发生器
    AUXR |= 0x04; // 定时器时钟1T模式
    T2L = 0xD0;   // 设置定时初始值
    T2H = 0xFF;   // 设置定时初始值
    AUXR |= 0x10; // 定时器2开始计时

    // IE2 |= 0x04; // 使能定时器2中断 ***
    EA = 1;
    ES = 1;
    REN = 1;
    // IP = 0x10; // 串行口为高优先级中断
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
    EN_485 = 1;
    while (*str)
    {
        Uart1_SendByte(*str++);
    }
    EN_485 = 0;
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
                接收到数据 = 1;
            }
        }
    }
}