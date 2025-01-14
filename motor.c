#include "speed_table.h"

char MotorGo(unsigned char num, unsigned char dir, int distance_mm, int speed_mm_per_s)
{
    // 计算速度比例
    float speed_ratio = 200.0 / speed_mm_per_s; // 200是生成表时的基准速度

    // 使用查表方式获取延时值
    for (int i = 0; i < SPEED_TABLE_SIZE; i++)
    {
        // 根据速度比例调整延时值
        unsigned int adjusted_delay = (unsigned int)(delay_table[i] * speed_ratio);

        // 限制最小和最大延时值
        if (adjusted_delay < 1152) // 最大速度对应的延时（200mm/s）
            adjusted_delay = 1152;
        if (adjusted_delay > 11520) // 最小速度对应的延时（20mm/s）
            adjusted_delay = 11520;

        if (OneStep(num, adjusted_delay) == ERR)
        {
            return ERR;
        }
    }

    return OK;
}