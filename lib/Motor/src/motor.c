#include <motor.h>

#define variable 100 // 100级变速

volatile float minperiod = 4.0;  //4*100us
volatile float msteps = 800.0;
volatile float maxspeed = 150.0; //187.5
// 定义数组来存储预先计算的S型速度值
__xdata float s_curve_speeds[variable];
__xdata float s_curve_speeds_slow[variable];

/*整数次幂函数*/
float int_pow(float base, int exponent) 
{
    float result = 1.0;
    int i;
    for (i = 0; i < exponent; ++i) {
        result *= base;
    }
    return result;
}

/*速度-周期转换 rotate_motor使用*/
float speed_to_period(float speed)		//	100us/step
{
	speed = speed/60;		//	r/min -> r/s
	float period = (1000*10/msteps)/speed;		//	100us/step
	return period;
}

/*输出每步*/
void one_step(char type,int period)
{
	if(type == 0)		//电机x
	{
		P1_0 = 0;
		delay_100us(period);	//速度转换成周期
		P1_0 = 1;
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
	
}

/*控制步进电机旋转（不变速）*/
void rotate_motor(char type,unsigned int steps,__bit dir, unsigned int speed)
{
	if(type == 0)
	{
		P1_1 = dir;
	}
	if(type == 1)
	{
		P0_3 = dir;
	}
	if(type == 2)
	{
		P0_5 = dir;
	}
	
	int currentperiod = speed_to_period(speed)/2;
	for(int i = 0;i < steps; i++)
	{
		if(interruptButtonFlag)	//外部中断
		{
			return;
		}
		else
		{
			one_step(type, currentperiod);
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
void variable_speed_motor(char type, unsigned int steps, __bit acc, __bit dir)
{
	if(type == 0)
	{
		P1_1 = dir;
	}
	if(type == 1)
	{
		P0_3 = dir;
	}
	if(type == 2)
	{
		P0_5 = dir;
	}
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
            one_step(type, currentperiod);
        }
    }
}


/* 步进电机控制（电机种类012，距离：单位mm，方向0|1，速度r/min） */	
void contorlMotor(char type,float distance,__bit dir, unsigned int setspeed) 
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
		variable_speed_motor(type,accsteps,1,dir);
	if(interruptButtonFlag == 0)
		rotate_motor(type,steps,dir,setspeed);
	if(interruptButtonFlag == 0)
		variable_speed_motor(type,decsteps,0,dir);
	if(interruptButtonFlag == 1)
	{
		interruptButtonFlag = 0;
		return;

	}
}
