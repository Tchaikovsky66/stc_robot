#include <motor.h>
#include <8051.h>
#include <delay.h>
#include <uart.h>
#include "../../include/stc15.h"

#define SPEED_ARRAY_SIZE 101  // 0-100共101个值

// 声明延迟值数组
__xdata volatile int delay_array[SPEED_ARRAY_SIZE];

//电机初始化
void Motor_init(void)
{
    P0M0 = 0xff; P0M1 = 0x00;   //P00-P07 推挽输出
    P0_1 = 0; //x轴电机脉冲归零

}

//计算步数和延时
void CalculateStepsAndDelay(float distance_cm, float speed_mm_per_s, int *steps, int *delay_10us)
{

    const float steps_per_revolution = 2000.0;
    const float distance_per_revolution_cm = 100.0;

    *steps = (int)((distance_cm / distance_per_revolution_cm) * steps_per_revolution);
    *steps = *steps*2;
 
    float steps_per_second = (speed_mm_per_s * steps_per_revolution) / (distance_per_revolution_cm/10);
    
 
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
            //Uart1_SendString("x轴\r\n");
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

//电机运动 输入（电机编号，距离mm，速度mm/s）
void MotorGo(char num ,int distance_mm, int speed_mm_per_s)
{
    //计算所需步数
    const float steps_per_revolution = 2000.0;
    const float distance_per_revolution_mm = 100.0;
    float min_speed = 20;

    int steps = (int)((distance_mm / distance_per_revolution_mm) * steps_per_revolution);
    //计算延时数组
    float speed_step = (speed_mm_per_s - min_speed) / (SPEED_ARRAY_SIZE - 1);
    for(int i = 0; i < SPEED_ARRAY_SIZE; i++) 
    {
        float current_speed = min_speed + (speed_step * i);
        int current_10us_delay;


        float steps_per_second = (current_speed * steps_per_revolution) / (distance_per_revolution_mm/10);
        current_10us_delay = (int)(1000000.0 / steps_per_second/2);
        if(current_10us_delay < 1)
        {
            current_10us_delay = 1;
        }
        delay_array[i] = current_10us_delay;

    }
    switch (num)
    {
    case 1/* constant-expression */:
        /* code */
        {
            P0_1 = 0;
            int i = 0;
            //Uart1_SendString("x轴\r\n");
            int acc_steps = 100;
            if(steps < 200) //如果距离小于200步，无匀速 ，脉冲时间*2
            {
                for(i = 0;i<steps/2;i++)
                {
                    P0_1 = !P0_1;
                    Delay10Us(delay_array[i]*2);
                    P0_1 = !P0_1;
                    Delay10Us(delay_array[i]*2);
                }
                for(i = steps/2;i>=0;i--)
                {
                    P0_1 = !P0_1;
                    Delay10Us(delay_array[i]*2);
                    P0_1 = !P0_1;
                    Delay10Us(delay_array[i]*2);
                }
            }
            else    //如果距离大于200步，先加速再匀速，最后减速
            {
                for(i = 0;i<100;i++)    //加速
                {
                    P0_1 = !P0_1;
                    Delay10Us(delay_array[i]);
                    P0_1 = !P0_1;
                    Delay10Us(delay_array[i]);
                }
                for(i = 0;i<steps-2*acc_steps;i++)    //匀速
                {
                    P0_1 = !P0_1;
                    Delay10Us(delay_array[100]);
                    P0_1 = !P0_1;
                    Delay10Us(delay_array[100]);
                }
                for(i = 100;i>=0;i--)    //减速
                {
                    P0_1 = !P0_1;
                    Delay10Us(delay_array[i]);
                    P0_1 = !P0_1;
                    Delay10Us(delay_array[i]);
                }
            }
        }
        break;
    
    default:
        break;
    }
}