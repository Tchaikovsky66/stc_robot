#include <motor.h>
#include <8051.h>
#include <delay.h>
#include <uart.h>
#include "../../include/stc15.h"
//电机初始化
void Motor_init(void)
{
    P0M0 = 0xff; P0M1 = 0x00;   //P00-P07 推挽输出

}

//计算步数和延时
void CalculateStepsAndDelay(float distance_cm, float speed_cm_per_s, int *steps, int *delay_10us)
{

    const float steps_per_revolution = 2000.0;
    const float distance_per_revolution_cm = 10.0;

    *steps = (int)((distance_cm / distance_per_revolution_cm) * steps_per_revolution);
    
 
    float steps_per_second = (speed_cm_per_s * steps_per_revolution) / distance_per_revolution_cm;
    
 
    *delay_10us = (int)(1000000.0 / steps_per_second/2);
    

    if (*delay_10us < 1)
    {
        *delay_10us = 1;
    }

}


//电机转动步数
void MotorSteps(char num ,int steps, int delay)
{
    switch (num)
    {
    case 1/* constant-expression */:
        /* code */
        {
            int i = 0;
            Uart1_SendString("x轴\r\n");
            for(i = 0;i<steps;i++)
            {
                P0_1 = !P0_1;
                Delay10Us(delay);
            }
        }
        break;
    
    default:
        break;
    }
}