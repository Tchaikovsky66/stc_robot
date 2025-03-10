#include <position.h>
#include <motor.h>
#include <delay.h>
#include <config.h>
#include <uart.h>
#include "../../include/main.h"
#include "../../include/stc15.h"
#include "position.h"

// 当前位置
volatile int sign0_x = 0;
volatile int sign0_y = 0;
volatile int sign0_y2 = 0;
// 临时目标位置
__xdata int target_x = 0;
__xdata int target_y = 0;
__xdata int target_y2 = 0;

// 参考点位置
__xdata int sign1_x = 0;
__xdata int sign1_y = 0;
__xdata int sign1_y2 = 0;
__xdata int sign2_x = 0;
__xdata int sign2_y = 0;
__xdata int sign2_y2 = 0;
__xdata int sign3_x = 0;
__xdata int sign3_y = 0;
__xdata int sign3_y2 = 0;
// 标志位
volatile __bit sign1_flag = 0;
volatile __bit sign2_flag = 0;
volatile __bit sign3_flag = 0;

/**
 * @brief 移动到指定坐标位置,更新当前坐标
 * @param target_x 目标X坐标
 * @param target_y 目标Y坐标
 * @param target_y2 目标Y2坐标
 * @param speed 移动速度
 * @note 先移动X轴再移动Y轴，移动完成后会更新sign0_x和sign0_y
 */
char MoveToPosition(int target_x, int target_y, int target_y2, int speed)
{
    static int x_distance;
    static unsigned char x_direction;
    static int y_distance;
    static unsigned char y_direction;
    static int y2_distance;
    static unsigned char y2_direction;

    // 计算X轴移动距离和方向
    x_distance = target_x - sign0_x;
    y_distance = target_y - sign0_y;
    y2_distance = target_y2 - sign0_y2;

    if (x_distance != 0)
    {
        if (x_distance > 0)
        {
            x_direction = GO_LEFT;
        }
        else
        {
            x_direction = GO_RIGHT;
            x_distance = -x_distance;
        }

        DelayMs(10);
        if (MotorGo(X_MOTOR, x_direction, x_distance, speed) == ERR)
        {
            return ERR;
        }
        sign0_x = target_x;
        DelayMs(10);
    }

    // 计算Y轴移动距离和方向

    if (y_distance != 0)
    {
        if (y_distance > 0)
        {
            y_direction = GO_DOWN;
        }
        else
        {
            y_direction = GO_UP;
            y_distance = -y_distance;
        }

        DelayMs(10);
        if (MotorGo(Y1_MOTOR, y_direction, y_distance, speed) == ERR)
        {
            return ERR;
        }
        sign0_y = target_y;
        DelayMs(10);
    }

    // 增加Y2轴控制

    if (y2_distance != 0)
    {
        if (y2_distance > 0)
        {
            y2_direction = GO_DOWN;
        }
        else
        {
            y2_direction = GO_UP;
            y2_distance = -y2_distance;
        }

        DelayMs(10);
        if (MotorGo(Y2_MOTOR, y2_direction, y2_distance, speed) == ERR)
        {
            return ERR;
        }
        sign0_y2 = target_y2;
        DelayMs(10);
    }

    UploadData(0x21 * 2, sign0_x);
    UploadData(0x22 * 2, sign0_y);
    UploadData(0x23 * 2, sign0_y2);
    return OK;
}

/**
 * @brief 上传当前坐标值到显示屏
 * @param x X轴坐标值
 * @param y Y轴坐标值
 * @param y2 Y2轴坐标值
 * @note 将当前位置坐标上传到显示屏对应地址
 */
void UploadPosition(char num, int x, int y, int y2)
{

    switch (num)
    {
    case 0:
        sign0_x = x;
        sign0_y = y;
        sign0_y2 = y2;
        break;
    case 1:
        // 上传X轴坐标
        UploadData(0x18 * 2, x);
        DelayMs(3);

        // 上传Y轴坐标
        UploadData(0x19 * 2, y);
        DelayMs(3);

        // 上传Y2轴坐标
        UploadData(0x1A * 2, y2);
        DelayMs(3);
        break;

    case 2:
        // 上传X轴坐标

        UploadData(0x1B * 2, x);
        DelayMs(3);

        UploadData(0x1C * 2, y);
        DelayMs(3);

        UploadData(0x1D * 2, y2);
        DelayMs(3);
        break;

    case 3:
        // 上传X轴坐标

        UploadData(0x1E * 2, x);
        DelayMs(3);

        UploadData(0x1F * 2, y);
        DelayMs(3);

        UploadData(0x20 * 2, y2);
        DelayMs(3);
        break;
    }
}
