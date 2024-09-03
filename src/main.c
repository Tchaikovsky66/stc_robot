/*	ipa15w4k61s4 --version1  9.2*/
#include <8051.h>
#include <delay.h>
#include <uart.h>
#include <button.h>

#define BUFFER_SIZE 16
#define motor_left 0
#define motor_right 1

#define variable 100 // 100级变速

// 定义数组来存储预先计算的S型速度值
__xdata float s_curve_speeds[variable];
__xdata float s_curve_speeds_slow[variable];


volatile float minperiod = 4;  //4*100us
volatile float msteps = 800;
volatile float maxspeed = 150; //187.5

volatile __bit interruptButtonFlag = 0; //按键中断

__xdata volatile char rx_buffer[BUFFER_SIZE];		//uart 接收缓存buffer
volatile unsigned char rx_index = 0;		//uart接收计数
volatile __bit string_received_flag = 0;		//uart接收标志位


/*整数次幂函数*/
float int_pow(float base, int exponent) {
    float result = 1.0;
    int i;
    for (i = 0; i < exponent; ++i) {
        result *= base;
    }
    return result;
}



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

/*串口中断*/
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

/*按键中断 INT0	P3^2*/
void my_ISR(void) __interrupt (0) 
{
    interruptButtonFlag = 1;  // 设置标志，表示按键已按下
}

/*电机初始化 考虑弃用*/
void motor_init(void)
{
	msteps = 800;  //400steps per r 
	maxspeed = 187.5; //	r/min
	minperiod = 400;	//0.4ms
}
/*速度-周期转换 rotate_motor使用*/
float speed_to_period(float speed)		//	100us/step
{
	speed = speed/60;		//	r/min -> r/s
	float period = (1000*10/msteps)/speed;		//	100us/step
	return period;
}
 
/*输出每步*/
void one_step(int period)
{
	P1_0 = 0;
	delay_100us(period);	//速度转换成周期
	P1_0 = 1;
	delay_100us(period);	//速度转换成周期
}
/*控制步进电机旋转（不变速）*/
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

/* 计算速度变化曲线 输入速度，输出周期 储存在s_curve_speeds[i]*/
void  calculate_s_curve_periods(float speed)
{
	float minspeed = 10;
    for (int i = 0; i < variable; i++)
    {
        float t = (float)i / (variable - 1);
			s_curve_speeds[i] = minspeed + (speed - minspeed) * (3 * int_pow(t, 2) - 2 * int_pow(t, 3));
			s_curve_speeds[i] = (1000*10/msteps)/(s_curve_speeds[i]/60);
			s_curve_speeds_slow[i] =  speed - (speed - minspeed) * (3 * int_pow(t, 2) - 2 * int_pow(t, 3));
			s_curve_speeds_slow[i] = (1000*10/msteps)/(s_curve_speeds[i]/60);
    }
}

/* 电机曲线变速 acc==1 加速 0减速 */
void variable_speed_motor(unsigned int steps, __bit acc, __bit dir)
{
    P1_1 = dir; // 方向
    for (int i = 0; i < variable; i++) // 100级变速
    {
        // 计算S型曲线速度
        float s_curve_speed = acc? s_curve_speeds[i] : s_curve_speeds_slow[i];
		
		
        int currentperiod = s_curve_speed / 2;
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

/* 步进电机控制（距离：单位mm，方向0|1，速度r/min） */	
void contorlMotor(float distance,__bit dir, unsigned int setspeed) 
{
	 unsigned int accsteps;
	 unsigned int decsteps;
	 float circle = distance/90;
	 float steps = msteps*circle;
	 if(steps>=400)		//加减速步数
	 {
	 	accsteps = 200;
	  	decsteps = 200;
		steps = steps - 400;
	 }
	 else //距离过短，加减速平分
	 {
		accsteps = steps/2;
		decsteps = steps/2;
		steps = 0;
	 }
	 if(steps <= 0)
	 	steps = 0;
	P1_1 = dir; //方向
	 calculate_s_curve_periods(setspeed);		//计算加速速度曲线，存入xdata s_cuclate_speed[variable]中
	if(interruptButtonFlag == 0)		//判断中断
		variable_speed_motor(accsteps,1,dir);
	if(interruptButtonFlag == 0)
		rotate_motor(steps,dir,setspeed);
	if(interruptButtonFlag == 0)
		variable_speed_motor(decsteps,0,dir);
	if(interruptButtonFlag == 1)
	{
		interruptButtonFlag = 0;
		return;

	}
}

//主函数
void main(void)
{
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
			contorlMotor(450,motor_left,110);
		}
		if(Button20_Pressed())
		{
        	//UART_SendString("Hello, world!\r\n"); // Send string
			contorlMotor(450,motor_right,110);
		}
		if(Button44_Pressed())
		{
			contorlMotor(45,motor_left,60);
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
