#include <motor.h>
#include <uart.h>
#include <delay.h>
#include "../../include/main.h"
#include "dwin.h"
#include "common.h"
#include "../../include/stc15.h"

#define 右 1
#define 左 0

__data int 当前y2坐标;
__data int 当前x坐标;
__data int 当前y1坐标;

    //X中间传感器 BIT P1.5 
    //放V工件有无 BIT P1.6 //	左边存放V型块上工件是否取走
    //取V工件有无 BIT P1.7
	// 加工设备有无工件	BIT	P2.0	                                 //加工设备Y3
	// 加工设备异常	BIT	P2.1	                                     //加工设备Y2
	// 加工设备原位置	BIT	P2.2	                                   //加工设备Y1
	// 加工设备复位	BIT	P2.3	                                     //加工设备X1
	// 加工设备运行	BIT	P2.4	                                     //加工设备X2
    //    Y1右工件检测 BIT P3.3
    //    Y1右零位 BIT P3.4
    //    Y2左零位 BIT P3.5
    //    Y2左工件检测 BIT P3.6
    //    X左防撞 BIT P3.7
    //    Y刹车 BIT P3.7
    //Y2轴左吸爪 BIT 0C2H     // P4.2
    //Y1轴右吸爪 BIT 0C4H // P4.4

__sbit __at(0x95) X中间传感器;
__sbit __at(0x96) 成品位置有工件;
__sbit __at(0x97) 物料位置有工件;

__sbit __at(0xB3) Y1工件检测;     // y1工件检测 P33
__sbit __at(0xB4) Y1零位;  // 0到达限位 ，1未到达 P34
__sbit __at(0xB5) Y2零位;  // 0到达限位 ，1未到达 P35
__sbit __at(0xB6) Y2工件检测;     // y2工件检测 P36

__sbit __at(0xA0) 加工设备有无工件; // P20 Y3
__sbit __at(0xA1) 加工设备异常;     // P21 Y2
__sbit __at(0xA2) 加工设备原位置;   // P22  Y1
__sbit __at(0xA3) 加工设备复位;     // P23  // X1
__sbit __at(0xA4) 加工设备运行;     // P24 // X2

__sbit __at(0xC2) 左吸爪;
__sbit __at(0xC4) 右吸爪;

void 通知加工设备复位(void)
{
    加工设备复位 = 0; // 复位
    DelayMs(200);
    加工设备复位 = 1; // 复位
    DelayMs(20);
}
void 通知加工设备运行(void)
{

    加工设备运行 = 0; // 运行
    DelayMs(200);
    加工设备运行 = 1; // 运行
    DelayMs(20);
}
void 夹取(char OP)
{
    if(OP == 左)
    {
        //左吸爪 = 0;
        P42 = 1;
    }
    else if(OP == 右)
    {
        P44 = 1;
    }
    DelayMs(500);
}
void 放工件(char OP)
{
    if (OP == 左)
    {
        P42 = 0;
    }
    else if (OP == 右)
    {
        P44 = 0;
    }
    DelayMs(500);
}
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
    // DelayMs(10);
    给迪文上传数据(Addr当前X位置, 当前x坐标);
    给迪文上传数据(Addr当前Y位置, 当前y1坐标);
    // UploadData(0x21 * 2, 当前x坐标);
    // UploadData(0x22 * 2, 当前y1坐标);
    // UploadData(0x23 * 2, 当前y2坐标);
    return OK;
}

void 运行前复位(void)
{
    MoveToPosition(未加工到右限位距离-100, 0, 0, 左右速度);//临时更改-100！！！
    当前x坐标 = 0;
    等待处位置 = 未加工到加工距离 - 3200;
    H170 = E80 + F20 + 夹取位置;
    MoveToPosition(等待处位置, 0, 0, 左右速度);
}

