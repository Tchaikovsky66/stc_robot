#include <motor.h>
#include <8051.h>
#include <delay.h>
#include <uart.h>
#include "../../include/stc15.h"
#include "../../include/main.h"
#include "../../include/speed_table.h"
#include <config.h>

#define SPEED_ARRAY_SIZE 101 // 0-100共101个值
#define ACC_TABLE_SIZE 600   // 加速段点数

// 声明延迟值数组
__xdata volatile unsigned int delay_array[SPEED_ARRAY_SIZE];
char Allow_Move = 0;
char Allow_Catch = 0;
volatile int tmp_steps = 0;
volatile int target_steps = 0;
volatile int set_to_catch_steps = 0;
// 定义加速表大小
__xdata unsigned int s_curve_table[ACC_TABLE_SIZE]; // S型加速查找表

// 在文件开头添加函数声明
static unsigned int CalculateDelay(float speed_mm_per_s); // 函数声明
void InitSCurveTable(float max_speed);                    // 添加最大速度参数

// 在文件开头定义
__xdata float Time1, Time2, Time3;
__xdata float v1, v2;
__xdata int s1_points, s2_points, s3_points, total_points;
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
    // P37 推挽输出 P30-P36 准双向口

    X_STEP = 0; // x轴电机脉冲归零
    X_EN = X_EN_OFF;

    P37 = 1; // 上下使能

    Allow_Move = 1;
    // MotorGo(Y1Y2_MOTOR, GO_DOWN, 20, 200);
    MotorGo(Y1_MOTOR, GO_DOWN, 20, 100);
    MotorGo(Y2_MOTOR, GO_DOWN, 20, 100);

    Allow_Move = 0;

    if (MotorGo(Y1_MOTOR, GO_UP, 500, 100) == ERR)
    {
        Allow_Move = 1;
        MotorGo(Y1_MOTOR, GO_DOWN, 10, 50);
        Allow_Move = 0;
    }
    if (MotorGo(Y2_MOTOR, GO_UP, 500, 100) == ERR)
    {
        Allow_Move = 1;
        MotorGo(Y2_MOTOR, GO_DOWN, 10, 50);
        Allow_Move = 0;
    }
    if (MotorGo(X_MOTOR, GO_RIGHT, 1000, 100) == ERR)
    {
        Allow_Move = 1;
        Allow_Catch = 0;
        MotorGo(X_MOTOR, GO_LEFT, 200, 200);

        Allow_Move = 0; //
    }
}

// /**
//  * @brief 计算电机运动的步数和延时
//  * @param distance_cm 移动距离(厘米)
//  * @param speed_mm_per_s 移动速度(毫米/秒)
//  * @param steps 计算得到的步数
//  * @param delay_10us 计算得到的延时值(10微秒)
//  * @note 根据距离和速度计算电机运动参数
//  */
// void CalculateStepsAndDelay(float distance_cm, float speed_mm_per_s, int *steps, int *delay_10us)
// {

//     const float steps_per_revolution = 2000.0;
//     const float distance_per_revolution_cm = 100.0;

//     *steps = (int)((distance_cm / distance_per_revolution_cm) * steps_per_revolution);
//     *steps = *steps * 2;

//     float steps_per_second = (speed_mm_per_s * steps_per_revolution) / (distance_per_revolution_cm / 10);

//     *delay_10us = (int)(1000000.0 / steps_per_second / 2);

//     if (*delay_10us < 1)
//     {
//         *delay_10us = 1;
//     }
// }

char OneStep(unsigned char num, int delay)
{
    if ((X_R_LIMIT == LIMIT_RICHED) && (Allow_Move == 0))
    {
        return ERR;
    }
    if ((Y1_LIMIT == LIMIT_RICHED) && (Allow_Move == 0))
    {
        return ERR;
    }
    if ((Y2_LIMIT == LIMIT_RICHED) && (Allow_Move == 0))
    {
        return ERR;
    }
    if ((Y1_OP == LIMIT_RICHED))
    {
        target_steps = tmp_steps + 20 * (60 + end_face_distance);
        // return Y1_GOT;
        set_to_catch_steps = tmp_steps;
        if (Allow_Catch == 1)
        {
        }
        // if (tmp_steps >= target_steps)
        // {
        //     target_steps = 0;
        //     return Y1_GOT;
        // }
    }

    switch (num)
    {
    case X_MOTOR:
        X_STEP = !X_STEP;
        SetPulseInterval(delay);
        return OK;

    case Y1_MOTOR:
        Y1_STEP = !Y1_STEP;
        SetPulseInterval(delay);
        return OK;

    case Y2_MOTOR:
        Y2_STEP = !Y2_STEP;
        SetPulseInterval(delay);
        return OK;

    case Y1Y2_MOTOR:
        Y1_STEP = !Y1_STEP;
        Y2_STEP = !Y2_STEP;
        SetPulseInterval(delay);

    default:
        return ERR; // 对于未知的电机编号，返回错误
    }
}

