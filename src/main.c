/*	ipa4wk61s4 --version  */
#include <8051.h>
#include <delay.h>
#include <uart.h>

#define BUFFER_SIZE 16
#define motor_left 0
#define motor_right 1

volatile float minperiod = 4;  //4*100us
volatile float msteps = 800;
volatile float maxspeed = 200; //187.5

volatile __bit interruptButtonFlag = 0; 

volatile char rx_buffer[BUFFER_SIZE];
volatile unsigned char rx_index = 0;
volatile __bit string_received_flag = 0;

float int_pow(float base, int exponent) {
    float result = 1.0;
    int i;
    for (i = 0; i < exponent; ++i) {
        result *= base;
    }
    return result;
}

//按键20按下
char Button20_Pressed(void)
{
    if (P2_0 == 0) // Check if button is pressed (assuming active low)
    {
        delay_ms(20); // Debounce delay
        if (P2_0 == 0) // Check again to confirm
        {
            while (P2_0 == 0); // Wait for button release
            delay_ms(20); // Debounce delay
            return 1; // Button press confirmed
        }
    }
    return 0; // No button press
}

char Button21_Pressed(void)
{
    if (P2_1 == 0) // Check if button is pressed (assuming active low)
    {
        delay_ms(20); // Debounce delay
        if (P2_1 == 0) // Check again to confirm
        {
            while (P2_1 == 0); // Wait for button release
            delay_ms(20); // Debounce delay
            return 1; // Button press confirmed
        }
    }
    return 0; // No button press
}

char Button44_Pressed(void)
{
    if (P4_4 == 0) // Check if button is pressed (assuming active low)
    {
        delay_ms(20); // Debounce delay
        if (P4_4 == 0) // Check again to confirm
        {
            while (P4_4 == 0); // Wait for button release
            delay_ms(20); // Debounce delay
            return 1; // Button press confirmed
        }
    }
    return 0; // No button press
}

//串口初始化
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

void UART_ISR(void) __interrupt (4)
{
	if (RI) // Check if receive interrupt flag is set
    {
        RI = 0; // Clear receive interrupt flag
        char received_char = SBUF ;// Read received data

        if (received_char == '\n') // Check for end of string
        {
            rx_buffer[rx_index] = '\0'; // Null-terminate the string
            string_received_flag = 1; // Set string received flag
            rx_index = 0; // Reset buffer index
        }
        else
        {
            if (rx_index < BUFFER_SIZE - 1) // Prevent buffer overflow
            {
                rx_buffer[rx_index++] = received_char;// Store received character
            }
        }
    }
}

//中断0初始化 用作按键中断 INT0	P3^2
void Interrupt0_Init(void)
{
	IT0 = 0;  // 0下降沿触发中断 1都可
    EX0 = 1;  // 使能外部中断 0
    EA = 1;   // 使能全局中断
}

//按键中断 INT0	P3^2
void my_ISR(void) __interrupt (0) 
{
    interruptButtonFlag = 1;  // 设置标志，表示按键已按下
}
void motor_init(void)
{
	msteps = 800;  //400steps per r 
	maxspeed = 187.5; //	r/min
	minperiod = 400;	//0.4ms
}
float speed_to_period(float speed)		//	100us/step
{
	// 确保速度不为零，避免除以零的情况
    if (speed <= 0) {
        return 0xFFFF; // 返回一个非常大的值，表示无法处理
    }
	speed = speed/60;		//	r/min -> r/s
	float period = (1000*10/msteps)/speed;		//	100us/step
	return period;
}
void one_step(int period)
{
	P1_0 = 0;
	delay_100us(period);	//速度转换成周期
	P1_0 = 1;
	delay_100us(period);	//速度转换成周期
}
//控制步进电机旋转
void rotate_motor(unsigned int steps,__bit dir, unsigned int speed)
{
	P1_1 = dir;
	int currentperiod = speed_to_period(speed)/2;
	for(int i = 0;i < steps; i++)
	{
		if(interruptButtonFlag)	//外部中断
		{
			return;
		}
		else
		{
			one_step(currentperiod);
		}
	}	

}

//电机可变级变速 acc==1 加速 0减速
// void variable_speed_motor(unsigned int steps, __bit acc, __bit dir, unsigned int setspeed)
// {
// 	P1_1 = dir; //方向
// 	float variable = 100;
// 	float minspeed = 20;
// 	float in = (setspeed - minspeed) / variable;
// 	unsigned int currentspeed = acc ? minspeed : setspeed;
// 	for(int i = 0; i < variable; i++) //100级变速
// 	{		
// 		int currentperiod = speed_to_period(currentspeed)/2;
// 		for(int j = 0; j < steps / variable; j++ )
// 		{
// 			if(interruptButtonFlag)	//外部中断
// 			{
// 				return;
// 			}
// 			one_step(currentperiod);
// 		}
// 		acc ? (currentspeed += in) : (currentspeed -= in);
// 		if(currentspeed > setspeed)
// 			currentspeed = setspeed;
// 		if(currentspeed<minspeed)
// 			currentspeed = minspeed;
// 	}
// }
 
// 电机曲线变速 acc==1 加速 0减速
void variable_speed_motor(unsigned int steps, __bit acc, __bit dir, unsigned int speed)
{
    P1_1 = dir; // 方向
    float variable = 100;
	float minspeed = 20;
	if(speed<minspeed)
	{
		minspeed = 0;
	}
    for (int i = 0; i < variable; i++) // 100级变速
    {
        // 计算S型曲线速度
        float t = (float)i / (variable - 1);
        float s_curve_speed = acc ? minspeed + (speed - minspeed) * (3 * int_pow(t, 2) - 2 * int_pow(t, 3))
                                  : speed - (speed - minspeed) * (3 * int_pow(t, 2) - 2 * int_pow(t, 3));

        int currentperiod = speed_to_period(s_curve_speed) / 2;
        for (int j = 0; j < steps / variable; j++)
        {
            if (interruptButtonFlag) // 外部中断
            {
                return;
            }
            one_step(currentperiod);
        }
    }
}


void contorlMotor(float distance,__bit dir, unsigned int setspeed)
{
	 unsigned int accsteps;
	 unsigned int decsteps;
	 float circle = distance/90;
	 float steps = msteps*circle;
	 if(steps>=800)
	 {
	 	accsteps = 200;
	  	decsteps = 200;
		steps = steps - 400;
	 }
	 else
	 {
		accsteps = 0;
		decsteps = 0;
	 }
	 if(steps <= 0)
	 	steps = 0;
	P1_1 = dir; //方向
	if(interruptButtonFlag == 0)
		variable_speed_motor(accsteps,1,dir,setspeed);
	if(interruptButtonFlag == 0)
		rotate_motor(steps,dir,setspeed);
	if(interruptButtonFlag == 0)
		variable_speed_motor(decsteps,0,dir,setspeed);
	if(interruptButtonFlag == 1)
	{
		interruptButtonFlag = 0;
		return;

	}
}

//主函数
void main(void)
{
	 // 配置中断优先级
    UART_Init(); // Initialize UART
	Interrupt0_Init();
	motor_init();
	delay_ms(100);
	UART_SendString("start!\r\n");
    while (1)
    {
		if(Button21_Pressed())
		{
        	//UART_SendString("Hello, world!\r\n"); // Send string
			contorlMotor(450,motor_left,50);
		}
		if(Button20_Pressed())
		{
        	//UART_SendString("Hello, world!\r\n"); // Send string
			contorlMotor(450,motor_right,50);
		}
		if(Button44_Pressed())
		{
			contorlMotor(110,motor_left,50);
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
