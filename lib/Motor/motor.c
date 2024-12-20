#include <motor.h>
#include <8051.h>
#include <delay.h>
#include <uart.h>
#include "../../include/stc15.h"
#include "../../include/main.h"

#define SPEED_ARRAY_SIZE 101 // 0-100共101个值

// 声明延迟值数组
__xdata volatile int delay_array[SPEED_ARRAY_SIZE];
char Allow_Move = 0;
/**
 * @brief 电机初始化
 * @note 设置端口模式，初始化电机状态，执行回零操作
 */
void Motor_init(void)
{
    P0M0 = 0xff;
    P0M1 = 0x00; // P00-P07 推挽输出
    P1M0 &= ~0xf8;
    P1M1 &= ~0xf8; // P13-P17 准双向口

    P3M0 |= 0x80;
    P3M1 &= ~0x80;
    // P37 推挽输出

    X_STEP = 0; // x轴电机脉冲归零
    X_EN = X_EN_OFF;

    P37 = 1;

    Allow_Move = 0;
    if (MotorGo(X_MOTOR, GO_RIGHT, 5000, 100) == ERR)
    {
        Allow_Move = 1;
        X_DIR = GO_LEFT;
        MotorGo(X_MOTOR, GO_LEFT, 100, 100);
    }
}

/**
 * @brief 计算电机运动的步数和延时
 * @param distance_cm 移动距离(厘米)
 * @param speed_mm_per_s 移动速度(毫米/秒)
 * @param steps 计算得到的步数
 * @param delay_10us 计算得到的延时值(10微秒)
 * @note 根据距离和速度计算电机运动参数
 */
void CalculateStepsAndDelay(float distance_cm, float speed_mm_per_s, int *steps, int *delay_10us)
{

    const float steps_per_revolution = 2000.0;
    const float distance_per_revolution_cm = 100.0;

    *steps = (int)((distance_cm / distance_per_revolution_cm) * steps_per_revolution);
    *steps = *steps * 2;

    float steps_per_second = (speed_mm_per_s * steps_per_revolution) / (distance_per_revolution_cm / 10);

    *delay_10us = (int)(1000000.0 / steps_per_second / 2);

    if (*delay_10us < 1)
    {
        *delay_10us = 1;
    }
}

// 电机转动步数
void MotorSteps(unsigned char num, int steps, int delay)
{
    switch (num)
    {
    case 1 /* constant-expression */:
        /* code */
        {
            int i = 0;
            // Uart1_SendString("x轴\r\n");
            for (i = 0; i < steps; i++)
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

char OneStep(unsigned char num, int delay)
{
    if ((X_R_LIMIT == LIMIT_RICHED) && (Allow_Move == 0))
    {
        return ERR;
    }

    switch (num)
    {
    case X_MOTOR:
        X_STEP = !X_STEP;
        Delay10Us(delay);
        return OK;

    case Y1_MOTOR:
        Y1_STEP = !Y1_STEP;
        Delay10Us(delay);
        return OK;

    case Y2_MOTOR:
        Y2_STEP = !Y2_STEP;
        Delay10Us(delay);
        return OK;

    default:
        return ERR; // 对于未知的电机编号，返回错误
    }
}

/**
 * @brief 控制电机运动
 * @param num 电机编号(X_MOTOR/Y1_MOTOR/Y2_MOTOR)
 * @param dir 运动方向(GO_LEFT/GO_RIGHT/GO_UP/GO_DOWN)
 * @param distance_mm 运动距离(毫米)
 * @param speed_mm_per_s 运动速度(毫米/秒)
 * @return char 运动状态(OK/ERR)
 * @note 根据参数控制指定电机运动
 */
char MotorGo(unsigned char num, unsigned char dir, int distance_mm, int speed_mm_per_s)
{
    X_EN = X_EN_ON;
    X_DIR = dir;
    Y1_DIR = dir;
    Y2_DIR = dir;
    // 计算所需步数
    const float steps_per_revolution = 2000.0;
    const float distance_per_revolution_mm = 100.0;
    float min_speed = 5;

    int steps = (int)((distance_mm / distance_per_revolution_mm) * steps_per_revolution);
    // 计算延时数组
    float speed_step = (speed_mm_per_s - min_speed) / (SPEED_ARRAY_SIZE - 1);
    for (int i = 0; i < SPEED_ARRAY_SIZE; i++)
    {
        float current_speed = min_speed + (speed_step * i);
        int current_10us_delay;

        float steps_per_second = (current_speed * steps_per_revolution) / (distance_per_revolution_mm / 10);
        current_10us_delay = (int)(1000000.0 / steps_per_second / 2);
        if (current_10us_delay < 1)
        {
            current_10us_delay = 1;
        }
        delay_array[i] = current_10us_delay;
    }

    X_STEP = 0;
    int i = 0;
    int j = 0;
    // Uart1_SendString("x轴\r\n");
    int acc_steps = 100;
    if (steps < 200) // 如果距离小于200步，无匀速 ，脉冲时间*2
    {
        for (i = 0; i < steps / 2; i++)
        {
            for (j = 0; j < 2; j++)
            {
                if (OneStep(num, delay_array[i] * 2) == ERR)
                {
                    return ERR;
                }
            }
        }
        for (i = steps / 2; i >= 0; i--)
        {
            for (j = 0; j < 2; j++)
            {
                if (OneStep(num, delay_array[i] * 2) == ERR)
                {
                    return ERR;
                }
            }
        }
    }
    else // 如果距离大于200步，先加速再匀速，最后减速
    {
        for (i = 0; i < 100; i++) // 加速
        {
            for (j = 0; j < 2; j++)
            {
                if (OneStep(num, delay_array[i]) == ERR)
                {
                    return ERR;
                }
            }
        }
        for (i = 0; i < steps - 2 * acc_steps; i++) // 匀速
        {
            for (j = 0; j < 2; j++)
            {
                if (OneStep(num, delay_array[100]) == ERR)
                {
                    return ERR;
                }
            }
        }
        for (i = 100; i >= 0; i--) // 减速
        {
            for (j = 0; j < 2; j++)
            {
                if (OneStep(num, delay_array[i]) == ERR)
                {
                    return ERR;
                }
            }
        }
    }

    X_EN = X_EN_OFF;
    X_DIR = 0;
    Y1_DIR = 0;
    Y2_DIR = 0;
    return OK;
}