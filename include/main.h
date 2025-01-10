#ifndef _MAIN_H
#define _MAIN_H

#define ERR 0
#define FINISH 0
#define OK 1

#define Y1_GOT 0
#define Y2_GOT 3

#define MODEL 0x50
#define UP_DOWN_SPEED 0x4A
#define UP_DOWN_DISTANCE 0x46
#define LEFT_RIGHT_SPEED 0x48
#define LEFT_RIGHT_DISTANCE 0x44

#define LIMIT_RICHED 0
#define GO_LEFT 1
#define GO_RIGHT 0
#define GO_UP 0
#define GO_DOWN 1

#define X_MOTOR 1
#define Y1_MOTOR 2
#define Y2_MOTOR 3
#define Y1Y2_MOTOR 4

#define CORE_DIAMETER_ADDRESS 0x29*2
#define END_FACE_DISTANCE_ADDRESS 0x2A*2

__sbit __at(0x93) X_R_LIMIT; // 0到达限位 ，1未到达 P13
__sbit __at(0x94) X_L_LIMIT; // 0到达限位 ，1未到达 P14
__sbit __at(0xB4) Y1_LIMIT;  // 0到达限位 ，1未到达 P34
__sbit __at(0xB3) Y1_OP;  // y1工件检测 P33
__sbit __at(0xB5) Y2_LIMIT;  // 0到达限位 ，1未到达 P35
__sbit __at(0xB6) Y2_OP;  // y2工件检测 P36
__sbit __at(0x80) X_DIR;     // 1向左，0向右  P00
__sbit __at(0x81) X_STEP;    // 脉冲输出  P01
__sbit __at(0x82) X_EN;      // 0使能，1不使能  P02
__sbit __at(0x83) Y1_DIR;    // 1向左，0向右  P03
__sbit __at(0x84) Y1_STEP;   // 脉冲输出  P04
__sbit __at(0x85) Y2_DIR;    // 1向左，0向右  P05
__sbit __at(0x86) Y2_STEP;   // 脉冲输出  P06
__sbit __at(0xC1) EN_485;    // 使能485发送

#define X_EN_ON 0
#define X_EN_OFF 1
extern char Allow_Move; // 0不移动，1移动
extern volatile int tmp_steps;

extern volatile int set_to_catch_steps;
/*
uart
*/
/*
delay
*/

#endif // !_MAIN_H