char 等待来料(void)
{
    GoToPage(20);

    while (1)
    {
        if (!Y1工件检测)
        {
            DelayMs(20);
            break;
        }
        //模拟下一步
        if(加工设备有无工件 && 加工设备原位置 && 加工设备异常)
        {
            DelayMs(20);
            if(!加工设备有无工件 && !加工设备原位置 && !加工设备异常)
            {   
                break;
            }
        }

        DelayMs(10);
    }
    return 1;
}
void 移动到物料位置(void)
{
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
    夹取(右);
    GoToPage(26);
    给迪文上传数据(Addr当前脉冲数, 0);

    DelayMs(100);

    GoToPage(27);
    MoveToPosition(等待处位置 - H170, 0, 0, 左右速度);
    GoToPage(28);
    给迪文上传数据(Addr当前脉冲数, 0);

    DelayMs(100);
}
void 从物料位置移动到等待位置(void)
{
    GoToPage(29);
    MoveToPosition(等待处位置, 0, 0, 左右速度);
    GoToPage(30);
    给迪文上传数据(Addr当前脉冲数, 0);
}

char 等待加工完成(void)
{
    while (1)
    {
        //有无工件为1，加工设备原位置为0，加工设备异常为0
        if (加工设备有无工件 && !加工设备原位置 && !加工设备异常)
        {
            DelayMs(20);
            //防抖
            if(加工设备有无工件 && !加工设备原位置 && !加工设备异常)
            {
             break;
            }
        }
        //增加如果等待时间过长,则返回错误
        DelayMs(10);
    }
    return 1;
}
void 去加工设备夹取已加工物料(void)
{
    GoToPage(31); // 加工位置工件夹取
    MoveToPosition(等待处位置, 0, 加工位置上下距离, 左右速度);
    GoToPage(32);
    给迪文上传数据(Addr当前脉冲数, 0);

    夹取(左);
    GoToPage(33);
    MoveToPosition(等待处位置, 0, 0, 左右速度);
    GoToPage(34);
    给迪文上传数据(Addr当前脉冲数, 0);

    DelayMs(100);
}
void 去加工设备放待加工工件(void)
{
    GoToPage(35); // y1去 放 加工位置
    MoveToPosition(未加工到加工距离, 0, 0, 左右速度);
    GoToPage(36);
    给迪文上传数据(Addr当前脉冲数, 0);

    DelayMs(100);

    GoToPage(37);
    MoveToPosition(未加工到加工距离, 加工位置上下距离, 0, 左右速度);
    GoToPage(38);
    给迪文上传数据(Addr当前脉冲数, 0);

    放工件(右);
    GoToPage(39);
    MoveToPosition(未加工到加工距离, 0, 0, 左右速度);
}
char 等待工件取走(void)
{
    GoToPage(40);
    给迪文上传数据(Addr当前脉冲数, 0);
    while (1)
    {
        if (!成品位置有工件)
        {
            DelayMs(20);

            break;
        }
        //模拟下一步
        if (加工设备有无工件 && 加工设备原位置 && 加工设备异常)
        {
            DelayMs(20);
            if (!加工设备有无工件 && !加工设备原位置 && !加工设备异常)
            {
                break;
            }
        }
        DelayMs(10);
    }
    return 1;
}
void 去成品位置放加工好工件(void)
{
    GoToPage(43);
    MoveToPosition(未加工到结束距离, 0, 两端上下距离, 左右速度);
    GoToPage(44);
    给迪文上传数据(Addr当前脉冲数, 0);

    放工件(左);
    GoToPage(45);
    MoveToPosition(未加工到结束距离, 0, 0, 左右速度);
    GoToPage(46);
    给迪文上传数据(Addr当前脉冲数, 0);

    DelayMs(100);
}
void 回到等待位置(void)
{
    GoToPage(47); // 回到零位 !!!!!!!!!!!表达有问题
    MoveToPosition(等待处位置, 0, 0, 左右速度);
    GoToPage(48);
    给迪文上传数据(Addr当前脉冲数, 0);
}

void 调试_测量工件(void)
{
    
    //测量夹取位置
    MoveToPosition(等待处位置, 0, 0, 左右速度);
    实时测量夹取位置 = 1;
    MoveToPosition(300, 0, 0, 200);
    夹取位置 = 实时测量夹取位置_脉冲数 / 20; // 计算夹取位置
    实时测量夹取位置_脉冲数 = 0; // 清除脉冲数
    给迪文上传数据(Addr夹取位置, 夹取位置);
    实时测量夹取位置_脉冲数 = 0; // 清除脉冲数
    实时测量夹取位置 = 0;
}

