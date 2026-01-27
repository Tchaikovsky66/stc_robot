__sbit __at(0xC1) EN_485; // 使能485发送
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

#define Addr图标标志位 0x11*2
#define 图标L_X1 0b00000001
#define 图标L_X2 0b00000010
#define 图标L_Y1 0b00000100
#define 图标L_Y2 0b00001000
#define 图标L_Y3 0b00010000
#define 图标H_右工件 0b00000001
#define 图标H_左工件 0b00000010
#define 图标H_加工工件 0b00000100
#define 图标H_右状态 0b00001000
#define 图标H_左状态 0b00010000



#define Addr当前脉冲数 0x0B * 2
#define Addr每10ms计数 0x0C * 2
#define Addr计算速度 0x0D * 2
#define Addr迪文14标志 0x14 * 2
#define Addr自动端面检测 0x15 * 2
#define Addr未加工到右限位距离 0x16 * 2
#define Addr开机初始化bar 0x17 * 2
#define Addr两端上下距离 0x18 * 2
#define Addr加工位置上下距离 0x19 * 2
#define Addr未加工到加工距离 0x1A * 2
#define Addr未加工到结束距离 0x1B * 2
#define Addr左右速度 0x1C * 2
#define Addr上下速度 0x1D * 2
#define Addr加减速系数 0x1E * 2
#define Addr检测速度 0x1F * 2

#define Addr夹取位置 0x20 * 2
#define Addr铁芯直径 0x21 * 2
#define Addr工件序号 0x22 * 2
#define Addr当前X位置 0x24 * 2
#define Addr当前Y位置 0x25 * 2
#define Addr等待处位置 0x28 * 2

#define AddrE80 0x2C * 2
#define AddrH170 0x2E *2
#define AddrF20 0x2F * 2

#define Addr工件名称 0x00
#define Addr型号1 0x08 * 2
#define Addr型号2 0x10 * 2
#define Addr型号3 0x18 * 2
#define Addr型号4 0x20 * 2
#define Addr型号5 0x28 * 2
#define Addr型号6 0x30 * 2
#define Addr型号7 0x38 * 2
#define Addr型号8 0x40 * 2
// dwin
extern __bit 自动端面检测;
extern __bit 测试运行;
extern __bit 保存工件名称;
extern __bit 复位并运行;
extern __bit 参数调试;
extern __bit 保存;
extern __bit 开始调试;
extern __bit 恢复出厂设置标志位;
extern __bit 实时测量夹取位置;

extern int 图标标志位;
extern char 图标标志位H;
extern char 图标标志位L;

extern int 实时测量夹取位置_脉冲数;
extern __xdata int 开机初始化bar;
volatile extern __xdata int 工件序号;
extern int 当前脉冲数;
extern __xdata int 两端上下距离;
extern __xdata int 加工位置上下距离;
extern __xdata int 未加工到加工距离;
extern __xdata int 未加工到结束距离;
extern __xdata int 未加工到右限位距离;
extern __xdata int 左右速度;
extern __xdata int 上下速度;
extern __xdata int 加减速系数;
extern __xdata int 夹取位置;
extern __xdata int 铁芯直径;
extern __xdata int F20;
extern __xdata int E80;
extern __xdata int H170;
extern int 当前X位置;
extern int 当前Y位置;
extern int 每10ms计数;
extern int 计算速度;
extern __xdata int 等待处位置;      //A180
// uart
extern __bit 接收到数据;
extern __xdata char 工件名称[8];
extern __xdata char 型号1[8];
extern __xdata char 型号2[8];
extern __xdata char 型号3[8];
extern __xdata char 型号4[8];
extern __xdata char 型号5[8];
extern __xdata char 型号6[8];
extern __xdata char 型号7[8];
extern __xdata char 型号8[8];