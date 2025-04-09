__sbit __at(0xC1) EN_485; // 使能485发送

#define Addr当前脉冲数 0x0B * 2
#define Addr每10ms计数 0x0C * 2
#define Addr计算速度 0x0D * 2
#define Addr迪文14标志 0x14 * 2
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

#define Addr当前X位置 0x24 * 2
#define Addr当前Y位置 0x25 * 2

#define Addr等待处位置 0x28 * 2
// dwin
extern __bit 测试运行;
extern __bit 复位并运行;
extern __bit 参数调试;
extern __bit 保存;
extern __bit 开始调试;

extern __xdata int 开机初始化bar;
extern int 当前脉冲数;
extern int 两端上下距离;
extern int 加工位置上下距离;
extern int 未加工到加工距离;
extern int 未加工到结束距离;
extern int 未加工到右限位距离;
extern int 左右速度;
extern int 上下速度;
extern int 加减速系数;
extern int 夹取位置;
extern int 铁芯直径;
extern int A180;
extern __xdata int F20;
extern int 当前X位置;
extern int 当前Y位置;
extern int 每10ms计数;
extern int 计算速度;
extern int 等待处位置;
// uart
extern __bit 接收到数据;