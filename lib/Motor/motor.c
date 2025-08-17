#include "motor.h"
#include "../../include/stc15.h"
#include "../../include/main.h"
#include "uart.h"
#include "dwin.h"
#include "common.h"
#include "delay.h"
// 在源文件中定义变量 - SDCC 方式
__sbit __at(0xB3) Y1工件检测; // y1工件检测 P33

__data int 中断次数 = 0;
__data int 当前总步数 = 0;
__data int 当前y1轴最大脉冲数 = 0;
__data int 当前y2轴最大脉冲数 = 0;

__data unsigned int x轴脉冲总值 = 0;
__data unsigned int y1轴脉冲总值 = 0;
__data unsigned int y2轴脉冲总值 = 0;

__data volatile unsigned int 当前y1轴脉冲数 = 0;
__data volatile unsigned int 当前y2轴脉冲数 = 0;

__bit 电机总开关 = 0;
__bit 限位开关标志位 = 0;
__bit 计数10ms发送标志位 = 0;
__data char 电机标志位 = 0;
__bit 正在归位标志位 = 0;
__bit 电机运行结束标志位 = 0;

// 常量数组放在代码区
__code const unsigned int delay_table1[101] = {
    46080, 41714, 35483, 29932, 25613, 22324, 19791, 17226, 15293, 13788,
    12583, 11596, 10773, 10074, 9474, 8952, 8493, 8087, 7724, 7366,
    7022, 6723, 6459, 6223, 6012, 5821, 5646, 5487, 5340, 5205,
    5079, 4962, 4853, 4750, 4654, 4564, 4478, 4397, 4320, 4248,
    4178, 4112, 4050, 3989, 3932, 3877, 3824, 3773, 3724, 3677,
    3632, 3589, 3546, 3506, 3467, 3429, 3392, 3356, 3322, 3288,
    3256, 3224, 3194, 3164, 3135, 3107, 3072, 3040, 3010, 2981,
    2953, 2926, 2901, 2876, 2853, 2830, 2809, 2788, 2768, 2749,
    2730, 2712, 2695, 2679, 2663, 2647, 2632, 2618, 2604, 2591,
    2578, 2566, 2554, 2542, 2531, 2520, 2510, 2500, 2490, 2481, 2481};
__code const unsigned int 速度表[101] = {
    5,
    6,
    8,
    10,
    13,
    17,
    19,
    23,
    25,
    28,
    30,
    33,
    35,
    37,
    39,
    41,
    43,
    44,
    46,
    48,
    49,
    51,
    52,
    54,
    55,
    57,
    58,
    59,
    61,
    62,
    63,
    64,
    65,
    67,
    68,
    69,
    70,
    71,
    72,
    73,
    74,
    75,
    76,
    77,
    78,
    79,
    80,
    80,
    81,
    82,
    83,
    84,
    84,
    85,
    86,
    87,
    87,
    88,
    88,
    89,
    90,
    90,
    91,
    91,
    92,
    92,
    93,
    93,
    93,
    94,
    94,
    95,
    95,
    95,
    96,
    96,
    96,
    97,
    97,
    97,
    98,
    98,
    98,
    98,
    98,
    99,
    99,
    99,
    99,
    99,
    99,
    99,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
};

void motor_init(void)
{
    /***使用定时器0****/
    P0M0 = 0xff; // 设置 P0 为推挽输出
    P0M1 = 0x00;
    P1M0 &= ~0xf8;
    P1M1 &= ~0xf8; // P13-P17 准双向口

    P3M0 |= 0x80;
    P3M1 &= ~0x80;

    AUXR |= 0x80; // 定时器时钟1T模式
    TMOD &= 0xF0; // 设置定时器模式为模式0
    TL0 = 0;      // 清零定时器初值
    TH0 = 0;
    TF0 = 0; // 清除TF0标志
    ET0 = 1; // 使能定时器0中断
    EA = 1;  // 开启总中断

    X_STEP = 0;
    Y1_STEP = 0;
    Y2_STEP = 0;
    P37 = 1;
}

