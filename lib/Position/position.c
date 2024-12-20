#include <position.h>
#include <motor.h>
#include <delay.h>
#include <config.h>
#include <uart.h>
#include "../../include/main.h"
#include "../../include/stc15.h"

// 当前位置
__xdata volatile int current_pos_x = 0;
__xdata volatile int current_pos_y = 0;

// 临时目标位置
__xdata int target_x = 0;
__xdata int target_y = 0;

// 参考点位置
__xdata int sign1_x = 0;
__xdata int sign1_y = 0;
__xdata int sign2_x = 0;
__xdata int sign2_y = 0;
__xdata int sign3_x = 0;
__xdata int sign3_y = 0;

// 标志位
volatile __bit sign1_flag = 0;
volatile __bit sign2_flag = 0;
volatile __bit sign3_flag = 0;

/**
 * @brief 移动到指定坐标位置
 * @param target_x 目标X坐标
 * @param target_y 目标Y坐标
 * @param speed 移动速度
 * @note 先移动X轴再移动Y轴，移动完成后会更新current_pos_x和current_pos_y
 */
void MoveToPosition(int target_x, int target_y, int speed)
{
    static int x_distance;
    static unsigned char x_direction;
    static int y_distance;
    static int y_direction;

    // 计算X轴移动距离和方向
    x_distance = target_x - current_pos_x;

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
        MotorGo(X_MOTOR, x_direction, x_distance, speed);
        current_pos_x = target_x;
        DelayMs(10);
    }

    // 计算Y轴移动距离和方向
    y_distance = target_y - current_pos_y;

    if (y_distance != 0)
    {
        if (y_distance > 0)
        {
            y_direction = GO_UP;
        }
        else
        {
            y_direction = GO_DOWN;
            y_distance = -y_distance;
        }

        DelayMs(10);
        MotorGo(Y1_MOTOR, y_direction, y_distance, speed);
        current_pos_y = target_y;
        DelayMs(10);
    }
}

/**
 * @brief 设置并移动到指定参考点
 * @param point_x 要设置的参考点X坐标
 * @param point_y 要设置的参考点Y坐标
 * @param point_num 参考点编号(1-3)
 * @param speed 移动速度
 * @return char 设置结果(OK/ERR)
 * @note 移动到指定位置并更新显示屏上的坐标值
 */
char SetReferencePoint(int point_x, int point_y, unsigned char point_num, int speed)
{
    switch (point_num)
    {
    case 1:
        sign1_x = point_x;
        sign1_y = point_y;
        MoveToPosition(sign1_x, sign1_y, speed);
        UploadData(0x19 * 2, sign1_x);
        UploadData(0x1A * 2, sign1_y);
        DelayMs(10);
        break;

    case 2:
        sign2_x = point_x;
        sign2_y = point_y;
        MoveToPosition(sign2_x, sign2_y, speed);
        UploadData(0x1B * 2, sign2_x);
        UploadData(0x1C * 2, sign2_y);
        DelayMs(10);
        break;

    case 3:
        sign3_x = point_x;
        sign3_y = point_y;
        MoveToPosition(sign3_x, sign3_y, speed);
        UploadData(0x1D * 2, sign3_x);
        UploadData(0x1E * 2, sign3_y);
        DelayMs(10);
        break;

    default:
        return ERR;
    }

    DelayMs(10);
    return OK;
}

// UpdateReferencePoint移到函数外部作为静态函数
static void UpdateReferencePoint(unsigned char point_num, int target_x, int target_y, int speed)
{
    switch (point_num)
    {
    case 1:
        sign1_x = target_x;
        sign1_y = target_y;
        MoveToPosition(target_x, target_y, speed);
        UploadData(0x19 * 2, sign1_x);
        UploadData(0x1A * 2, sign1_y);
        break;

    case 2:
        sign2_x = target_x;
        sign2_y = target_y;
        MoveToPosition(target_x, target_y, speed);
        UploadData(0x1B * 2, sign2_x);
        UploadData(0x1C * 2, sign2_y);
        break;

    case 3:
        sign3_x = target_x;
        sign3_y = target_y;
        MoveToPosition(target_x, target_y, speed);
        UploadData(0x1D * 2, sign3_x);
        UploadData(0x1E * 2, sign3_y);
        break;
    }
    UploadData(0x16 * 2, 0);
}

/**
 * @brief 处理参考点设置程
 * @param left_right_distance 左右移动距离
 * @param speed 移动速度
 * @return unsigned char 当前正在设置的参考点编号(1-3)
 * @note 根据各种标志位控制参考点的设置流程
 */