// /**
//  * @brief 控制电机运动
//  * @param num 电机编号(X_MOTOR/Y1_MOTOR/Y2_MOTOR)
//  * @param dir 运动方向(GO_LEFT/GO_RIGHT/GO_UP/GO_DOWN)
//  * @param distance_mm 运动距离(毫米)
//  * @param speed_mm_per_s 运动速度(毫米/秒)
//  * @return char 运动状态(OK/ERR)
//  * @note 根据参数控制指定电机运动
//  */
// char MotorGo(unsigned char num, unsigned char dir, int distance_mm, int speed_mm_per_s)
// {
//     tmp_steps = 0;
//     X_EN = X_EN_ON;
//     X_DIR = dir;
//     Y1_DIR = dir;
//     Y2_DIR = dir;
//     // 计算所需步数
//     const float steps_per_revolution = 2000.0;
//     const float distance_per_revolution_mm = 100.0;
//     float min_speed = 5;

//     int steps = (int)((distance_mm / distance_per_revolution_mm) * steps_per_revolution);
//     // 计算延时数组
//     float speed_step = (speed_mm_per_s - min_speed) / (SPEED_ARRAY_SIZE - 1);
//     for (int i = 0; i < SPEED_ARRAY_SIZE; i++)
//     {
//         float current_speed = min_speed + (speed_step * i);
//         unsigned long temp = 276320UL / current_speed;
//         if (temp > 65535UL)
//         {
//             temp = 65535UL; // 限制最大值
//         }
//         delay_array[i] = 65536UL - temp;
//     }

//     X_STEP = 0;
//     int i = 0;
//     int j = 0;
//     // Uart1_SendString("x轴\r\n");
//     int acc_steps = 100;
//     if (steps < 200) // 如果距离小于200步，无匀速 ，脉冲时间*2
//     {
//         for (i = 0; i < steps / 2; i++)
//         {

//             for (j = 0; j < 2; j++)
//             {
//                 tmp_steps++;
//                 if (OneStep(num, delay_array[i] * 2) == ERR)
//                 {
//                     return ERR;
//                 }
//             }
//         }
//         for (i = steps / 2; i >= 0; i--)
//         {
//             for (j = 0; j < 2; j++)
//             {
//                 tmp_steps++;
//                 if (OneStep(num, delay_array[i] * 2) == ERR)
//                 {
//                     return ERR;
//                 }
//             }
//         }
//     }
//     else // 如果距离大于200步，先加速再匀速，最后减速
//     {
//         for (i = 0; i < 100; i++) // 加速
//         {
//             for (j = 0; j < 2; j++)
//             {
//                 tmp_steps++;
//                 if (OneStep(num, delay_array[i]) == ERR)
//                 {
//                     return ERR;
//                 }
//             }
//         }
//         for (i = 0; i < steps - 2 * acc_steps; i++) // 匀速
//         {
//             for (j = 0; j < 2; j++)
//             {
//                 tmp_steps++;
//                 if (OneStep(num, delay_array[100]) == ERR)
//                 {
//                     return ERR;
//                 }
//             }
//         }
//         for (i = 100; i >= 0; i--) // 减速
//         {
//             for (j = 0; j < 2; j++)
//             {
//                 tmp_steps++;
//                 if (OneStep(num, delay_array[i]) == ERR)
//                 {
//                     return ERR;
//                 }
//             }
//         }
//     }

//     X_EN = X_EN_OFF;
//     X_DIR = 0;
//     Y1_DIR = 0;
//     Y2_DIR = 0;
//     return OK;
// }

/**
 * @brief 初始化S型加速查找表
 * @param max_speed 最大速度mm/s
 * @note 每次运动前调用以更新加速表
 */