void Timer1_10ms_Init(void) // 10毫秒@11.0592MHz
{
    AUXR &= 0xBF; // 定时器时钟12T模式
    TMOD &= 0x0F; // 设置定时器模式
    TL1 = 0x00;   // 设置定时初始值
    TH1 = 0xDC;   // 设置定时初始值
    TF1 = 0;      // 清除TF1标志
    // TR1 = 1;      // 定时器1开始计时
    ET1 = 1; // 使能定时器1中断
}

void timer1_isr(void) __interrupt(3)
{
    // TF1 = 0; // 清除TF1标志

    if (电机总开关)
    {
        每10ms计数++;
        // Uart1_SendByte(每10ms计数);
        // Uart1_SendString("time1");
        计数10ms发送标志位 = 1;
    }
    // else
    // {
    //     TR1 = 0;
    // }
}
void timer0_isr(void) __interrupt(1)
{
    // 判断是否到达限位
    if ((X_R_LIMIT == 0 | X_L_LIMIT == 0 | Y1_LIMIT == 0 | Y2_LIMIT == 0) && 正在归位标志位)
    {
        TR0 = 0; // 停止定时器

        // 电机总开关 = 0;
        限位开关标志位 = 1;
    }
    
    if (电机总开关)
    {
        
        if (当前脉冲数 >= 当前总步数)
        {
            TR0 = 0; // 停止定时器
            电机总开关 = 0;
            电机标志位 = 0;
            电机运行结束标志位 = 1;

            X_STEP = 0;
            Y1_STEP = 0;
            Y2_STEP = 0;
        }
        中断次数++;
        switch (电机标志位)
        {
        case 1:
            /* code */
            X_EN = 0;         // x轴脉冲使能
            X_STEP = !X_STEP; // x轴脉冲输出
            if (中断次数 == 4)
            {
                中断次数 = 0;
                当前脉冲数++;
                /*检测工件长度*/
                if (实时测量夹取位置)
                {
                    if (Y1工件检测 == 0)
                    {
                        if(复位并运行)
                        {
                            电机运行结束标志位 = 1;

                        }
                        if (X_DIR == 0)
                        {
                            实时测量夹取位置_脉冲数++;
                        }
                        else
                        {
                            实时测量夹取位置_脉冲数--;
                        }
                    }
                }
                /*检测结束*/
                if (X_DIR == 0)
                {
                    x轴脉冲总值--;
                }
                else
                {
                    x轴脉冲总值++;
                }
            }
            break;
        case 2:
            Y1_STEP = !Y1_STEP; // y1轴脉冲输出
            if (中断次数 == 4)
            {
                中断次数 = 0;
                当前脉冲数++;
                if (Y1_DIR == 0)
                {
                    y1轴脉冲总值--;
                }
                else
                {
                    y1轴脉冲总值++;
                }
            }
            break;
        case 3:
            Y2_STEP = !Y2_STEP; // y1轴脉冲输出
            if (中断次数 == 4)
            {
                中断次数 = 0;
                当前脉冲数++;
                if (Y2_DIR == 0)
                {
                    y2轴脉冲总值--;
                }
                else
                {
                    y2轴脉冲总值++;
                }
            }
            break;

        default:
            break;
        }
    }
}

