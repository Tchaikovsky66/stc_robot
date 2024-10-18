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

__xdata volatile int result[UART_BUFFER_SIZE / 2];

// 全局变量声明
// int model = 0;
// int core_diameter = 0;
// int end_face_distance = 0;
// int up_down_speed = 0;
// int up_down_distance = 0;
// int left_right_speed = 0;
// int left_right_distance = 0;

int update_parameters(void) {
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

void print_all_data(void)
{
	int i = 0;
	if(RCVOK == 0xff)
	{
		for(i;i<32;i++)
		{
			UART_SendByte(CFGBUF[i]);
		}
	}
}


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
	led_blink(2);

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
	unsigned char roll = 0x00;
	UART_SendFrame(0x03,0x00,roll);
	 // 设置P0.0到P0.6为推挽输出模式
    P0M0 |= 0x7f; // 设置P0M0的第0到第6位为1（0b01111111）
    P0M1 &= ~0x00; // 设置P0M1的第0到第7位为0（0b00000000）

    UART_Init(); // Initialize UART
	Interrupt0_Init();
	UART_SendString("start!\r\n");

	//delay_ms(100);

	//motor_init();		//位置初始化

	//while(RCVOK == 0x00); // 等待接收完成

	roll = 0x03;
	UART_SendFrame(0x03,0x00,roll);

    while (1)
    {	
		if(update_parameters())
		{
			if(model == 0x01 )	// 如果接收到0x01指令	并且接收标志为0xff	
		{	
			print_all_data();
			RCVOK = 0x00; // 清空接收标志	
			go_position(400,200,200,left_right_speed,up_down_speed);

			//UART_SendString("go_position(400,200,200,left_right_speed,up_down_speed)\r\n");
			UART2_SendString("0x00 0x01 0x02\r\n");
			go_position(0,0,0,left_right_speed,up_down_speed);

			//UART_SendString("go_position(0,0,0,left_right_speed,up_down_speed)\r\n");
			UART2_SendString("0x00 0x01 0x02\r\n");
			led_blink(1);
			RCVOK = 0;
		}
		if(model == 0x02)
		{
			print_all_data();
			led_blink(2);
			RCVOK = 0;
		}
		}
		//led_blink(1);	
		
		if(Button21_Pressed())
		{
        	//UART_SendString("Hello, world!\r\n"); // Send string
			roll++;
			UART_SendFrame(0x03,0x00,roll);
		}

		if(Button20_Pressed())
		{
			roll--;
			UART_SendFrame(0x03,0x00,roll);
		}
		if(Button44_Pressed())
		{  
			get_dwin_data();
			delay_ms(10);
			print_all_data();
		}
			
	}

		// if(x_ok && y1_ok)
		// {

		// 	go_position(0,50,50,400);


		// 	go_position(0,0,0,500);

		// 	go_position(400,200,200,400);

		// 	go_position(400,0,0,500);

		// 	go_position(660,0,0,500);
		// 	go_position(0,0,0,550);			
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
