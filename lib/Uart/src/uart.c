#include <uart.h>

__xdata volatile char rx_buffer[UART_BUFFER_SIZE]; // uart 接收缓存buffer
volatile unsigned char rx_cnt = 0;                 // uart接收计数
volatile __bit string_received_flag = 0;           // uart接收标志位

__xdata unsigned char RCVOK, RCVDATA = 0, RX5A, RXA5, RXLEN, RXCMD, RXADRH, RXADRL, RXDLEN, TXTIME;
unsigned char TX_P, RX_P = 0;    // 收发缓冲区的指针位置
__xdata unsigned char CFGBUF[32]; // 接收到的数据缓存
__xdata unsigned char TXBUF[84] = {0x5A, 0xA5, 0x4, 0x83, 0x0, 0x10, 0x10};
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

/*发送单个字符*/
// UART发送单个字节
void UART_SendByte(unsigned char byte)
{
    SBUF = byte;
    while (!TI)
        ;   // 等待发送完成
    TI = 0; // 清除发送中断标志
}

/*发送字符串*/
void UART_SendString(char *str)
{
    while (*str)
    {
        UART_SendByte(*str++);
    }
}

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

/*串口中断*/
// void UART_ISR(void) __interrupt (4)
// {
//     if (RI) {
//         RI = 0;  // 清除接收中断标志
//         rx_buffer[rx_cnt++] = SBUF;  // 读取接收到的数据
//         if (rx_cnt >= UART_BUFFER_SIZE) {
//             rx_cnt = 0;  // 防止缓冲区溢出
//             string_received_flag = 1;
//         }

//     }
// }

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
            if ((RX5A == 0x5A) && (RXA5 == 0xA5) && (RXLEN == 0x24) && (RXCMD == 0x83) && (RXADRH == 0x00) && (RXADRL == 0x10) && (RXDLEN == 0x10))
            {
                RCVDATA = 0xff;
                RX_P = 0;
            }
        }
        else
        {
            P2_0 = 0;
            CFGBUF[RX_P] = i;
            RX_P++;
            if (RX_P == 32)
            {
                RCVDATA = 0;
                RCVOK = 0xff;
            }
        }
    }
    // if (TI) // 发送中断
    // {
    //     TI = 0;
    //     TX_P++;
    //     if (TX_P < 84)
    //     {
    //         SBUF = TXBUF[TX_P];
    //     }
    // }
}

// void my_isr(void) __interrupt (4)
// {
//     if(RI)
//     {
//         RI = 0;
//         if(rx_cnt == BUFFER_SIZE)
//         {
//             rx_cnt = 0;
//             string_received_flag = 1;
//         }
//         else
//         {
//             char received_char = SBUF;
//             rx_buffer[rx_cnt++] = received_char;
//         }
//     }
// }
