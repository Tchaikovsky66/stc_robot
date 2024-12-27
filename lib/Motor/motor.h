#ifndef _MOTOR_H
#define _MOTOR_H

void Motor_init(void);

void CalculateStepsAndDelay(float distance_cm, float speed_cm_per_s, int *steps, int *delay_10us);
char MotorGo(unsigned char num ,unsigned char dir, int distance_mm, int speed_mm_per_s);
extern char Allow_Catch;
#endif // !_MOTOR_H