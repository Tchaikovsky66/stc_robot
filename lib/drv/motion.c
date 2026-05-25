#include "motion.h"
#include "motor.h"
#include "gpio.h"
#include "dwin.h"
#include "delay.h"
#include "params.h"
#include "uart.h"

/*=== 当前坐标（归位后为 0） ===*/
__data int 当前x坐标 = 0;
__data int 当前y1坐标 = 0;
__data int 当前y2坐标 = 0;

/*=== 坐标运动：先X后Y1后Y2 ===*/
char MoveToPosition(int target_x, int target_y1, int target_y2, int speed)
{
    int dist;
    char dir;

    /* X 轴 */
    dist = target_x - 当前x坐标;
    if (dist != 0)
    {
        if (dist > 0) { dir = GO_LEFT; }
        else          { dir = GO_RIGHT; dist = -dist; }
        DelayMs(10);
        if (motor_go(X_MOTOR, dir, dist, speed) == ERR)
            return ERR;
        当前x坐标 = target_x;
        DelayMs(10);
    }

    /* Y1 轴 */
    dist = target_y1 - 当前y1坐标;
    if (dist != 0)
    {
        if (dist > 0) { dir = GO_DOWN; }
        else          { dir = GO_UP; dist = -dist; }
        DelayMs(10);
        if (motor_go(Y1_MOTOR, dir, dist, speed) == ERR)
            return ERR;
        当前y1坐标 = target_y1;
        DelayMs(10);
    }

    /* Y2 轴 */
    dist = target_y2 - 当前y2坐标;
    if (dist != 0)
    {
        if (dist > 0) { dir = GO_DOWN; }
        else          { dir = GO_UP; dist = -dist; }
        DelayMs(10);
        if (motor_go(Y2_MOTOR, dir, dist, speed) == ERR)
            return ERR;
        当前y2坐标 = target_y2;
        DelayMs(10);
    }

    给迪文上传数据(ADDR_DWIN_CUR_X, 当前x坐标);
    给迪文上传数据(ADDR_DWIN_CUR_Y, 当前y1坐标);
    return OK;
}

/*=== 运行前复位：移到右限位再到等待位 ===*/
void 运行前复位(void)
{
    MoveToPosition(未加工到右限位距离, 0, 0, 左右速度);
    当前x坐标 = 0;
    等待处位置 = 未加工到加工距离 - 3200;
    H170 = E80 + F20 + 夹取位置;
    MoveToPosition(等待处位置, 0, 0, 左右速度);
}

/*=== 夹爪控制 ===*/
void 夹取(char side)
{
    if (side == SIDE_LEFT)
        LEFT_GRIPPER = 1;
    else if (side == SIDE_RIGHT)
        RIGHT_GRIPPER = 1;
    DelayMs(500);
}

void 放工件(char side)
{
    if (side == SIDE_LEFT)
        LEFT_GRIPPER = 0;
    else if (side == SIDE_RIGHT)
        RIGHT_GRIPPER = 0;
    DelayMs(500);
}

/*=== 加工设备通信 ===*/
void 通知加工设备复位(void)
{
    MACHINE_RESET = 0;
    DelayMs(20);
    MACHINE_RESET = 1;
    DelayMs(20);
}

void 通知加工设备运行(void)
{
    MACHINE_RUN = 0;
    DelayMs(20);
    MACHINE_RUN = 1;
    DelayMs(20);
}

/*=== 等待信号 ===*/
void 等待加工完成(void)
{
    while (1)
    {
        if (!MACHINE_HOME)
            break;
        if (MACHINE_HAS_PART && MACHINE_HOME && !MACHINE_ERROR)
        {
            DelayMs(20);
            if (MACHINE_HAS_PART && MACHINE_HOME && !MACHINE_ERROR)
                break;
        }
        DelayMs(10);
    }
}

void 等待工件取走(void)
{
    GoToPage(40);
    给迪文上传数据(ADDR_DWIN_PULSE, 0);
    while (1)
    {
        if (PRODUCT_PRESENT)
        {
            DelayMs(20);
            break;
        }
        if (!MACHINE_HAS_PART && !MACHINE_HOME && !MACHINE_ERROR)
        {
            DelayMs(20);
            if (!MACHINE_HAS_PART && !MACHINE_HOME && !MACHINE_ERROR)
                break;
        }
        DelayMs(10);
    }
}
