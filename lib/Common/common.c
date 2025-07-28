#include "common.h"

__bit 测试运行 = 0;
__bit 保存工件名称 = 0;
__bit 复位并运行 = 0;
__bit 参数调试 = 0;
__bit 开始调试 = 0;
__bit 保存 = 0;
__bit 恢复出厂设置标志位 = 0;
__bit 实时测量夹取位置 = 0;

int 实时测量夹取位置_脉冲数 = 0;
__xdata int 开机初始化bar;
int 每10ms计数 = 0;
int 当前脉冲数 = 0;
__xdata int 两端上下距离 = 50 * 10;
__xdata int 加工位置上下距离 = 400 * 10;
__xdata int 未加工到加工距离 = 500 * 10;
__xdata int 未加工到结束距离 = 760 * 10;
__xdata int 左右速度 = 600;
__xdata int 上下速度 = 600;
__xdata int 加减速系数 = 20;
__xdata int 夹取位置 = 70 * 10; // G70
__xdata int 铁芯直径 = 40 * 10;
int 当前X位置 = 0;
int 当前Y位置 = 0;
int 计算速度 = 500;
__xdata int 等待处位置 = 180 * 10;
__xdata int 未加工到右限位距离 = 30 * 10;
__xdata int F20 = 200;
__xdata int E80 = 800;
__xdata int H170 = 1700;
volatile __xdata int 工件序号 = 0x100;

__xdata char 工件名称[] = {0xB9, 0xA4, 0xBC, 0xFE, 0x31, 0x31, 0x00, 0x00};
__xdata char 型号1[] = {0xB9, 0xA4, 0xBC, 0xFE, 0x31, 0x00, 0x00, 0x00};
__xdata char 型号2[] = {0xB9, 0xA4, 0xBC, 0xFE, 0x32, 0x00};
__xdata char 型号3[] = {0xB9, 0xA4, 0xBC, 0xFE, 0x33, 0x00};
__xdata char 型号4[] = {0xB9, 0xA4, 0xBC, 0xFE, 0x34, 0x00};
__xdata char 型号5[] = {0xB9, 0xA4, 0xBC, 0xFE, 0x35, 0x00};
__xdata char 型号6[] = {0xB9, 0xA4, 0xBC, 0xFE, 0x36, 0x00};
__xdata char 型号7[] = {0xB9, 0xA4, 0xBC, 0xFE, 0x37, 0x00};
__xdata char 型号8[] = {0xB9, 0xA4, 0xBC, 0xFE, 0x38, 0x00};
