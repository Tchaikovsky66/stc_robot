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


float int_pow(float base, int exponent); 
float speed_to_period(float speed);		//	100us/step
void one_step(char type, int period);
void rotate_motor(char type, unsigned int steps,__bit dir, unsigned int speed);
void calculate_s_curve_periods(float speed);
void variable_speed_motor(char type, unsigned int steps, __bit acc, __bit dir);
void contorlMotor(char type, float distance,__bit dir, unsigned int setspeed);




#endif // !_MOTOR_H 