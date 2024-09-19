#include <uart.h>



__xdata volatile char rx_buffer[UART_BUFFER_SIZE];		//uart 接收缓存buffer
volatile unsigned char rx_cnt = 0;		//uart接收计数
volatile __bit string_received_flag = 0;		//uart接收标志位

/*串口初始化*/
void UART_Init(void)
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器时钟1T模式
	T2L = 0xD0;			//设置定时初始值
	T2H = 0xFF;			//设置定时初始值
	AUXR |= 0x10;		//定时器2开始计时

    // 使能串口中断
    EA = 1; // Enable global interrupt
    ES = 1; // Enable serial interrupt
}

/*发送单个字符*/
// UART发送单个字节
void UART_SendByte(unsigned char byte) {
    SBUF = byte;
    while (!TI);  // 等待发送完成
    TI = 0;       // 清除发送中断标志
}

/*发送字符串*/
void UART_SendString(char *str)
{
    while (*str)
    {
        UART_SendByte(*str++);
    }
}

void UART_SendFrame(unsigned char address, unsigned char data1, unsigned char data2) {
    UART_SendByte(FRAME_HEADER1);
    UART_SendByte(FRAME_HEADER2);
    UART_SendByte(DATA_LENGTH);
    UART_SendByte(COMMAND);
    UART_SendByte(address);
    UART_SendByte(data1);
    UART_SendByte(data2);
}


/*串口中断*/
void UART_ISR(void) __interrupt (4) 
{
    if (RI) {
        RI = 0;  // 清除接收中断标志
        rx_buffer[rx_cnt++] = SBUF;  // 读取接收到的数据
        if (rx_cnt >= UART_BUFFER_SIZE) {
            rx_cnt = 0;  // 防止缓冲区溢出
        }
    }
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
