#include <stdio.h>

#define MIN_SPEED 20
#define MAX_SPEED 1000
#define variable 100
int motor_acc[100];
float msteps = 3200.0;

volatile int x_flag = 0;


int one()
{
    if(x_flag)
    {
        return 1;
    }

    else
        printf("one()\n");
    return 0;
}

int two()
{
    if(one())
    {
        return 1;
    }
    else
        printf("two\n");
    return 0;
}

void test_speed()
{
    int msteps = 2000;
    int period = 2000000;

    
    int speed = (1/period)/msteps;      //  steps/s
    int liner_speed = speed*(90/msteps);        //  mm/s
    
    int steps;
    int liner = steps*(90/msteps);

    for(int i = 0;i<100; i++)
    {
        printf("%d/n",liner_speed);
    }
}
float calculate_pulse_period(float speed_mm_per_sec) {
    // 定义常量
    const float STEPS_PER_REVOLUTION = 2000.0;  // 每转步数
    const float MM_PER_REVOLUTION = 90.0;       // 每转移动距离（毫米）
    
    // 计算每秒需要的步数
    float steps_per_second = (speed_mm_per_sec / MM_PER_REVOLUTION) * STEPS_PER_REVOLUTION;
    
    // 计算脉冲周期（秒）
    float pulse_period_sec = 1.0 / steps_per_second;
    
    // 将周期转换为微秒
    float pulse_period_us = pulse_period_sec * 100000.0/2;
    unsigned long pulse_period_us_int = (unsigned long)pulse_period_us;
    
    return pulse_period_us;
}

// 生成直线加速数组
void generate_linear_acceleration(float setspeed) {
    // 确保setspeed在最大最小速度之间
    if (setspeed < MIN_SPEED) setspeed = MIN_SPEED;
    if (setspeed > MAX_SPEED) setspeed = MAX_SPEED;
    
    float speed_difference = setspeed - MIN_SPEED;
    float speed_increment = speed_difference / variable;
    
    for (int i = 0; i < variable; i++) {
        float current_speed = MIN_SPEED + speed_increment + i * speed_increment;
        motor_acc[i] = calculate_pulse_period(current_speed);
    }
}

int distance_to_steps(float distance_mm) {
    const float STEPS_PER_REVOLUTION = 2000.0;  // 每转步数
    const float MM_PER_REVOLUTION = 90.0;       // 每转移动距离（毫米）
    
    // 计算每步的步长（毫米）
    float step_length_mm = MM_PER_REVOLUTION / STEPS_PER_REVOLUTION;
    
    // 计算给定距离需要的步数并四舍五入到最近的整数
    int steps = (int)(distance_mm / step_length_mm + 0.5);

    return steps;
}
int main()
{
    // x_flag = 1;
    // if(two())
    //     printf("quit\n");
    // direct_acc(350);
    // for(int i = 0;i<100;i++)
    // {
    //     printf("%f\n",motor_acc[i]);
    // }
    generate_linear_acceleration(450);
    for(int i = 0;i<100;i++)
    {
        printf("%d\n",motor_acc[i]);
    }
    
    printf("steps:%d\n",distance_to_steps(350));

    int x = 0x0A;
    if(x == 10)
    {
        printf("x = 10\n");
    }
    else
    {
        printf("x != 1\n");
    }
}
