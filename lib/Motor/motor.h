#ifndef _MOTOR_H
#define _MOTOR_H

void Motor_init(void);
char MotorGo(unsigned char num, unsigned char dir, int distance_mm, int speed_mm_per_s);
extern char Allow_Catch;
#endif // !_MOTOR_H