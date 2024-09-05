#include <motor.h>

#define variable 100 // 100级变速

volatile float minperiod = 4.0;  //4*100us
volatile float msteps = 3200.0;	//3200/10
volatile float maxspeed = 100.0; //187.5
// 定义数组来存储预先计算的S型速度值
__xdata float s_curve_speeds[variable];
__xdata float s_curve_speeds_slow[variable];
__xdata float motor_acc[variable];

volatile __bit x_0_flag = 0;	//检查x方向0位
volatile __bit x_1_flag = 0;	//检查左边是否碰撞
volatile __bit x_ok = 0;		//判断x方向是否可动作
// typedef struct 
// {
// 	/* data */
// 	int x;
// 	int y1;
// 	int y2;
// }Point;

Point current_position;
Point target_position;

/*整数次幂函数*/
// float int_pow(float base, int exponent) 
// {
//     float result = 1.0;
//     int i;
//     for (i = 0; i < exponent; ++i) {
//         result *= base;
//     }
//     return result;
// }

/*速度-周期转换 rotate_motor使用*/
float speed_to_period(float speed)		//	100us/step
{
	speed = speed/60;		//	r/min -> r/s
	float period = (1000*10/(msteps/10))/speed;		//	100us/step
	return period;
}

/*输出每步*/
int one_step(char type,int period)
{
	x_0_flag = !P1_3; 	//右0位
	x_1_flag = !P1_4;	//左限位

	P4_1 = 0;
	if(x_1_flag)
	{
		x_ok = 0;
		return 1;
	}
	if(x_0_flag && !x_ok)		//x在0️位且x不可动作
	{
		return 1;
	}
	if(type == 0)		//电机x
	{
		P0_1 = 0;
		delay_100us(period);	//速度转换成周期
		P0_1 = 1;
		delay_100us(period);	//速度转换成周期
	}
	if(type == 1)		//电机y1
	{
		P0_4 = 0;
		delay_100us(period);	//速度转换成周期
		P0_4 = 1;
		delay_100us(period);	//速度转换成周期
	}
	if(type == 2)		//电机y2
	{
		P0_6 = 0;
		delay_100us(period);	//速度转换成周期
		P0_6 = 1;
		delay_100us(period);	//速度转换成周期
	}

	return 0;
	
}

void direct_acc(int speed)
{
	int minspeed = 20;
	if(speed>maxspeed)
		speed = maxspeed;
	for(int i = 0;i<variable;i++)
	{
		motor_acc[i] = minspeed + i*(speed-minspeed)/variable;
		motor_acc[i] = (1000*10/(msteps/10))/(motor_acc[i]/60);
        motor_acc[i] = motor_acc[i]/2;
		if(motor_acc[i] < 10)
			motor_acc[i] = 10;
		if(motor_acc[i] > 50)
			motor_acc[i] = 50;
	}
}

/*控制步进电机旋转（不变速）*/
int rotate_motor(char type,unsigned int steps, unsigned int speed)
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

/* 计算速度变化曲线 输入速度，输出周期 储存在s_curve_speeds[i]*/
// void  calculate_s_curve_periods(float speed)
// {
// 	float minspeed = 20;
//     for (int i = 0; i < variable; i++)
//     {
//         float t = (float)i / (variable - 1);
// 			s_curve_speeds[i] = speed * (3 * int_pow(t, 2) - 2 * int_pow(t, 3));
// 			if(s_curve_speeds[i]>speed)
// 				s_curve_speeds[i] = speed;
// 			if(s_curve_speeds[i]<minspeed);
// 				s_curve_speeds[i] = minspeed;
// 			s_curve_speeds[i] = ((1000*10/msteps)/(s_curve_speeds[i]/60))/2;

// 			s_curve_speeds_slow[i] =  speed * (1 - (3 * int_pow(t, 2) - 2 * int_pow(t, 3)));
// 			if(s_curve_speeds_slow[i]<minspeed);
// 				s_curve_speeds_slow[i] = minspeed;
// 			if(s_curve_speeds_slow[i]>speed)
// 				s_curve_speeds_slow[i] = speed;
// 			s_curve_speeds_slow[i] = ((1000*10/msteps)/(s_curve_speeds[i]/60))/2;
//     }
// }


/* 电机曲线变速 acc==1 加速 0减速 */
int variable_speed_motor(char type, unsigned int steps, __bit acc)
{
	if(acc)
	{
		for (int i = 0; i < variable; i++) // 100级变速
		{	
			for (int j = 0; j < steps / variable; j++)
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
			for (int j = 0; j < steps / variable; j++)
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


/* 步进电机控制（电机种类012，距离：单位mm，方向0|1，速度r/min） */	
void contorlMotor(char type,float distance,__bit dir, unsigned int setspeed) 
{
	 unsigned int accsteps;
	 unsigned int decsteps;
	 float circle = distance/90;
	 float steps = msteps*circle;

	 if(steps>=1600)		//加减速步数
	 {
	 	accsteps = 800;
	  	decsteps = 800;
		steps = steps - 1600;
	 }
	 else //距离过短，加减速平分
	 {
		accsteps = steps/2;
		decsteps = steps/2;
		steps = 0;
	 }
	 if(steps <= 0)
	 	steps = 0;

	P0_0 = dir;
	P0_2 = 0;
	P0_3 = dir;
	
	
	//calculate_s_curve_periods(setspeed);		//计算加速速度曲线，存入xdata s_cuclate_speed[variable]中
	direct_acc(setspeed);
	if(interruptButtonFlag == 1 || variable_speed_motor(type,accsteps,1))		//判断中断
	{

		return ;
	}
	if(interruptButtonFlag == 1 || rotate_motor(type,steps,setspeed) )
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


//移动到目标
int move_to_target(Point* current,Point* target)
{
	float distence_x = target->x - current->x;
	float distence_y1 = target->y1 - current->y1;

	distence_x>0?contorlMotor(0,distence_x,motor_left,100):contorlMotor(0,-distence_x,motor_right,100);
	//distence_y1>0?contorlMotor(1,distence_y1,y1_up,100):contorlMotor(1,-distence_x,y1_down,100);

	current->x = target->x;
	current->y1 = target->y1;

	return 0;
}