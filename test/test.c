#include <stdio.h>
int steps;
float delay_10us;
void CalculateStepsAndDelay(float distance_cm, float speed_cm_per_s, int *steps, float *delay_10us)
{
    // 每转2000步，移动9cm
    const float steps_per_revolution = 2000.0;
    const float distance_per_revolution_cm = 10.0;
    
    // 计算总步数
    *steps = (int)((distance_cm / distance_per_revolution_cm) * steps_per_revolution);
    
    // 计算延时时间，速度单位为cm/s，转换为步/s
    float steps_per_second = (speed_cm_per_s * steps_per_revolution) / distance_per_revolution_cm;
    
    // 每步需要的延时时间，单位为10微秒
    *delay_10us = (int)(1000000.0 / steps_per_second/2);
    
    // 确保延时时间至少为1（即10us）
    if (*delay_10us < 1)
    {
        *delay_10us = 1;
    }

    // 备注：支持的最大速度
    // 最大速度取决于最小延时时间，即10us
    // 最大速度 = (10cm / 2000步) * (1000000us / 10us) cm/s
    // 计算得最大速度为500 cm/s
}

int main()
{
    CalculateStepsAndDelay(10,5,&steps,&delay_10us);
    printf("steps = %d\r\ndelay_10us = %f\r\n",steps,delay_10us);
}