#include "params.h"

/*=== 迪文屏标志位 ===*/
__bit 测试运行 = 0;
__bit 保存工件名称 = 0;
__bit 开始调试 = 0;
__bit 复位并运行 = 0;
__bit 参数调试 = 0;
__bit 恢复出厂设置标志位 = 0;
__bit 保存 = 0;
__bit 自动端面检测 = 1;
__bit 实时测量夹取位置 = 0;
__bit 模拟来料 = 0;

/*=== 运动参数（默认值与迪文屏同步） ===*/
__xdata int 两端上下距离 = DEFAULT_UD_DIST;
__xdata int 加工位置上下距离 = DEFAULT_PROC_DIST;
__xdata int 未加工到加工距离 = DEFAULT_RAW2PROC;
__xdata int 未加工到结束距离 = DEFAULT_RAW2END;
__xdata int 未加工到右限位距离 = DEFAULT_RAW_LIMIT;
__xdata int 左右速度 = DEFAULT_LR_SPEED;
__xdata int 上下速度 = DEFAULT_UD_SPEED;
__xdata int 加减速系数 = DEFAULT_ACCEL;
__xdata int 夹取位置 = DEFAULT_CATCH_POS;
__xdata int 铁芯直径 = DEFAULT_CORE_DIA;
__xdata int 等待处位置 = DEFAULT_WAIT_POS;
__xdata int F20 = DEFAULT_F20;
__xdata int E80 = DEFAULT_E80;
__xdata int H170 = DEFAULT_H170;
volatile __xdata int 工件序号 = DEFAULT_MODEL_NO;

/*=== 状态/显示变量 ===*/
int 当前X位置 = 0;
int 当前Y位置 = 0;
__xdata int 开机初始化bar = 0;

/*=== 图标 ===*/
int 图标标志位 = 0;
char 图标标志位H = 0;
char 图标标志位L = 0;

/*=== 工件名称 ===*/
__xdata char 工件名称[] = {0xB9, 0xA4, 0xBC, 0xFE, 0x31, 0x31, 0x00, 0x00};
__xdata char 型号1[] = {0xB9, 0xA4, 0xBC, 0xFE, 0x31, 0x00, 0x00, 0x00};
__xdata char 型号2[] = {0xB9, 0xA4, 0xBC, 0xFE, 0x32, 0x00};
__xdata char 型号3[] = {0xB9, 0xA4, 0xBC, 0xFE, 0x33, 0x00};
__xdata char 型号4[] = {0xB9, 0xA4, 0xBC, 0xFE, 0x34, 0x00};
__xdata char 型号5[] = {0xB9, 0xA4, 0xBC, 0xFE, 0x35, 0x00};
__xdata char 型号6[] = {0xB9, 0xA4, 0xBC, 0xFE, 0x36, 0x00};
__xdata char 型号7[] = {0xB9, 0xA4, 0xBC, 0xFE, 0x37, 0x00};
__xdata char 型号8[] = {0xB9, 0xA4, 0xBC, 0xFE, 0x38, 0x00};
