#include <motor.h>

// 定义最小速度和最大速度
#define MIN_SPEED 20
#define MAX_SPEED 1000
#define variable 100		// 100级变速

// 定义数组来存储预先计算的S型速度值
__xdata unsigned int motor_acc[variable];

volatile __bit x_right_flag = 0;	//检查x方向0位
volatile __bit x_left_flag = 0;	//检查左边是否碰撞
volatile __bit x_ok = 0;		//判断x方向是否可动作x

volatile __bit y1_0_flag = 0;	//检查y方向0位
volatile __bit y2_0_flag = 0;	//检查y方向0位

volatile __bit y1_ok = 0;		//判断y方向是否可动作
volatile __bit y2_ok = 0;		//判断y方向是否可动作


volatile int x = 0;
volatile int y1 = 0;
volatile int y2 = 0;


void init_position(void)
{
	x = 0;
	y1 = 0;
	y2  = 0;
}

void go_position(int target_x,int target_y1,int target_y2,int setspeed)
{
	int dif_x = target_x - x;
	int dif_y1 = target_y1 - y1;
	int dif_y2 = target_y2 - y2;

if(dif_y1 != 0)
	{
		if(dif_y1>0)
		{
			contorlMotor(1,dif_y1,y1_down,setspeed);
		}
		else
		{
			contorlMotor(1,-dif_y1,y1_up,setspeed);
		}

		y1 = target_y1;
	}
	if(dif_y2 != 0)
	{
		if(dif_y2>0)
		{
			contorlMotor(2,dif_y2,y2_down,setspeed);
		}
		else
		{
			contorlMotor(2,-dif_y2,y2_up,setspeed);
		}

		y2 = target_y2;
	}

	if(dif_x != 0)
	{
		if(dif_x>0)
		{
			contorlMotor(0,dif_x,motor_right,setspeed);
		}
		else
		{
			contorlMotor(0,-dif_x,motor_left,setspeed);
		}

		x = target_x;
	}
	

}

// 将输入距离转换为整数步数
int distance_to_steps(float distance_mm) {
    const float STEPS_PER_REVOLUTION = 2000.0;  // 每转步数
    const float MM_PER_REVOLUTION = 90.0;       // 每转移动距离（毫米）
    
    // 计算每步的步长（毫米）
    float step_length_mm = MM_PER_REVOLUTION / STEPS_PER_REVOLUTION;
    
    // 计算给定距离需要的步数并四舍五入到最近的整数
    int steps = (int)(distance_mm / step_length_mm + 0.5);
    
    return steps;
}


// 计算步进电机脉冲周期 /10us
int calculate_pulse_period(float speed_mm_per_sec) {
    // 定义常量
    const float STEPS_PER_REVOLUTION = 2000.0;  // 每转步数
    const float MM_PER_REVOLUTION = 90.0;       // 每转移动距离（毫米）
    
    // 计算每秒需要的步数
    float steps_per_second = (speed_mm_per_sec / MM_PER_REVOLUTION) * STEPS_PER_REVOLUTION;
    
    // 计算脉冲周期（秒）
    float pulse_period_sec = 1.0 / steps_per_second;
    
    // 将周期转换为微秒
    float pulse_period_us = pulse_period_sec * 100000.0/2;
    unsigned long pulse_period_us_int = (unsigned long)pulse_period_us;		//to 10us
    
    return pulse_period_us;
}

// 生成直线加速数组
void generate_linear_acceleration(float setspeed) {
    // 确保setspeed在最大最小速度之间
    if (setspeed < MIN_SPEED) setspeed = MIN_SPEED;
    if (setspeed > MAX_SPEED) setspeed = MAX_SPEED;
    
    float speed_difference = setspeed - MIN_SPEED;
    float speed_increment = speed_difference / variable;
    
    for (int i = 0; i < variable; i++) {
        float current_speed = MIN_SPEED + speed_increment + i * speed_increment;
        motor_acc[i] = calculate_pulse_period(current_speed);
    }
}


