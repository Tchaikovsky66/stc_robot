/*
 * motor.c — 电机驱动 + Timer0/Timer1 ISR
 */
#include "stc15.h"
#include "config.h"
#include "gpio.h"
#include "motor.h"
#include "dwin.h"
#include "params.h"
#include "delay.h"
#include "uart.h"

/*=== 变量定义 ===*/
volatile char motor_busy = 0;
volatile char limit_hit = 0;
volatile int pulse_count = 0;
char homing_mode = 0;

__data int _motor_target = 0;
__data char _motor_axis = 0;
__data int _motor_isr_cnt = 0;

__data unsigned int x轴脉冲总值 = 0;
__data unsigned int y1轴脉冲总值 = 0;
__data unsigned int y2轴脉冲总值 = 0;

int 每10ms计数 = 0;
int 计算速度 = DEFAULT_CALC_SPEED;
int 实时测量夹取位置_脉冲数 = 0;

/*=== S曲线速度表 ===*/
__code const unsigned int 速度表[101] = {
    5,  6,  8,  10, 13, 17, 19, 23, 25, 28,
    30, 33, 35, 37, 39, 41, 43, 44, 46, 48,
    49, 51, 52, 54, 55, 57, 58, 59, 61, 62,
    63, 64, 65, 67, 68, 69, 70, 71, 72, 73,
    74, 75, 76, 77, 78, 79, 80, 80, 81, 82,
    83, 84, 84, 85, 86, 87, 87, 88, 88, 89,
    90, 90, 91, 91, 92, 92, 93, 93, 93, 94,
    94, 95, 95, 95, 96, 96, 96, 97, 97, 97,
    98, 98, 98, 98, 98, 99, 99, 99, 99, 99,
    99, 99, 100, 100, 100, 100, 100, 100, 100, 100,
    100
};

/*=== 初始化 ===*/
void motor_init(void)
{
    P0M0 = 0xFF;
    P0M1 = 0x00;
    P1M0 &= ~0xF8;
    P1M1 &= ~0xF8;
    P3M0 |= 0x80;
    P3M1 &= ~0x80;

    AUXR |= 0x80;  /* Timer0 1T */
    TMOD &= 0xF0;
    TL0 = 0; TH0 = 0;
    TF0 = 0; TR0 = 0;
    ET0 = 1;
    EA = 1;

    X_STEP = 0; Y1_STEP = 0; Y2_STEP = 0;
    P37 = 1;
}

void Timer1_10ms_Init(void)
{
    AUXR &= 0xBF;
    TMOD &= 0x0F;
    TL1 = 0x00;
    TH1 = 0xDC;
    TF1 = 0; TR1 = 0;
    ET1 = 1;
}

/*=== motor_go: S曲线加减速 ===*/
char motor_go(char axis, char dir, int distance, int speed)
{
    unsigned int reload;
    unsigned long temp;
    int 速度系数, 加速总步数, 总放大系数, 匀速阶段步数;
    int idx;
    unsigned int max_reload;

    if (distance <= 0) return FINISH;

    X_DIR = dir; Y1_DIR = dir; Y2_DIR = dir;
    X_STEP = 0; Y1_STEP = 0; Y2_STEP = 0;

    速度系数 = speed / 10;
    if (速度系数 < 1) 速度系数 = 1;
    总放大系数 = 加减速系数 * 速度系数 / 10;
    if (总放大系数 < 1) 总放大系数 = 1;
    加速总步数 = (unsigned int)总放大系数 * 10;
    匀速阶段步数 = distance - 2 * 加速总步数;

    if (匀速阶段步数 <= 0)
    {
        加速总步数 = distance * 10 / 加减速系数;
        速度系数 = 加速总步数 / 加减速系数;
        if (速度系数 < 1) 速度系数 = 1;
        总放大系数 = 加减速系数 * 速度系数 / 10;
        if (总放大系数 < 1) 总放大系数 = 1;
        匀速阶段步数 = distance - 2 * 加速总步数;
        计算速度 = 速度系数 * 10;
    }

    temp = 2763200UL / ((unsigned long)速度表[100] * 速度系数);
    max_reload = (temp > 65535UL) ? 60000U : (65535U - (unsigned int)temp);

    _motor_isr_cnt = 0;
    _motor_axis = axis;
    _motor_target = distance;
    pulse_count = 0;
    limit_hit = 0;
    motor_busy = 1;

    /* 初始 reload: 最低速 */
    temp = 2763200UL / ((unsigned long)速度表[0] * 速度系数);
    reload = (temp > 65535UL) ? 2481U : (65535U - (unsigned int)temp);
    TL0 = reload;
    TH0 = reload >> 8;
    TF0 = 0;
    TR0 = 1;
    TR1 = 1;

    while (motor_busy)
    {
        if (limit_hit)
        {
            X_STEP = 0; Y1_STEP = 0; Y2_STEP = 0;
            TR0 = 0; TR1 = 0;
            motor_busy = 0;
            return ERR;
        }

        /* S曲线速度更新 */
        if (匀速阶段步数 <= 0)
        {
            if (pulse_count < (distance >> 1))
                idx = pulse_count * 10 / 总放大系数;
            else
                idx = (distance - pulse_count) * 10 / 总放大系数;
            if (idx > 100) idx = 100;
            if (idx < 0) idx = 0;
            temp = 2763200UL / ((unsigned long)速度表[idx] * 速度系数);
            reload = (temp > 65535UL) ? 2481U : (65535U - (unsigned int)temp);
        }
        else
        {
            if (pulse_count < 加速总步数)
            {
                idx = pulse_count * 10 / 总放大系数;
                if (idx > 100) idx = 100;
                temp = 2763200UL / ((unsigned long)速度表[idx] * 速度系数);
                reload = (temp > 65535UL) ? 2481U : (65535U - (unsigned int)temp);
            }
            else if (pulse_count <= 匀速阶段步数)
            {
                reload = max_reload;
            }
            else
            {
                idx = (distance - pulse_count) * 10 / 总放大系数;
                if (idx >= 100) idx = 99;
                if (idx < 0) idx = 0;
                temp = 2763200UL / ((unsigned long)速度表[idx] * 速度系数);
                reload = (temp > 65535UL) ? 2481U : (65535U - (unsigned int)temp);
            }
        }
        TL0 = reload;
        TH0 = reload >> 8;
    }

    TR0 = 0; TR1 = 0;
    return FINISH;
}

