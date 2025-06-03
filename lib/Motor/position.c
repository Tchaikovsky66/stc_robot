#include <motor.h>
#include <uart.h>
#include <delay.h>
#include "../../include/main.h"
#include "dwin.h"
#include "common.h"
__data int 当前y2坐标;
__data int 当前x坐标;
__data int 当前y1坐标;

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
    static char x_direction;
    static int y_distance;
    static char y_direction;
    static int y2_distance;
    static char y2_direction;

    // 计算X轴移动距离和方向
    x_distance = target_x - 当前x坐标;
    y_distance = target_y - 当前y1坐标;
    y2_distance = target_y2 - 当前y2坐标;

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
        if (motor_go(X_MOTOR, x_direction, x_distance, speed) == ERR)
        {
            return ERR;
        }
        当前x坐标 = target_x;
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
        if (motor_go(Y1_MOTOR, y_direction, y_distance, speed) == ERR)
        {
            return ERR;
        }
        当前y1坐标 = target_y;
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
        if (motor_go(Y2_MOTOR, y2_direction, y2_distance, speed) == ERR)
        {
            return ERR;
        }
        当前y2坐标 = target_y2;
        DelayMs(10);
    }
    //DelayMs(10);
   给迪文上传数据(Addr当前X位置, 当前x坐标);
   给迪文上传数据(Addr当前Y位置, 当前y1坐标);
    // UploadData(0x21 * 2, 当前x坐标);
    // UploadData(0x22 * 2, 当前y1坐标);
    // UploadData(0x23 * 2, 当前y2坐标);
    return OK;
}