void InitSCurveTable(float max_speed)
{
    // 常量保持不变
    const float min_speed = 20.0;
    const float jerk = 2000.0;
    // const float max_acc = 200.0;
    const float max_acc = 400;
    // const float jerk = 0.1 * max_acc + 400;

    // 所有局部变量都使用xdata
    __xdata float t;
    __xdata float v;
    __xdata float v2;

    int i;

    // 计算时间参数
    Time1 = max_acc / jerk; // 0.5
    v1 = min_speed + 0.5 * jerk * Time1 * Time1;
    Time2 = (max_speed - (min_speed + jerk * Time1 * Time1)) / max_acc; // 300-(10+600*0.5*0.5)   140/300
    Time3 = Time1;

    s1_points = (int)(ACC_TABLE_SIZE * Time1 / (Time1 + Time2 + Time3)); //  0.5/ (44/30)
    s2_points = ACC_TABLE_SIZE - 2 * s1_points;
    s3_points = s1_points;

    // s1_points = 210;
    // s2_points = 180;
    // s3_points = 210;
    // 1. 加加速段
    for (i = 0; i < s1_points; i++)
    {
        t = (float)i * Time1 / s1_points;
        v = min_speed + 0.5 * jerk * t * t;
        s_curve_table[i] = CalculateDelay(v);
    }

    // 2. 匀加速段
    v2 = v1 + max_acc * Time2; // 第二段末速度
    for (i = 0; i < s2_points; i++)
    {
        t = (float)i * Time2 / s2_points;
        v = v1 + max_acc * t;
        s_curve_table[i + s1_points] = CalculateDelay(v);
    }

    // 3. 减加加速段
    for (i = 0; i < s1_points; i++)
    {
        t = (float)i * Time3 / s1_points;
        v = v2 + max_acc * t - 0.5 * jerk * t * t;
        if (v > max_speed)
            v = max_speed;
        s_curve_table[i + s1_points + s2_points] = CalculateDelay(v);
    }

    // 计算三段位移
    // __xdata float s1 = 0, s2 = 0, s3 = 0;
    // __xdata float total_distance = 0;

    // // 1. 加加速段位移
    // // s1 = (1/6)*j*t^3
    // s1 = (1.0/6.0) * jerk * Time1 * Time1 * Time1;

    // // 2. 匀加速段位移
    // // s2 = v1*t + (1/2)*a*t^2
    // s2 = v1 * Time2 + 0.5 * max_acc * Time2 * Time2;

    // // 3. 减加加速段位移
    // // s3 = v2*t + a*t^2/2 - j*t^3/6
    // s3 = v2 * Time3 + max_acc * Time3 * Time3 / 2.0 - jerk * Time3 * Time3 * Time3 / 6.0;

    // // 总位移
    // total_distance = s1 + s2 + s3;
}
char MotorGo(unsigned char num, unsigned char dir, int distance_mm, int speed_mm_per_s)
{

    // 计算速度比例
    float speed_ratio = 200.0 / speed_mm_per_s; // 200是生成表时的基准速度
    __xdata unsigned long total_steps = distance_mm * 20;
    __xdata int const_steps = total_steps - 2 * SPEED_TABLE_SIZE;
    __xdata unsigned int adjusted_delay;
    // 初始化电机
    tmp_steps = 0;
    X_EN = X_EN_ON;
    X_DIR = dir;
    Y1_DIR = dir;
    Y2_DIR = dir;
    if (distance_mm < 200)
    {
        for (int i = 0; i < total_steps; i++)
        {
            if (OneStep(num, CalculateDelay(speed_mm_per_s)) == ERR)
            {
                return ERR;
            }

            if (OneStep(num, CalculateDelay(speed_mm_per_s)) == ERR)
            {
                return ERR;
            }
        }
    }
    else
    {
        // 使用查表方式获取延时值
        //1.1 加速
        for (int i = 0; i < SPEED_TABLE_SIZE; i++)
        {
            // 根据速度比例调整延时值
            // adjusted_delay = (unsigned int)(delay_table[i] * speed_ratio);

            // // 限制最小和最大延时值
            // if (adjusted_delay < 1152) // 最大速度对应的延时（200mm/s）
            //     adjusted_delay = 1152;
            // if (adjusted_delay > 11520) // 最小速度对应的延时（20mm/s）
            //     adjusted_delay = 11520;

            if (OneStep(num, 65536UL - delay_table[i]) == ERR)
            {
                return ERR;
            }
            if (OneStep(num, 65536UL - delay_table[i]) == ERR)
            {
                return ERR;
            }
        }
        //2. 匀速
        for (int i = 0; i < const_steps; i++)
        {
            // 根据速度比例调整延时值
            // adjusted_delay = (unsigned int)(delay_table[SPEED_TABLE_SIZE - 1] * speed_ratio);

            // // 限制最小和最大延时值
            // if (adjusted_delay < 1152) // 最大速度对应的延时（200mm/s）
            //     adjusted_delay = 1152;
            // if (adjusted_delay > 11520) // 最小速度对应的延时（20mm/s）
            //     adjusted_delay = 11520;
            if (OneStep(num, 65536UL - delay_table[SPEED_TABLE_SIZE - 1]) == ERR)
            {
                return ERR;
            }
            if (OneStep(num, 65536UL - delay_table[SPEED_TABLE_SIZE - 1]) == ERR)
            {
                return ERR;
            }
        }
        //3.减速
        for (int i = SPEED_TABLE_SIZE - 1; i >= 0; i--)
        {
            // 根据速度比例调整延时值
            // adjusted_delay = (unsigned int)(delay_table[i] * speed_ratio);

            // // 限制最小和最大延时值
            // if (adjusted_delay < 1152) // 最大速度对应的延时（200mm/s）
            //     adjusted_delay = 1152;
            // if (adjusted_delay > 11520) // 最小速度对应的延时（20mm/s）
            //     adjusted_delay = 11520;
            if (OneStep(num, 65536UL - delay_table[i]) == ERR)
            {
                return ERR;
            }
            if (OneStep(num, 65536UL - delay_table[i]) == ERR)
            {
                return ERR;
            }
        }
    }
    X_EN = X_EN_OFF;
    X_DIR = 0;
    Y1_DIR = 0;
    Y2_DIR = 0;

    return OK;
}
/**
 * @brief S型加速电机运动控制(查表版)
 */
