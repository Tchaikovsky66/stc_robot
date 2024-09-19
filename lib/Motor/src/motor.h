#ifndef _MOTOR_H 
#define _MOTOR_H

#include <8051.h>
#include <delay.h>
#include <uart.h>
#include <button.h>
#include <int_isr.h>

#define motor_left 1
#define motor_right 0
#define y1_up 0
#define y1_down 1
#define y2_up 0
#define y2_down 1

extern volatile float minperiod;  //4*100us
extern volatile float msteps;
extern volatile float maxspeed; //187.5
// 定义数组来存储预先计算的S型速度值
extern __xdata unsigned int motor_acc[];

extern volatile __bit x_left_flag;
extern volatile __bit x_right_flag;
extern volatile __bit x_ok;

extern volatile __bit y1_0_flag;	//检查y方向0位
extern volatile __bit y2_0_flag;	//检查y方向0位

extern volatile __bit y1_ok;		//判断y方向是否可动作
extern volatile __bit y2_ok;		//判断y方向是否可动作


extern volatile int x;
extern volatile int y1;
extern volatile int y2;


int calculate_pulse_period(float speed_mm_per_sec);
int one_step(char type, int period);
int rotate_motor(char type, unsigned int steps);
int variable_speed_motor(char type, unsigned int steps, __bit acc);
void contorlMotor(char type, int distance,__bit dir, float setspeed);

void init_position(void);
void go_position(int target_x,int target_y1,int target_y2,int setspeed);

#endif // !_MOTOR_H 