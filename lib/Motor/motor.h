#ifndef _MOTOR_H
#define _MOTOR_H

void MotorSteps(char num ,int steps, int delay);
void CalculateStepsAndDelay(float distance_cm, float speed_cm_per_s, int *steps, int *delay_10us);


#endif // !_MOTOR_H