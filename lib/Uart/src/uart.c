# include <uart.h>

#define BUFFER_SIZE 16

__xdata volatile char rx_buffer[BUFFER_SIZE];		//uart 接收缓存buffer
volatile unsigned char rx_cnt = 0;		//uart接收计数
volatile __bit string_received_flag = 0;		//uart接收标志位

/*串口初始化*/
void UART_Init(void)
{
    SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器时钟1T模式

	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0xD0;		//设置定时初始值
	TH1 = 0xFF;		//设置定时初始值

	ET1 = 0;		//禁止定时器%d中断
	TR1 = 1;		//定时器1开始计时

    // 使能串口中断
    EA = 1; // Enable global interrupt
    ES = 1; // Enable serial interrupt
}

/*发送单个字符*/
void UART_SendChar(char c)
{
    SBUF = c; // Load data into UART buffer
    while (!TI); // Wait for transmission to complete
    TI = 0; // Clear transmission interrupt flag
}

/*发送字符串*/
void UART_SendString(char *str)
{
    while (*str)
    {
        UART_SendChar(*str++);
    }
}



/*串口中断*/
void UART_ISR(void) __interrupt (4)
{
	if (RI) // Check if receive interrupt flag is set
    {
        RI = 0; // Clear receive interrupt flag
        char received_char = SBUF ;// Read received data

        if (received_char == '\n') // Check for end of string
        {
            rx_buffer[rx_cnt] = '\0'; // Null-terminate the string
            string_received_flag = 1; // Set string received flag
            //rx_cnt = 0; // Reset buffer index
        }
        else
        {
            if (rx_cnt < BUFFER_SIZE - 1) // Prevent buffer overflow
            {
                rx_buffer[rx_cnt++] = received_char;// Store received character
            }
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