unsigned char HandleReferencePointSetting(int left_right_distance, int speed)
{
    static unsigned char point_num = 1;

    // if (sign3_flag)
    // {
    //     while (1)
    //     {
    //         if (RCVOK == 0xff)
    //         {
    //             update_parameters();
    //             DelayMs(10);
    //         }
    //         // // 处理移动控制
    //         if (left_flag)
    //         {
    //             P41 = 1;
    //             DelayMs(3);
    //             Uart1_SendString("go left\r\n");
    //             DelayMs(10);
    //             target_x = current_pos_x + left_right_distance;
    //             target_y = current_pos_y;
    //             UpdateReferencePoint(point_num, target_x, target_y, speed);
    //             left_flag = 0;
    //             P41 = 0;
    //         }
    //         else if (right_flag)
    //         {
    //             P41 = 1;
    //             DelayMs(3);
    //             Uart1_SendString("go right\r\n");
    //             DelayMs(10);
    //             target_x = current_pos_x - left_right_distance;
    //             target_y = current_pos_y;
    //             UpdateReferencePoint(point_num, target_x, target_y, speed);
    //             right_flag = 0;
    //             P41 = 0;
    //         }
    //         else if (up_flag)
    //         {
    //             P41 = 1;
    //             DelayMs(3);
    //             Uart1_SendString("go up\r\n");
    //             DelayMs(10);
    //             target_y = current_pos_y - left_right_distance;
    //             target_x = current_pos_x;
    //             UpdateReferencePoint(point_num, target_x, target_y, speed);
    //             up_flag = 0;
    //             P41 = 0;
    //         }
    //         else if (down_flag)
    //         {
    //             P41 = 1;
    //             DelayMs(3);
    //             Uart1_SendString("go down\r\n");
    //             DelayMs(10);
    //             target_y = current_pos_y + left_right_distance;
    //             target_x = current_pos_x;
    //             UpdateReferencePoint(point_num, target_x, target_y, speed);
    //             down_flag = 0;
    //             P41 = 0;
    //         }

    //         if(!left_flag)
    //         {
    //             break;
    //         }
    //     }
    //     P41 = 1;
    //     Uart1_SendString("sign3_flag\r\n");
    //     Uart1_SendString("参考点设置完成\r\n");
    //     DelayMs(10);
    //     point_num = 0;
    //     sign3_flag = 0;
    //     CFGBUF[0x13 * 2] = 0;
    //     P41 = 0;
    // }

    if (RCVOK == 0xff)
    {
        update_parameters();
        DelayMs(10);
    }
    // // 处理参考点切换逻辑
    if (!init_pos_flag)
    {
        return FINISH;
    }
    else if (sign3_flag)
    {
        P41 = 1;
        Uart1_SendString("sign3_flag\r\n");
        Uart1_SendString("参考点设置完成\r\n");
        DelayMs(10);
        point_num = 0;
        sign3_flag = 0;
        CFGBUF[0x13 * 2] = 0;
        P41 = 0;
    }
    else if (sign2_flag)
    {
        P41 = 1;
        Uart1_SendString("sign2_flag\r\n");
        Uart1_SendString("准备设置参考点3\r\n");
        DelayMs(10);
        point_num = 3;
        sign2_flag = 0;
        CFGBUF[0x13 * 2] = 0;
        P41 = 0;
    }
    else if (sign1_flag)
    {
        P41 = 1;
        Uart1_SendString("sign1_flag\r\n");
        Uart1_SendString("准备设置参考点2\r\n");
        DelayMs(10);
        point_num = 2;
        sign1_flag = 0;
        CFGBUF[0x13 * 2] = 0;
        P41 = 0;
    }

    // 处理移动控制
    if (left_flag)
    {
        P41 = 1;
        DelayMs(3);
        Uart1_SendString("go left\r\n");
        DelayMs(10);
        target_x = current_pos_x + left_right_distance;
        target_y = current_pos_y;
        UpdateReferencePoint(point_num, target_x, target_y, speed);
        left_flag = 0;
        P41 = 0;
    }
    else if (right_flag)
    {
        P41 = 1;
        DelayMs(3);
        Uart1_SendString("go right\r\n");
        DelayMs(10);
        target_x = current_pos_x - left_right_distance;
        target_y = current_pos_y;
        UpdateReferencePoint(point_num, target_x, target_y, speed);
        right_flag = 0;
        P41 = 0;
    }
    else if (up_flag)
    {
        P41 = 1;
        DelayMs(3);
        Uart1_SendString("go up\r\n");
        DelayMs(10);
        target_y = current_pos_y - left_right_distance;
        target_x = current_pos_x;
        UpdateReferencePoint(point_num, target_x, target_y, speed);
        up_flag = 0;
        P41 = 0;
    }
    else if (down_flag)
    {
        P41 = 1;
        DelayMs(3);
        Uart1_SendString("go down\r\n");
        DelayMs(10);
        target_y = current_pos_y + left_right_distance;
        target_x = current_pos_x;
        UpdateReferencePoint(point_num, target_x, target_y, speed);
        down_flag = 0;
        P41 = 0;
    }

    // 如果没有任何标志被触发，让出时间给接收
    if (!sign1_flag && !sign2_flag && !sign3_flag &&
        !left_flag && !right_flag && !up_flag && !down_flag)
    {
        P41 = 0;
        DelayMs(1);
    }

    return OK;
}
