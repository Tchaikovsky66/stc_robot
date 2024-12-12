#ifndef _MOTOR_H
#define _MOTOR_H

void Motor_init(void);

void MotorSteps(char num ,int steps, int delay);
void CalculateStepsAndDelay(float distance_cm, float speed_cm_per_s, int *steps, int *delay_10us);
char MotorGo(char num ,char dir, int distance_mm, int speed_mm_per_s);


#endif // !_MOTOR_H