char motor_go(char num, char 方向, int 距离, int 速度)
{
    中断次数 = 0;
    电机运行结束标志位 = 0;
    电机标志位 = num;
    X_STEP = 0;
    Y1_STEP = 0;
    Y2_STEP = 0;
    X_DIR = 方向;
    Y1_DIR = 方向;
    Y2_DIR = 方向;
    unsigned int reload;
    int 速度系数 = 速度 / 10;                  // 50    25
    int 加速系数 = 加减速系数;                 // 15    15
    int 加速总步数 = 加速系数 * 速度系数;      // 15*50 = 750   25*15 = 375
    int 总放大系数 = 加速系数 * 速度系数 / 10; // 75        37.5
    加速总步数 = (unsigned int)总放大系数 * 10;
    int 匀速阶段步数 = 0;
    当前总步数 = 距离 * 1;
    匀速阶段步数 = 当前总步数 - 2 * 加速总步数;
    if (匀速阶段步数 <= 0)
    {
        加速总步数 = 当前总步数 * 10 / 加速系数;    /// 50mm*10 *10 / 20 = 250
        速度系数 = 加速总步数 / 加速系数;           // 250/20 = 12  12.5
        总放大系数 = 加速系数 * 速度系数 / 10;      // 20*12/10 = 24    25
        匀速阶段步数 = 当前总步数 - 2 * 加速总步数; // 0
        计算速度 = 速度系数 * 10;
    }
    当前脉冲数 = 0;
    电机总开关 = 1;
    int pulse_num_tmp = 0;
    每10ms计数 = 0;
    unsigned long temp = 2763200UL / ((unsigned long)速度表[100] * 速度系数);
    temp = 2763200UL / ((unsigned int)速度表[100] * 速度系数);
    unsigned int max_reload = (temp > 65535UL) ? 60000U : (65535U - (unsigned int)temp);
    给迪文上传数据(Addr当前脉冲数, 当前脉冲数);
    给迪文上传数据(Addr每10ms计数, 每10ms计数);
    while (1)
    {
        if ((当前脉冲数 % 50 == 0) && (当前脉冲数 != 0))
        {
            计数10ms发送标志位 = 0;
            // if (num = 1)
            // {
            //     给迪文上传数据(Addr当前脉冲数, x轴脉冲总值);
            // }
            // if (num = 2)
            // {
            //     给迪文上传数据(Addr当前脉冲数, y1轴脉冲总值);
            // }
            // if (num = 3)
            // {
            //     给迪文上传数据(Addr当前脉冲数, y2轴脉冲总值);
            // }
            给迪文上传数据(Addr当前脉冲数, 当前脉冲数);
            // 给迪文上传数据(Addr每10ms计数, 每10ms计数);
            //  给迪文上传数据(Addr当前X位置, 当前X位置);
            //  给迪文上传数据(Addr当前Y位置, 当前Y位置);
        }

        if (电机运行结束标志位)
        {
            给迪文上传数据(Addr当前脉冲数, 当前脉冲数);
            给迪文上传数据(Addr每10ms计数, 每10ms计数);
            break;
        }
        if (限位开关标志位)
        {
            X_STEP = 0;
            Y1_STEP = 0;
            Y2_STEP = 0;
            TR0 = 0; // 定时器0计时
            TR1 = 0; // 定时器1计时
            TL0 = 0; // 清零定时器初值
            TH0 = 0;
            TF0 = 0; // 清除TF0标志
            return ERR;
        }
        if (匀速阶段步数 <= 0)
        {
            if (当前脉冲数 < 0.5 * 当前总步数) // 加速阶段
            {
                pulse_num_tmp = 当前脉冲数 * 10 / 总放大系数;
                if (pulse_num_tmp > 100)
                    pulse_num_tmp = 100; // 防止数组越界
                temp = 2763200UL / ((unsigned long)速度表[pulse_num_tmp] * 速度系数);
                // temp = delay_table1[pulse_num_tmp];
                reload = (temp > 65535UL) ? 2481U : (65535U - (unsigned int)temp);
                TL0 = reload; // 设置新的定时值
                TH0 = reload >> 8;
            }
            else // 减速阶段
            {
                pulse_num_tmp = (当前总步数 - 当前脉冲数) * 10 / 总放大系数;
                if (pulse_num_tmp > 100)
                    pulse_num_tmp = 100; // 防止数组越界
                temp = 2763200UL / ((unsigned long)速度表[pulse_num_tmp] * 速度系数);
                reload = (temp > 65535UL) ? 2481U : (65535U - (unsigned int)temp);
                TL0 = reload;
                TH0 = reload >> 8;
            }
        }
        else
        {
            if (当前脉冲数 < 加速总步数) // 加速阶段
            {
                pulse_num_tmp = 当前脉冲数 * 10 / 总放大系数;
                temp = 2763200UL / ((unsigned long)速度表[pulse_num_tmp] * 速度系数);
                // temp = delay_table1[pulse_num_tmp];
                reload = (temp > 65535UL) ? 2481U : (65535U - (unsigned int)temp);
                TL0 = reload; // 设置新的定时值
                TH0 = reload >> 8;
            }
            else if (当前脉冲数 <= 匀速阶段步数) // 匀速阶段
            // else if ((当前脉冲数 <= 匀速阶段步数) && (匀速阶段步数 > 0)) // 匀速阶段

            {
                TL0 = max_reload;
                TH0 = max_reload >> 8;
            }
            else // 减速阶段
            {
                pulse_num_tmp = (当前总步数 - 当前脉冲数) * 10 / 总放大系数;
                if (pulse_num_tmp >= 100)
                    pulse_num_tmp = 99; // 防止数组越界
                temp = 2763200UL / ((unsigned long)速度表[pulse_num_tmp] * 速度系数);
                reload = (temp > 65535UL) ? 2481U : (65535U - (unsigned int)temp);
                TL0 = reload;
                TH0 = reload >> 8;
            }
            // else
            // {
            //     pulse_num_tmp = 当前脉冲数 / 总放大系数;
            //     reload = 65535 - (unsigned int)(2763200 / ((速度表[101 - (当前脉冲数 - 当前总步数 + 加速总步数) / 总放大系数]) * 速度系数));
            //     TL0 = reload;
            //     TH0 = reload >> 8;
            // }
        }
        TR0 = 1; // 定时器0计时
        TR1 = 1; // 定时器1计时
    }

    TR0 = 0; // 定时器0计时
    TR1 = 0; // 定时器1计时
    TL0 = 0; // 清零定时器初值
    TH0 = 0;
    TF0 = 0; // 清除TF0标志

    return FINISH;
}

