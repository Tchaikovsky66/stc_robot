/*	ipa15w4k61s4 --version1  9.2*/
#include <8051.h>
#include <delay.h>
#include <uart.h>
#include <button.h>
#include <motor.h>
#include <int_isr.h>



__xdata volatile int  tmp_x = 0;
__xdata volatile int  tmp_y = 0;
__xdata volatile char tmp_num = 0;



// void rx1_process(void)
// {
// 	int i = 0;
// 	int f = 0;
// 	int p = 0;
// 	if(rx_cnt == 2)
// 	{		
// 		char i = rx_buffer[0];
// 		if((i == 's') || (i == 'S'))	//暂停
// 		{			
// 			UART_SendString("stop");
// 			interruptButtonFlag = 1;		
// 		}
// 	}
// 	tmp_num = rx_buffer[0];
// 	if((rx_buffer[1] == ',') && (rx_cnt >= 5))
// 	{
// 		for(f = 0,i = 2;i<rx_cnt;i++)
// 		{
// 			if(rx_buffer[i] == ',')	
// 				break;
// 			f = f*10 + rx_buffer[i] - '0';
// 			tmp_x = f;
// 		}
// 		// if(rx_buffer[i] != ',')
// 		// 	f = 0;
// 		i++;
// 		for(p = 0,i = 2;i<rx_cnt;i++)
// 		{
// 			if(rx_buffer[i] == ',')	
// 				break;
// 			p = p*10 + rx_buffer[i] - '0';
// 			tmp_y = p;
// 		}
// 	}

// 	rx_cnt = 0;
// }

/* led41闪烁次数 */
void led_blink(int time)
{
	for(int i = 0;i<time;i++)
	{
		P4_1 = 0;
		delay_ms(500);
		P4_1 = 1;
		delay_ms(500);
	}
}

void motor_init(void)
{
	led_blink(4);

	contorlMotor(1,600,y1_up,200);
	if(y1_0_flag)
	{
		y1_ok = 1;
	}
	contorlMotor(2,600,y2_up,200);
	if(y2_0_flag)
	{
		y2_ok = 1;
	}
	contorlMotor(0,900,motor_left,200);
	if(x_left_flag)
	{
		x_left_flag = 0;
		init_position();
		x_ok = 1;
	}
	
}

void move_test(void)
{

	contorlMotor(1,100,y1_down,300);
	delay_ms(10);
	contorlMotor(1,100,y1_up,300);
	delay_ms(10);
}

void catch_rotor(void)
{

}

//主函数
void main(void)
{
	 // 设置P0.0到P0.6为推挽输出模式
    P0M0 |= 0x7f; // 设置P0M0的第0到第6位为1（0b01111111）
    P0M1 &= ~0x00; // 设置P0M1的第0到第7位为0（0b00000000）

    UART_Init(); // Initialize UART
	Interrupt0_Init();
	UART_SendString("start!\r\n");

	delay_ms(100);

	motor_init();		//位置初始化

    while (1)
    {		
		unsigned char roll = 0x00;
		if(x_ok && y1_ok)
		{

			//循环发送信息
			 UART_SendFrame(0x03,0x00,roll);
			 roll++;
			led_blink(2);

			go_position(0,50,50,400);


			go_position(0,0,0,500);

			go_position(400,200,200,400);

			go_position(400,0,0,500);

			go_position(660,0,0,500);
			go_position(0,0,0,550);


			
		}
		

		// if(Button21_Pressed())
		// {
        // 	//UART_SendString("Hello, world!\r\n"); // Send string
		// 	contorlMotor(0,45,motor_left,50);
		// 	current_position.x = current_position.x-100;
		// }
		
		// if(string_received_flag)
		// {
		// 	//rx1_process();
		// 	UART_SendString(rx_buffer);
		// 	delay_ms(1);
		// 	target_position.x = tmp_x;
		// 	target_position.y1 = tmp_y;
		// 	move_to_target(&current_position, &target_position);		
		// 	string_received_flag = 0;			
		// }

    }
}
