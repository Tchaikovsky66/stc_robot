#include <stdio.h>
int steps;
float delay_10us;
void CalculateStepsAndDelay(float distance_cm, float speed_cm_per_s, int *steps, float *delay_10us)
{
    // ÿת2000�����ƶ�9cm
    const float steps_per_revolution = 2000.0;
    const float distance_per_revolution_cm = 10.0;
    
    // �����ܲ���
    *steps = (int)((distance_cm / distance_per_revolution_cm) * steps_per_revolution);
    
    // ������ʱʱ�䣬�ٶȵ�λΪcm/s��ת��Ϊ��/s
    float steps_per_second = (speed_cm_per_s * steps_per_revolution) / distance_per_revolution_cm;
    
    // ÿ����Ҫ����ʱʱ�䣬��λΪ10΢��
    *delay_10us = (int)(1000000.0 / steps_per_second/2);
    
    // ȷ����ʱʱ������Ϊ1����10us��
    if (*delay_10us < 1)
    {
        *delay_10us = 1;
    }

    // ��ע��֧�ֵ�����ٶ�
    // ����ٶ�ȡ������С��ʱʱ�䣬��10us
    // ����ٶ� = (10cm / 2000��) * (1000000us / 10us) cm/s
    // ���������ٶ�Ϊ500 cm/s
}

int main()
{
    CalculateStepsAndDelay(10,5,&steps,&delay_10us);
    printf("steps = %d\r\ndelay_10us = %f\r\n",steps,delay_10us);
}