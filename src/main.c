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
	contorlMotor(0,900,motor_right,20);
	if(x_0_flag)
	{
		//x_0_flag = 0;
		init_point(&current_position,0,0);
		x_ok = 1;
	}
	
}

//主函数
void main(void)
{
	 // 设置P0.0到P0.6为推挽输出模式
    P0M0 |= 0x7f; // 设置P0M0的第0到第6位为1（0b01111111）
    P0M1 &= ~0x00; // 设置P0M1的第0到第7位为0（0b00000000）
	// P1M0 |= 0x7f; 
	// P1M1 &= ~0x00;

	//P0 = 0x00;

    UART_Init(); // Initialize UART
	Interrupt0_Init();
	UART_SendString("start!\r\n");

	delay_ms(100);
  	// 初始化当前坐标
	init_point(&current_position,0,0);
	motor_init();
    while (1)
    {		
		if(x_ok)
		{
			set_target_point(&target_position,180,0);
			move_to_target(&current_position,&target_position);
			P4_1 = 1;
			delay_ms(10);

			set_target_point(&target_position,320,0);
			move_to_target(&current_position,&target_position);
			P4_1 = 1;
			delay_ms(10);

			set_target_point(&target_position,180,0);
			move_to_target(&current_position,&target_position);
			P4_1 = 1;
			delay_ms(10);

			set_target_point(&target_position,50,0);
			move_to_target(&current_position,&target_position);
			P4_1 = 1;
			delay_ms(10);
		}
		

		// if(Button21_Pressed())
		// {
        // 	//UART_SendString("Hello, world!\r\n"); // Send string
		// 	contorlMotor(0,45,motor_left,50);
		// 	current_position.x = current_position.x-100;
		// }
		// if(Button20_Pressed())
		// {
        // 	//UART_SendString("Hello, world!\r\n"); // Send string
		// 	contorlMotor(0,180,motor_right,100);
		// 	//current_position.x = current_position.x+100;
		// 	//rotate_motor(0,3200,60);
		// 	// for(int i = 0;i<1600-1;i++)
		// 	// {
		// 	// 	one_step(0,motor_acc[99]);
		// 	// }
		// }
		// if(Button44_Pressed())
		// {
		// 	//UART_SendString("Hello, world!\r\n"); // Send string
		// 	//contorlMotor(45,motor_left,60);
		// 	set_target_point(&target_position, 30, 15);

    	// 	// 移动步进电机到目标坐标
   		// 	move_to_target(&current_position, &target_position);
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
