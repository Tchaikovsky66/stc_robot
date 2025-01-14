#include <stdio.h>
/**
 * @brief 初始化S型加速查找表
 * @param max_speed 最大速度mm/s
 * @note 每次运动前调用以更新加速表
 */
#define ACC_TABLE_SIZE 600

int s1_points, s2_points, s3_points;

void InitSCurveTable(float max_speed)
{
    // 常量保持不变
    const float min_speed = 20.0;
    const float jerk = 2000;
    const float max_acc = 400;
    // const float max_acc = max_speed;
    // const float jerk = 0.1 * max_acc + 400;

    // 所有局部变量都使用xdata
    float t;
    float v;
    float v2;

    int i;

    float Time1, Time2, Time3;
    float v1;
    // 计算时间参数
    Time1 = max_acc / jerk; // 0.5
    v1 = min_speed + 0.5 * jerk * Time1 * Time1;
    Time2 = (max_speed - (min_speed + jerk * Time1 * Time1)) / max_acc; // 300-(10+600*0.5*0.5)   140/300
    Time3 = Time1;
    printf("Time1 = %f\r\n", Time1);
    printf("Time2 = %f\r\n", Time2);
    printf("Time3 = %f\r\n", Time3);

    s1_points = (int)(ACC_TABLE_SIZE * Time1 / (Time1 + Time2 + Time3)); //  0.5/ (44/30)
    s2_points = ACC_TABLE_SIZE - 2 * s1_points;
    s3_points = s1_points;

    printf("s1_points = %d\r\n", s1_points);
    printf("s2_points = %d\r\n", s2_points);
    printf("s3_points = %d\r\n", s3_points);
    // 1. 加加速段
    for (i = 0; i < s1_points; i++)
    {
        t = (float)i * Time1 / s1_points;
        v = min_speed + 0.5 * jerk * t * t;
        //  s_curve_table[i] = CalculateDelay(v);
    }

    // 2. 匀加速段
    v2 = v1 + max_acc * Time2; // 第二段末速度
    for (i = 0; i < s2_points; i++)
    {
        t = (float)i * Time2 / s2_points;
        v = v1 + max_acc * t;
        //   s_curve_table[i + s1_points] = CalculateDelay(v);
    }

    // 3. 减加加速段
    for (i = 0; i < s1_points; i++)
    {
        t = (float)i * Time3 / s1_points;
        v = v2 + max_acc * t - 0.5 * jerk * t * t;
        if (v > max_speed)
            v = max_speed;
        //   s_curve_table[i + s1_points + s2_points] = CalculateDelay(v);
    }

    // 计算三段位移
    float s1 = 0, s2 = 0, s3 = 0;
    float total_distance = 0;

    // 1. 加加速段位移
    // s1 = (1/6)*j*t^3
    s1 = (1.0 / 6.0) * jerk * Time1 * Time1 * Time1;
    printf("s1 = %f\r\n", s1);
    // 2. 匀加速段位移
    // s2 = v1*t + (1/2)*a*t^2
    s2 = v1 * Time2 + 0.5 * max_acc * Time2 * Time2;
    printf("s2 = %f\r\n", s2);
    // 3. 减加加速段位移
    // s3 = v2*t + a*t^2/2 - j*t^3/6
    s3 = v2 * Time3 + max_acc * Time3 * Time3 / 2.0 - jerk * Time3 * Time3 * Time3 / 6.0;
    printf("s1 = %f\r\n", s1);
    // 总位移
    total_distance = s1 + s2 + s3;

    printf("total_distance = %f\r\n", total_distance);
}
int main()
{
    while (1)
    {
        int max_speed;
        printf("请输入最大速度mm/s：");
        scanf("%d", &max_speed);
        InitSCurveTable(max_speed);
    }
}