void 电机复位(void)
{
    GoToPage(58); // 开机初始化
    DelayMs(20);
    给迪文上传数据(Addr开机初始化bar, 0);
    正在归位标志位 = 1;
    if (motor_go(Y1_MOTOR, GO_UP, 5000, 200) == ERR)
    {
        给迪文上传数据(Addr开机初始化bar, 1);

        正在归位标志位 = 0;
        y1轴脉冲总值 = 0;
        限位开关标志位 = 0;
        DelayMs(100);
        给迪文上传数据(Addr开机初始化bar, 2);

        motor_go(Y1_MOTOR, GO_DOWN, 100, 100);
        y1轴脉冲总值 = 0;
        给迪文上传数据(Addr开机初始化bar, 3);
    }
    正在归位标志位 = 1;
    DelayMs(10);

    if (motor_go(Y2_MOTOR, GO_UP, 5000, 200) == ERR)
    {
        给迪文上传数据(Addr开机初始化bar, 4);

        正在归位标志位 = 0;
        y2轴脉冲总值 = 0;
        限位开关标志位 = 0;
        DelayMs(100);
        给迪文上传数据(Addr开机初始化bar, 5);

        motor_go(Y2_MOTOR, GO_DOWN, 100, 100);
        y2轴脉冲总值 = 0;
        给迪文上传数据(Addr开机初始化bar, 6);
    }
    正在归位标志位 = 1;
    DelayMs(10);
    if (motor_go(X_MOTOR, GO_RIGHT, 8000, 100) == ERR)
    {
        给迪文上传数据(Addr开机初始化bar, 7);

        正在归位标志位 = 0;
        x轴脉冲总值 = 0;
        限位开关标志位 = 0;
        DelayMs(100);
        给迪文上传数据(Addr开机初始化bar, 8);

        motor_go(X_MOTOR, GO_LEFT, 100, 100);
        x轴脉冲总值 = 0;
        给迪文上传数据(Addr开机初始化bar, 9);
    }
    x轴脉冲总值 = 0;
    y1轴脉冲总值 = 0;
    y2轴脉冲总值 = 0;
}