/*=== 三轴归位：先 X 向右找右限位→回退100；再 Y1 向上→回退100；再 Y2 向上→回退100 ===*/
void 电机复位(void)
{
    GoToPage(58);
    DelayMs(20);
    给迪文上传数据(ADDR_DWIN_INIT_BAR, 0);

    /* 1. X 轴向右找限位 */
    homing_mode = 1;
    limit_hit = 0;
    motor_go(X_MOTOR, GO_RIGHT, 8000, 100);
    homing_mode = 0;
    limit_hit = 0;
    给迪文上传数据(ADDR_DWIN_INIT_BAR, 3);
    DelayMs(100);
    motor_go(X_MOTOR, GO_LEFT, 100, 100);
    x轴脉冲总值 = 0;

    /* 2. Y1 轴向上找限位 */
    homing_mode = 1;
    limit_hit = 0;
    motor_go(Y1_MOTOR, GO_UP, 5000, 200);
    homing_mode = 0;
    limit_hit = 0;
    给迪文上传数据(ADDR_DWIN_INIT_BAR, 6);
    DelayMs(100);
    motor_go(Y1_MOTOR, GO_DOWN, 100, 100);
    y1轴脉冲总值 = 0;

    /* 3. Y2 轴向上找限位 */
    homing_mode = 1;
    limit_hit = 0;
    motor_go(Y2_MOTOR, GO_UP, 5000, 200);
    homing_mode = 0;
    limit_hit = 0;
    给迪文上传数据(ADDR_DWIN_INIT_BAR, 9);
    DelayMs(100);
    motor_go(Y2_MOTOR, GO_DOWN, 100, 100);
    y2轴脉冲总值 = 0;
}

/*=== Timer0 ISR: 步进脉冲生成 + 归位限位检测 ===*/
void timer0_isr(void) __interrupt(1)
{
    if (homing_mode)
    {
        char hit = 0;
        switch (_motor_axis)
        {
        case X_MOTOR:
            if (X_DIR == GO_RIGHT) { if (X_R_LIMIT == 0) hit = 1; }
            else                    { if (X_L_LIMIT == 0) hit = 1; }
            break;
        case Y1_MOTOR:
            if (Y1_DIR == GO_UP && Y1_LIMIT == 0) hit = 1;
            break;
        case Y2_MOTOR:
            if (Y2_DIR == GO_UP && Y2_LIMIT == 0) hit = 1;
            break;
        }
        if (hit)
        {
            TR0 = 0;
            limit_hit = 1;
            motor_busy = 0;
            return;
        }
    }

    if (!motor_busy)
        return;

    _motor_isr_cnt++;

    switch (_motor_axis)
    {
    case X_MOTOR:
        X_EN = MOTOR_EN_ON;
        X_STEP = !X_STEP;
        if (_motor_isr_cnt >= 4)
        {
            _motor_isr_cnt = 0;
            pulse_count++;
            if (X_DIR == 0) x轴脉冲总值--;
            else            x轴脉冲总值++;
        }
        break;

    case Y1_MOTOR:
        Y1_STEP = !Y1_STEP;
        if (_motor_isr_cnt >= 4)
        {
            _motor_isr_cnt = 0;
            pulse_count++;
            if (Y1_DIR == 0) y1轴脉冲总值--;
            else             y1轴脉冲总值++;
        }
        break;

    case Y2_MOTOR:
        Y2_STEP = !Y2_STEP;
        if (_motor_isr_cnt >= 4)
        {
            _motor_isr_cnt = 0;
            pulse_count++;
            if (Y2_DIR == 0) y2轴脉冲总值--;
            else             y2轴脉冲总值++;
        }
        break;
    }

    if (pulse_count >= _motor_target)
    {
        TR0 = 0;
        motor_busy = 0;
        X_STEP = 0;
        Y1_STEP = 0;
        Y2_STEP = 0;
    }
}

/*=== Timer1 ISR: 10ms 计数 ===*/
void timer1_isr(void) __interrupt(3)
{
    if (motor_busy)
        每10ms计数++;
}
