#include <uart.h>

__xdata volatile char rx_buffer[UART_BUFFER_SIZE]; // uart 接收缓存buffer
volatile unsigned char rx_cnt = 0;                 // uart接收计数
volatile __bit string_received_flag = 0;           // uart接收标志位

__xdata unsigned char RCVOK = 0, RCVDATA = 0, RX5A, RXA5, RXLEN, RXCMD, RXADRH, RXADRL, RXDLEN, TXTIME, RXDATA1, RXDATA2;
unsigned char TX_P, RX_P = 0x00;    // 收发缓冲区的指针位置
__xdata unsigned char CFGBUF[32]; // 接收到的数据缓存
__xdata unsigned char TXBUF[84] = {0x5A, 0xA5, 0x4, 0x83, 0x0, 0x10, 0x10};
__xdata unsigned char TMPBUF[2];
int model = 0;
int core_diameter = 0;
int end_face_distance = 0;
int up_down_speed = 0;
int up_down_distance = 0;
int left_right_speed = 0;
int left_right_distance = 0;
/*串口初始化*/
void UART_Init(void)
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器时钟1T模式
	T2L = 0x98;			//设置定时初始值
	T2H = 0xFF;			//设置定时初始值
	AUXR |= 0x10;		//定时器2开始计时

    EA = 1;
    ES = 1;

    REN = 1;
}
/* !!!err */
// void UART2_Init(void)	//57600bps@24.000MHz
// {
// 	S2CON = 0x50;		//8位数据,可变波特率
// 	AUXR |= 0x04;		//定时器时钟1T模式
// 	T2L = 0x98;			//设置定时初始值
// 	T2H = 0xFF;			//设置定时初始值
// 	AUXR |= 0x10;		//定时器2开始计时
// }


/*发送单个字符*/
// UART发送单个字节
void UART_SendByte(unsigned char byte)
{
    SBUF = byte;
    while (!TI)
        ;   // 等待发送完成
    TI = 0; // 清除发送中断标志
}
// 使用串口2发送单个字节
void UART2_SendByte(unsigned char byte)
{
    S2BUF = byte;
    while (!(S2CON & 0x02));  // 等待发送完成
    S2CON &= ~0x02;  // 清除发送中断标志
}

/*发送字符串*/
void UART_SendString(unsigned char *str)
{
    while (*str)
    {
        UART_SendByte(*str++);
    }
}
// 使用串口2发送字符串
void UART2_SendString(unsigned char *str)
{
    while (*str)
    {
        UART2_SendByte(*str++);
    }
}

//发送帧 
void UART_SendFrame(unsigned char address, unsigned char data1, unsigned char data2)
{
    UART_SendByte(FRAME_HEADER1);
    UART_SendByte(FRAME_HEADER2);
    UART_SendByte(DATA_LENGTH);
    UART_SendByte(COMMAND);
    UART_SendByte(address);
    UART_SendByte(data1);
    UART_SendByte(data2);
}

//从串口屏获取数据
void get_dwin_data(void)
{
    UART_SendByte(0x5A);
    UART_SendByte(0xA5);
    UART_SendByte(0x04);
    UART_SendByte(0x83);
    UART_SendByte(0x00);
    UART_SendByte(0x10);
    UART_SendByte(0x10);
    
}
void UART_ISR(void) __interrupt(4)
{
    unsigned char i;
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
                RX_P = (RXADRL - 0x10)*2;
                RXDLEN = RXDLEN*2;
                RX_P = 0;
            }
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