void 调试_抓取点(void)
{
    GoToPage(16);
    夹取(右); // 夹取右吸爪
    MoveToPosition(300, 500, 0, 200);
}

void 调试_加工位置(void)
{
    MoveToPosition(当前X位置, 0, 0, 200);
    MoveToPosition(5000, 4000, 0, 200);
    GoToPage(17);
}

void 调试_结束位置(void)
{
    MoveToPosition(当前X位置, 0, 0, 200);
    MoveToPosition(7600, 0, 两端上下距离, 200);
    GoToPage(18);
}

/******************* 调试 ********************/
void 调试(void)
{
    放工件(右); // 先放开右吸爪
    放工件(左); // 先放开左吸爪
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
            /*测试*/
            //调试_测量工件();
            //测量夹取位置
            DelayMs(2000);    
            /*测试结束*/
            //测量夹取位置
            DelayMs(20);
            MoveToPosition(300, 900-铁芯直径-50, 0, 200); // 到达取物料点,准备归零
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
            当前x坐标 = 0;      //warning：疑似这里出问题 ***未加工到右限位距离是0点
            // 当前X位置 = 0;
            // 给迪文上传数据(Addr当前X位置, 0);
            铁芯直径 = 900 - 两端上下距离;
            给迪文上传数据(Addr铁芯直径,铁芯直径);
            /*测量工件位置*/
            MoveToPosition(0, 两端上下距离-20, 0, 左右速度);
            MoveToPosition(夹取位置+500, 两端上下距离-20, 0, 左右速度);
            实时测量夹取位置 = 1;
            MoveToPosition(0,两端上下距离-20,0,150);
            夹取位置 = 实时测量夹取位置_脉冲数-300; // 计算夹取位置
            实时测量夹取位置_脉冲数 = 0; // 清除脉冲数
            给迪文上传数据(Addr夹取位置, 夹取位置);
            实时测量夹取位置_脉冲数 = 0; // 清除脉冲数
            实时测量夹取位置 = 0;
            MoveToPosition(0,两端上下距离,0,150);

            /*测量结束*/
            夹取(右); // 夹取右吸爪
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
            MoveToPosition(0, 两端上下距离, 0, 200); // 到达取物料点,准备归零
            放工件(右); // 放开右吸爪
            MoveToPosition(0, 0, 0, 200);

            GoToPage(2);
            break;
        }
    }
}

void 运行1(void)
{
    运行前复位();
    通知加工设备复位();
    放工件(右); // 先放开右吸爪
    放工件(左); // 先放开左吸爪
    while (1)
    {
        //阻塞点
        等待来料();
        移动到物料位置();
        从物料位置移动到等待位置();
        //阻塞点
        if(加工设备有无工件 && !加工设备原位置 && !加工设备异常)
        {
            等待加工完成();
            去加工设备夹取已加工物料();
        }
        去加工设备放待加工工件();
        通知加工设备运行();
        //阻塞点
        等待工件取走();
        去成品位置放加工好工件();
        回到等待位置();
    }
}
void 运行(void)
{
    MoveToPosition(未加工到右限位距离, 0, 0, 左右速度);
    当前x坐标 = 0;
    等待处位置 = 未加工到加工距离 - 3200;
    H170 = E80 + F20 + 夹取位置;
    __bit i = 0;

    while (1)
    {
        if (i == 0)
        {
            给迪文上传数据(Addr图标标志位, 0x0000);
            i = 1;
        }
        else
        {
            给迪文上传数据(Addr图标标志位, 0xffff);
            i = 0;
        }
        GoToPage(20);
        /*等待来料*/
        //DelayMs(2000);
        等待来料();
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
        //夹取();
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

        等待加工完成();
        // 等待加工完成
        // DelayMs(2000);
        GoToPage(31); // 加工位置工件夹取
        MoveToPosition(等待处位置, 0, 加工位置上下距离, 左右速度);
        GoToPage(32);
        给迪文上传数据(Addr当前脉冲数, 0);

        //夹取();
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

        //放工件();
        GoToPage(39);
        MoveToPosition(未加工到加工距离, 0, 0, 左右速度);
        /*等待工件取走*/
       
        GoToPage(40);
        给迪文上传数据(Addr当前脉冲数, 0);
        等待工件取走();
        //DelayMs(2000);
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

        //放工件();
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