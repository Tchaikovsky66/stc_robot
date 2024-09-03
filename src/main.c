/*	ipa15w4k61s4 --version1  9.2*/
#include <8051.h>
#include <delay.h>
#include <uart.h>
#include <button.h>
#include <motor.h>
#include <int_isr.h>

#define motor_left 0
#define motor_right 1

/*电机初始化 考虑弃用*/
void motor_init(void)
{
	msteps = 800;  //400steps per r 
	maxspeed = 150; //	r/min
	minperiod = 400;	//0.4ms
}

//主函数
void main(void)
{
    UART_Init(); // Initialize UART
	Interrupt0_Init();
	UART_SendString("start!\r\n");
	delay_ms(100);
    while (1)
    {
		if(Button21_Pressed())
		{
        	//UART_SendString("Hello, world!\r\n"); // Send string
			contorlMotor(450,motor_left,110);
		}
		if(Button20_Pressed())
		{
        	//UART_SendString("Hello, world!\r\n"); // Send string
			contorlMotor(450,motor_right,110);
		}
		if(Button44_Pressed())
		{
			UART_SendString("Hello, world!\r\n"); // Send string
			//contorlMotor(45,motor_left,60);
			//contorlMotor(0,800*10,0,motor_left);
		}
		if(string_received_flag)
		{
			string_received_flag = 0;
			UART_SendString(rx_buffer);
		}
		P0_1 = !P0_1;

    }
}
