#include <uart.h>
#include "stc15.h"

volatile char RCVOK = 0, RCVDATA = 0, RX5A, RXA5, RXLEN, RXCMD, RXADRH, RXADRL, RXDLEN, TXTIME, RXDATA1, RXDATA2;
unsigned char TX_P, RX_P = 0x00;    // 收发缓冲区的指针位置
__xdata unsigned char CFGBUF[32];
volatile int model = 0;
volatile int core_diameter = 0;
volatile int end_face_distance = 0;
volatile int up_down_speed = 0;
volatile int up_down_distance = 0;
volatile int left_right_speed = 0;
volatile int left_right_distance = 0;

int update_parameters(void) 
{
    model = (CFGBUF[0] << 8) | CFGBUF[1];
    core_diameter = (CFGBUF[2] << 8) | CFGBUF[3];
    end_face_distance = (CFGBUF[4] << 8) | CFGBUF[5];
    up_down_speed = (CFGBUF[6] << 8) | CFGBUF[7];
    up_down_distance = (CFGBUF[8] << 8) | CFGBUF[9];
    left_right_speed = (CFGBUF[10] << 8) | CFGBUF[11];
    left_right_distance = (CFGBUF[12] << 8) | CFGBUF[13];

	if(RCVOK == 0xff)
	{
		RCVOK == 0;
		return 1;
	}
	else
		return 0;
}

// 发送CFGBUF中所有数据
void SendAllData(void)
{
    
    unsigned char i;
    for (i = 0; i < 32; i++)
    {
        // 通过串口发送每个字节
        Uart1_SendByte(CFGBUF[i]);
    }
}

// 获取串口屏所有数据
void GetAllData(void)
{
    Uart1_SendByte(0x5A);
    Uart1_SendByte(0xA5);
    Uart1_SendByte(0x04);
    Uart1_SendByte(0x83);
    Uart1_SendByte(0x00);
    Uart1_SendByte(0x10);
    Uart1_SendByte(0x10);
}
//  跳转到串口屏*页（16进制）
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

//  串口1  中断
void Uart1_Isr(void) __interrupt(4)
{
    unsigned char i;
    if (RI) // 检测串口1接收中断
    {
        i = SBUF;
        RI = 0; //  清除串口1接收中断请求位
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
                RX_P = (RXADRL - 0x10)*2;
                RXDLEN = RXDLEN*2;
                RX_P = 0;
            }
            else //保存所有数据
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
	if (TI)				//检测串口1发送中断
	{
		TI = 0;			//清除串口1发送中断请求位
	}
}

//  串口1 初始化
void Uart1_Init(void)	//57600bps@24.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器时钟1T模式
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x98;			//设置定时初始值
	TH1 = 0xFF;			//设置定时初始值
	ET1 = 0;			//禁止定时器中断
	TR1 = 1;			//定时器1开始计时
	ES = 1;				//使能串口1中断

}

//  串口1 发送字符串
void Uart1_SendString(char *str)
{
    while (*str != '\0') // 当字符串未结束时
    {
        SBUF = *str;     // 将字符发送到串口缓冲区
        while (!TI);     // 等待发送完成
        TI = 0;          // 清除发送中断标志
        str++;           // 指向下一个字符
    }
}

//  串口1 发送字符
void Uart1_SendByte(char byte)
{
    SBUF = byte;       // 将字节发送到串口缓冲区
    while (!TI);       // 等待发送完成
    TI = 0;            // 清除发送中断标志
}