/*输出每步*/
int one_step(char type,int period)
{
	x_right_flag = !P1_3; 	//右0位
	x_left_flag = !P1_4;	//左限位
	y1_0_flag = !P3_3;	//y1 0位	
	y2_0_flag = !P3_6;	//y1 0位	


	if(x_right_flag)
	{
		x_ok = 0;
		return 1;
	}
	if(y1_0_flag && !y1_ok)		//初始化
	{
		return 1;
	}
	if(y2_0_flag && !y2_ok)		//初始化
	{
		return 1;
	}
	if(x_left_flag && !x_ok)		//x在0️位且x不可动作	初始化
	{
		return 1;
	}
	if(x_left_flag && x_ok)
	{
		x = 0;
		y1 = 0;
	}
	if(type == 0)		//电机x
	{
		//P0_1 = !P0_1;
		P0_1 = 0;		//输出脉冲
		delay_10us(period);	//速度转换成周期
		P0_1 = 1;		//输出脉冲
		delay_10us(period);	//速度转换成周期
	}
	if(type == 1)		//电机y1
	{
		//P0_4 = !P0_4;
		P0_4 = 0;
		delay_10us(period);	//速度转换成周期
		P0_4 = 1;
		delay_10us(period);	//速度转换成周期
	}
	if(type == 2)		//电机y2
	{
		P0_6 = 0;
		delay_10us(period);	//速度转换成周期
		P0_6 = 0;
		delay_10us(period);	//速度转换成周期
	}

	return 0;
	
}


/*控制步进电机旋转（不变速）*/
int rotate_motor(char type,unsigned int steps)
{
	//int currentperiod = speed_to_period(speed)/2;
	for(int i = 0;i < steps; i++)
	{
		if(interruptButtonFlag)	//外部中断
		{
			return 1 ;
		}
		else
		{
			if(one_step(type, motor_acc[99]))
			{
				return 1;
			}
		}
	}	
	return 0;
}

/* 电机曲线变速 acc==1 加速 0减速 */
int variable_speed_motor(char type, unsigned int steps, __bit acc)
{
	int x = steps/variable;
	if(acc)
	{

		for (int i = 0; i < variable; i++) // 100级变速
		{	
			for (int j = 0; j < x; j++)
			{


				if (interruptButtonFlag) // 外部中断
				{
					return 1;
				}
				if(one_step(type, motor_acc[i]))
					return 1;
			}
	
		}
	}

	if(acc == 0)
	{
		for (int i = 99; i >=0; i--) // 100级变速
		{	
			for (int j = 0; j < x; j++)
			{

				if (interruptButtonFlag) // 外部中断
				{
					return 1;
				}
				if(one_step(type, motor_acc[i]))
					return 1;
			}
	
		}
	}
	return 0;

}

/* 步进电机控制（电机编号0、1、2，距离：单位mm，方向0|1，速度r/min） */	
void contorlMotor(char type,int distance,__bit dir, float setspeed) 
{
	 unsigned int accsteps;
	 unsigned int decsteps;
	 unsigned int steps = distance_to_steps(distance);		//P0_1 = !P0_1;	总步数*2

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

	P0_0 = dir;		//电机0 方向
	P0_2 = 0;		//电机0 enable
	P0_3 = dir;		//电机1 方向 
	P0_5 = dir;
	
	generate_linear_acceleration(setspeed);
	if(interruptButtonFlag == 1 || variable_speed_motor(type,accsteps,1))		//判断中断
	{
		return ;
	}
	if(interruptButtonFlag == 1 || rotate_motor(type,steps) )
	{
		return ;
	}
	if(interruptButtonFlag == 1 ||variable_speed_motor(type,decsteps,0))
	{
		return ;
	}
	if(interruptButtonFlag)
	{
		interruptButtonFlag = 0;
		return;
	}
}
