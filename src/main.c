/*	ipa15w4k61s4 --version1  9.2*/
#include <8051.h>
#include <delay.h>
#include <uart.h>
#include <button.h>
#include <motor.h>
#include <int_isr.h>

#define motor_left 0
#define motor_right 1
#define y1_up 0;
#define y1_down 1;
#define y2_up 1;
#define y2_down 0;

typedef struct 
{
	/* data */
	int x;
	int y1;
	int y2;
}Point;

/* 坐标初始化*/
void init_point(Point* p,int x, int y1)
{
	p -> x = x;
	p -> y1 = y1;
	// p -> y2 = y2;
}

/* 设置目标点 */
void set_target_point(Point* target, int x, int y1)
{
    target->x = x;
    target->y1 = y1;
	// target->y2 = y2;
}

int move_to_target(Point* current,Point* target)
{
	float distence_x = target->x - current->x;
	float distence_y1 = target->y1 - current->y1;

	distence_x>0?contorlMotor(0,distence_x,motor_left,50):contorlMotor(0,-distence_x,motor_right,50);
	//distence_y1>0?contorlMotor(1,distence_y1,y1_up,50):contorlMotor(1,-distence_x,y1_down,50);

	current->x = target->x;
	current->y1 = target->y1;

	return 0;
}

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

	Point current_position;
    Point target_position;

  	// 初始化当前坐标
	init_point(&current_position,0,0);

    while (1)
    {

		if(Button21_Pressed())
		{
        	//UART_SendString("Hello, world!\r\n"); // Send string
			contorlMotor(0,450,motor_left,110);
			current_position.x = current_position.x+450;
		}
		if(Button20_Pressed())
		{
        	//UART_SendString("Hello, world!\r\n"); // Send string
			contorlMotor(0,450,motor_right,110);
			current_position.x = current_position.x-450;
		}
		if(Button44_Pressed())
		{
			//UART_SendString("Hello, world!\r\n"); // Send string
			//contorlMotor(45,motor_left,60);
			set_target_point(&target_position, 30, 15);

    		// 移动步进电机到目标坐标
   			move_to_target(&current_position, &target_position);
		}
		if(string_received_flag)
		{
			string_received_flag = 0;
			UART_SendString(rx_buffer);
		}

    }
}
