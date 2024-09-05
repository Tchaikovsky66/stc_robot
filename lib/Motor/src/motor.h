#ifndef _MOTOR_H 
#define _MOTOR_H

#include <8051.h>
#include <delay.h>
#include <uart.h>
#include <button.h>
#include <int_isr.h>

extern volatile float minperiod;  //4*100us
extern volatile float msteps;
extern volatile float maxspeed; //187.5
// 定义数组来存储预先计算的S型速度值
extern __xdata float s_curve_speeds[];
extern __xdata float s_curve_speeds_slow[];
extern __xdata float motor_acc[];

extern volatile __bit x_0_flag;
extern volatile __bit x_1_flag;
extern volatile __bit x_ok;

typedef struct 
{
	/* data */
	int x;
	int y1;
	int y2;
}Point;

extern  Point current_position;
extern  Point target_position;


float int_pow(float base, int exponent); 
float speed_to_period(float speed);		//	100us/step
int one_step(char type, int period);
int rotate_motor(char type, unsigned int steps,unsigned int speed);
void calculate_s_curve_periods(float speed);
int variable_speed_motor(char type, unsigned int steps, __bit acc);
void contorlMotor(char type, float distance,__bit dir, unsigned int setspeed);
void init_point(Point* p,int x, int y1);
void set_target_point(Point* target, int x, int y1);
int move_to_target(Point* current,Point* target);




#endif // !_MOTOR_H 