char MotorGo1(unsigned char num, unsigned char dir, int distance_mm, int speed_mm_per_s)
{
    // 所有局部变量都使用xdata
    __xdata unsigned long total_steps;
    __xdata int const_steps;
    //;
    __xdata unsigned int const_delay;

    // 参数检查
    if (speed_mm_per_s > 500)
        speed_mm_per_s = 500;
    if (speed_mm_per_s < 20)
        speed_mm_per_s = 20;

    // 初始化电机
    tmp_steps = 0;
    X_EN = X_EN_ON;
    X_DIR = dir;
    Y1_DIR = dir;
    Y2_DIR = dir;

    // 计算步数(上升沿个数)
    total_steps = distance_mm * 20;
    total_points = s1_points + s2_points + s3_points;

    // 计算加速表
    InitSCurveTable((float)speed_mm_per_s);

    // 计算匀速段步数
    const_steps = total_steps - (total_points) * 4;
    const_delay = s_curve_table[total_points - 1];
    // if (const_steps < 0)
    // {
    //     ratio = (float)total_steps / ((total_points) * 2);
    //     s1_points *= ratio;
    //     s2_points *= ratio;
    //     s3_points *= ratio;
    //     const_steps = 0;
    // }

    if (total_steps <= 600)
    {
        for (int i = 0; i < total_steps; i++)
        {
            if (OneStep(num, const_delay) == ERR)
                return ERR;
            if (OneStep(num, const_delay) == ERR)
                return ERR;
        }
    }
    else
    {
        // 1. 加速段 (加加速 + 匀加速 + 减加加速)
        for (int i = 0; i < total_points; i++)
        {
            if (OneStep(num, s_curve_table[i]) == ERR)
                return ERR;
            if (OneStep(num, s_curve_table[i]) == ERR)
                return ERR;
            if (OneStep(num, s_curve_table[i]) == ERR)
                return ERR;
            if (OneStep(num, s_curve_table[i]) == ERR)
                return ERR;
        }

        // 2. 匀速段
        for (int i = 0; i < const_steps; i++)
        {
            if (OneStep(num, const_delay) == ERR)
                return ERR;
            if (OneStep(num, const_delay) == ERR)
                return ERR;
        }

        // 3. 减速段 (与加速段对称)
        for (int i = total_points - 1; i >= 0; i--)
        {
            if (OneStep(num, s_curve_table[i]) == ERR)
                return ERR;
            if (OneStep(num, s_curve_table[i]) == ERR)
                return ERR;
            if (OneStep(num, s_curve_table[i]) == ERR)
                return ERR;
            if (OneStep(num, s_curve_table[i]) == ERR)
                return ERR;
        }
    }

    X_EN = X_EN_OFF;
    X_DIR = 0;
    Y1_DIR = 0;
    Y2_DIR = 0;
    return OK;
}
/**
 * @brief 计算延时值
 * @param speed_mm_per_s 当前速度(毫米/秒)
 * @param steps_per_revolution 每转步数
 * @param distance_per_revolution_mm 每转距离(毫米)
 * @return unsigned int 定时器重装值
 */
static unsigned int CalculateDelay(float v)
{
    // float steps_per_second = (speed_mm_per_s * steps_per_revolution) / distance_per_revolution_mm;
    // unsigned long count = 11059UL * 1000UL / (steps_per_second );

    // if (count > 65535UL)
    //     count = 65535UL;
    // unsigned long count = (276320UL / v) / 2;
    unsigned long count = (230400UL / v);

    return 65536UL - count;
}