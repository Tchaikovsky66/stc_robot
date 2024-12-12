#include <motor.h>
#include <8051.h>
#include <delay.h>
#include <uart.h>
#include "../../include/stc15.h"
#include "../../include/main.h"

#define SPEED_ARRAY_SIZE 101  // 0-100共101个值

// 声明延迟值数组
__xdata volatile int delay_array[SPEED_ARRAY_SIZE];
char Allow_Move = 0;
//电机初始化
void Motor_init(void)
{
    P0M0 = 0xff; P0M1 = 0x00;   //P00-P07 推挽输出
    P1M0 &= ~0xf8; P1M1 &= ~0xf8;   //P13-P17 准双向口

    X_STEP = 0; //x轴电机脉冲归零
    X_EN = X_EN_OFF;

    Allow_Move = 0;
    X_DIR = GO_RIGHT;
    if(MotorGo(X_MOTOR,GO_RIGHT,5000,100) == ERR)
    {
        Allow_Move = 1;
        //X_DIR = GO_LEFT;    
        //MotorGo(1,100,100);
    }


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
                X_STEP = !X_STEP;
                Delay10Us(delay);
            }
        }
        break;
    
    default:
        break;
    }
}

char OneStep(int delay)
{
    if((X_R_LIMIT == LIMIT_RICHED)&&(Allow_Move == 0))
    {
        return ERR;
    }
    
        X_STEP = !X_STEP;
        Delay10Us(delay);
        return OK;
    
    
}

//电机运动 输入（电机编号，距离mm，速度mm/s）
char MotorGo(char num ,char dir, int distance_mm, int speed_mm_per_s)
{
    X_EN = X_EN_ON;
    X_DIR = dir;
    //计算所需步数
    const float steps_per_revolution = 2000.0;
    const float distance_per_revolution_mm = 100.0;
    float min_speed = 10;

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
            X_STEP = 0;
            int i = 0;
            int j = 0;
            //Uart1_SendString("x轴\r\n");
            int acc_steps = 100;
            if(steps < 200) //如果距离小于200步，无匀速 ，脉冲时间*2
            {
                for(i = 0;i<steps/2;i++)
                {
                    for(j = 0;j<2;j++)
                    {
                        if(OneStep(delay_array[i]*2) == ERR)
                        {
                            return ERR;
                        }
                    }
                }
                for(i = steps/2;i>=0;i--)
                {
                    for(j = 0;j<2;j++)
                    {
                        if(OneStep(delay_array[i]*2) == ERR)
                        {
                            return ERR;
                        }
                    }
                }
            }
            else    //如果距离大于200步，先加速再匀速，最后减速
            {
                for(i = 0;i<100;i++)    //加速
                {
                    for(j = 0;j<2;j++)
                    {
                        if(OneStep(delay_array[i]) == ERR)
                        {
                            return ERR;
                        }
                    }
                }
                for(i = 0;i<steps-2*acc_steps;i++)    //匀速
                {
                    for(j = 0;j<2;j++)
                    {
                        if(OneStep(delay_array[100]) == ERR)
                        {
                            return ERR;
                        }
                    }
                }
                for(i = 100;i>=0;i--)    //减速
                {
                    for(j = 0;j<2;j++)
                    {
                        if(OneStep(delay_array[i]) == ERR)
                        {
                            return ERR;
                        }
                    }
                }
            }
        }
        break;
    
    default:
        break;

    
    }
    X_EN = X_EN_OFF;
    X_DIR = 0;
    return OK;
}