void 调试(void)
{
    GoToPage(15);
    while (1)
    {
        if (接收到数据)
        {
            从迪文更新数据();
            DelayMs(20);
            接收到数据 = 0;
        }
        DelayMs(20);
        if (开始调试)
        {
            开始调试 = 0;
            给迪文上传数据(Addr迪文14标志, 0xffff);
            从迪文更新数据();
            DelayMs(20);
            GoToPage(16);
            DelayMs(20);

            MoveToPosition(300, 500, 0, 200); // 到达取物料点,准备归零
            DelayMs(20);

            break;
        }
    }
    // 调试抓取点
    while (1)
    {
        if (接收到数据)
        {
            DelayMs(20);
            从接收缓冲区更新数据();
            接收到数据 = 0;

            MoveToPosition(当前X位置, 当前Y位置, 0, 200);
        }
        if (开始调试) // 前往第二个调试点
        {
            开始调试 = 0;

            未加工到右限位距离 = 当前x坐标;
            两端上下距离 = 当前Y位置;
            给迪文上传数据(Addr未加工到右限位距离, 未加工到右限位距离);
            给迪文上传数据(Addr两端上下距离, 两端上下距离);
            给迪文上传数据(Addr迪文14标志, 0xffff);
            从迪文更新数据();
            当前x坐标 = 0;
            // 当前X位置 = 0;
            // 给迪文上传数据(Addr当前X位置, 0);
            MoveToPosition(0, 0, 0, 200);
            MoveToPosition(5000, 4000, 0, 200);
            GoToPage(17);
            break;
        }
    }
    while (1) // 调试加工位置
    {
        if (接收到数据)
        {
            DelayMs(20);
            从接收缓冲区更新数据();
            接收到数据 = 0;

            MoveToPosition(当前X位置, 当前Y位置, 0, 200);
        }
        if (开始调试) // 前往第三个调试点
        {
            开始调试 = 0;
            未加工到加工距离 = 当前X位置;
            加工位置上下距离 = 当前Y位置;
            给迪文上传数据(Addr迪文14标志, 0xffff);
            给迪文上传数据(Addr未加工到加工距离, 未加工到加工距离);
            给迪文上传数据(Addr加工位置上下距离, 加工位置上下距离);
            从迪文更新数据();
            MoveToPosition(当前X位置, 0, 0, 200);
            MoveToPosition(7600, 0, 两端上下距离, 200);
            GoToPage(18);
            break;
        }
    }
    while (1)
    {
        if (接收到数据)
        {
            DelayMs(20);
            从接收缓冲区更新数据();
            接收到数据 = 0;

            MoveToPosition(当前X位置, 0, 当前Y位置, 200);
        }
        if (开始调试)
        {
            开始调试 = 0;
            未加工到结束距离 = 当前X位置;
            给迪文上传数据(Addr未加工到结束距离, 未加工到结束距离);
            MoveToPosition(当前X位置, 0, 0, 200);
            MoveToPosition(0, 0, 0, 200);
            GoToPage(2);
            break;
        }
    }
}
void 夹取(void)
{
    DelayMs(500);
}
void 放工件(void)
{
    DelayMs(500);
}
void 运行(void)
{
    MoveToPosition(未加工到右限位距离, 0, 0, 左右速度);
    当前x坐标 = 0;
    等待处位置 = 未加工到加工距离 - 3200;
    H170 = E80 + F20 + 夹取位置;
    while (1)
    {
        // 等待来料
        GoToPage(20);
        /*等待来料*/
        DelayMs(2000);
        MoveToPosition(等待处位置, 0, 0, 左右速度);
        // 向右快进E80
        GoToPage(21);
        MoveToPosition(等待处位置 - E80, 0, 0, 左右速度);
        // 向右监测F20
        GoToPage(22); // 检测结束

        MoveToPosition(等待处位置 - E80 - F20, 0, 0, 200);
        // 向右快进G70
        GoToPage(23); // 工件夹取中
        给迪文上传数据(Addr当前脉冲数, 0);

        DelayMs(100);

        GoToPage(24); // 工件夹取中
        MoveToPosition(等待处位置 - H170, 0, 0, 左右速度);

        GoToPage(25);
        MoveToPosition(等待处位置 - H170, 两端上下距离, 0, 左右速度);
        夹取();
        GoToPage(26);
        给迪文上传数据(Addr当前脉冲数, 0);

        DelayMs(100);

        GoToPage(27);
        MoveToPosition(等待处位置 - H170, 0, 0, 左右速度);
        GoToPage(28);
        给迪文上传数据(Addr当前脉冲数, 0);

        DelayMs(100);

        GoToPage(29);
        MoveToPosition(等待处位置, 0, 0, 左右速度);
        GoToPage(30);
        给迪文上传数据(Addr当前脉冲数, 0);

        // 等待加工完成
        DelayMs(2000);
        GoToPage(31); // 加工位置工件夹取
        MoveToPosition(等待处位置, 0, 加工位置上下距离, 左右速度);
        GoToPage(32);
        给迪文上传数据(Addr当前脉冲数, 0);

        夹取();
        GoToPage(33);
        MoveToPosition(等待处位置, 0, 0, 左右速度);
        GoToPage(34);
        给迪文上传数据(Addr当前脉冲数, 0);

        DelayMs(100);

        GoToPage(35); // y1去 放 加工位置
        MoveToPosition(未加工到加工距离, 0, 0, 左右速度);
        GoToPage(36);
        给迪文上传数据(Addr当前脉冲数, 0);

        DelayMs(100);

        GoToPage(37);
        MoveToPosition(未加工到加工距离, 加工位置上下距离, 0, 左右速度);
        GoToPage(38);
        给迪文上传数据(Addr当前脉冲数, 0);

        放工件();
        GoToPage(39);
        MoveToPosition(未加工到加工距离, 0, 0, 左右速度);
        /*等待工件取走*/
        GoToPage(40);
        给迪文上传数据(Addr当前脉冲数, 0);

        DelayMs(2000);
        GoToPage(41);
        MoveToPosition(未加工到结束距离, 0, 0, 左右速度);
        GoToPage(42);
        给迪文上传数据(Addr当前脉冲数, 0);

        DelayMs(100);

        // 放加工工件
        GoToPage(43);
        MoveToPosition(未加工到结束距离, 0, 两端上下距离, 左右速度);
        GoToPage(44);
        给迪文上传数据(Addr当前脉冲数, 0);

        放工件();
        GoToPage(45);
        MoveToPosition(未加工到结束距离, 0, 0, 左右速度);
        GoToPage(46);
        给迪文上传数据(Addr当前脉冲数, 0);

        DelayMs(100);

        GoToPage(47); // 回到零位 !!!!!!!!!!!表达有问题
        MoveToPosition(等待处位置, 0, 0, 左右速度);
        GoToPage(48);
        给迪文上传数据(Addr当前脉冲数, 0);
    }
}