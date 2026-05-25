#include "dwin.h"
#include "uart.h"
#include "gpio.h"
#include "delay.h"
#include "params.h"

static __xdata char tmp[8];

/*=== 基础工具函数 ===*/

void Blick(void)
{
    EN_485 = 0; DelayMs(500);
    EN_485 = 1; DelayMs(500);
    EN_485 = 0; DelayMs(500);
    EN_485 = 1; DelayMs(500);
}

void InitValue(unsigned char address, unsigned int data)
{
    CFGBUF[address]     = (data >> 8) & 0xFF;
    CFGBUF[address + 1] = data & 0xFF;
}

/*=== 页面切换 ===*/

void GoToPage(unsigned char page)
{
    DelayMs(20);
    EN_485 = 1;
    Uart1_SendByte(0x5A);
    Uart1_SendByte(0xA5);
    Uart1_SendByte(0x04);
    Uart1_SendByte(0x80);
    Uart1_SendByte(0x03);
    Uart1_SendByte(0x00);
    Uart1_SendByte(page);
    EN_485 = 0;
    DelayMs(20);
}

/*=== 写变量到迪文屏 ===*/

void WriteData(unsigned char address, unsigned char data1, unsigned char data2)
{
    EN_485 = 1;
    Uart1_SendByte(0x5A);
    Uart1_SendByte(0xA5);
    Uart1_SendByte(0x05);
    Uart1_SendByte(0x82);
    Uart1_SendByte(0x00);
    Uart1_SendByte(address);
    Uart1_SendByte(data1);
    Uart1_SendByte(data2);
    EN_485 = 0;
}

void 给迪文上传数据(unsigned char address, unsigned int data)
{
    EN_485 = 1;
    InitValue(address, data);
    Uart1_SendByte(0x5A);
    Uart1_SendByte(0xA5);
    Uart1_SendByte(0x05);
    Uart1_SendByte(0x82);
    Uart1_SendByte(0x00);
    Uart1_SendByte(address / 2);
    Uart1_SendByte(CFGBUF[address]);
    Uart1_SendByte(CFGBUF[address + 1]);
    EN_485 = 0;
}

/*=== 图标管理 ===*/

void 更新图标L(__bit add, char icon)
{
    if (add)
        图标标志位L |= icon;
    else
        图标标志位L &= ~icon;
    给迪文上传数据(ADDR_DWIN_ICON, ((unsigned int)图标标志位H << 8) | 图标标志位L);
}

void 更新图标H(__bit add, char icon)
{
    if (add)
        图标标志位H |= icon;
    else
        图标标志位H &= ~icon;
    给迪文上传数据(ADDR_DWIN_ICON, ((unsigned int)图标标志位H << 8) | 图标标志位L);
}

/*=== 批量读取迪文屏数据 ===*/

void 获取迪文所有数据(void)
{
    static const unsigned char cmd[] = {0x5A, 0xA5, 0x04, 0x83, 0x00, 0x00, 0x30};
    Uart1_SendBuffer(cmd, sizeof(cmd));
}

void 获取所有工件名称(void)
{
    static const unsigned char cmd[] = {0x5A, 0xA5, 0x04, 0x83, 0x01, 0x00, 0x44};
    Uart1_SendBuffer(cmd, sizeof(cmd));
}

/*=== 从 CFGBUF 解析参数到全局变量 ===*/

/* 从 CFGBUF 读取16位大端值 */
static int cfg_read16(unsigned char addr)
{
    return (CFGBUF[addr] << 8) | CFGBUF[addr + 1];
}

void 从接收缓冲区更新数据(void)
{
    unsigned char flags = ~CFGBUF[ADDR_DWIN_FLAG14];
    测试运行           = flags & 0x01;
    保存工件名称       = flags & 0x02;
    开始调试           = flags & 0x04;
    模拟来料           = flags & 0x08;
    复位并运行         = flags & 0x10;
    参数调试           = flags & 0x20;
    恢复出厂设置标志位 = flags & 0x40;
    保存               = flags & 0x80;
    自动端面检测       = (~cfg_read16(ADDR_DWIN_AUTO_FACE)) & 0x01;

    工件序号           = (CFGBUF[ADDR_DWIN_MODEL_NO] << 8) | 0;
    两端上下距离       = cfg_read16(ADDR_DWIN_UD_DIST);
    加工位置上下距离   = cfg_read16(ADDR_DWIN_PROC_DIST);
    未加工到加工距离   = cfg_read16(ADDR_DWIN_RAW2PROC);
    未加工到结束距离   = cfg_read16(ADDR_DWIN_RAW2END);
    未加工到右限位距离 = cfg_read16(ADDR_DWIN_RAW_LIMIT);
    左右速度           = cfg_read16(ADDR_DWIN_LR_SPEED);
    上下速度           = cfg_read16(ADDR_DWIN_UD_SPEED);
    加减速系数         = cfg_read16(ADDR_DWIN_ACCEL);
    夹取位置           = cfg_read16(ADDR_DWIN_CATCH_POS);
    铁芯直径           = cfg_read16(ADDR_DWIN_CORE_DIA);
    当前X位置          = cfg_read16(ADDR_DWIN_CUR_X);
    当前Y位置          = cfg_read16(ADDR_DWIN_CUR_Y);
    计算速度           = cfg_read16(ADDR_DWIN_CALC_SPEED);
    F20                = cfg_read16(ADDR_DWIN_F20);
    E80                = cfg_read16(ADDR_DWIN_E80);
}

void 从迪文更新数据(void)
{
    获取迪文所有数据();
    DelayMs(40);
    从接收缓冲区更新数据();
    DelayMs(20);
    获取所有工件名称();
    DelayMs(20);
}

/*=== 将 CFGBUF 全部发送给迪文屏 ===*/

void 从缓存区发送数据给迪文(void)
{
    InitValue(ADDR_DWIN_E80, E80);
    InitValue(ADDR_DWIN_F20, F20);

    Uart1_SendByte(0x5A);
    Uart1_SendByte(0xA5);
    Uart1_SendByte(sizeof(CFGBUF) + 3);
    Uart1_SendByte(0x82);
    Uart1_SendByte(0x00);
    Uart1_SendByte(0x00);
    Uart1_SendBuffer(CFGBUF, sizeof(CFGBUF));
}

/*=== 恢复出厂设置 ===*/

void 恢复出厂设置(void)
{
    static const char init_hdr[] = {0x5A, 0xA5, 0x65, 0x82, 0x00, 0x00};

    CFGBUF[0x00] = 0xCC;
    InitValue(ADDR_DWIN_FLAG14,    0xFFFF);
    InitValue(ADDR_DWIN_MODEL_NO,  DEFAULT_MODEL_NO);
    InitValue(ADDR_DWIN_LR_SPEED,  500);
    InitValue(ADDR_DWIN_UD_SPEED,  500);
    InitValue(ADDR_DWIN_ACCEL,     DEFAULT_ACCEL);
    InitValue(ADDR_DWIN_UD_DIST,   DEFAULT_UD_DIST);
    InitValue(ADDR_DWIN_PROC_DIST, DEFAULT_PROC_DIST);
    InitValue(ADDR_DWIN_RAW2PROC,  DEFAULT_RAW2PROC);
    InitValue(ADDR_DWIN_RAW2END,   DEFAULT_RAW2END);
    InitValue(ADDR_DWIN_RAW_LIMIT, DEFAULT_RAW_LIMIT);
    InitValue(ADDR_DWIN_DET_SPEED, DEFAULT_F20);
    InitValue(ADDR_DWIN_CATCH_POS, DEFAULT_CATCH_POS);
    InitValue(ADDR_DWIN_CORE_DIA,  DEFAULT_CORE_DIA);
    InitValue(ADDR_DWIN_CUR_X,     0);
    InitValue(ADDR_DWIN_CUR_Y,     0);
    InitValue(ADDR_DWIN_E80,       DEFAULT_E80);
    InitValue(ADDR_DWIN_F20,       DEFAULT_F20);

    Uart1_SendBuffer(init_hdr, 6);
    Uart1_SendBuffer(CFGBUF, sizeof(CFGBUF));
}

/*=== 型号相关 ===*/

void 从缓存更新型号(void)
{
    unsigned char i;
    for (i = 0; i < 8; i++)
    {
        型号1[i] = 型号数组[ADDR_MODEL_1 + i];
        型号2[i] = 型号数组[ADDR_MODEL_2 + i];
        型号3[i] = 型号数组[ADDR_MODEL_3 + i];
        型号4[i] = 型号数组[ADDR_MODEL_4 + i];
        型号5[i] = 型号数组[ADDR_MODEL_5 + i];
        型号6[i] = 型号数组[ADDR_MODEL_6 + i];
        型号7[i] = 型号数组[ADDR_MODEL_7 + i];
        型号8[i] = 型号数组[ADDR_MODEL_8 + i];
    }
}

void 将型号发送给迪文(void)
{
    unsigned char i;
    unsigned int j;
    static const char hdr[] = {0x5A, 0xA5, 0x40, 0x82, 0x01, 0x00};

    for (i = 0; i < 8; i++)
    {
        型号数组[ADDR_MODEL_NAME + i] = 工件名称[i];
        型号数组[ADDR_MODEL_1 + i] = 型号1[i];
        型号数组[ADDR_MODEL_2 + i] = 型号2[i];
        型号数组[ADDR_MODEL_3 + i] = 型号3[i];
        型号数组[ADDR_MODEL_4 + i] = 型号4[i];
        型号数组[ADDR_MODEL_5 + i] = 型号5[i];
        型号数组[ADDR_MODEL_6 + i] = 型号6[i];
        型号数组[ADDR_MODEL_7 + i] = 型号7[i];
        型号数组[ADDR_MODEL_8 + i] = 型号8[i];
    }
    Uart1_SendBuffer(hdr, 6);
    for (j = 0; j < sizeof(型号数组); j++)
    {
        Uart1_SendByte(型号数组[j]);
    }
}

void 发送型号(unsigned int addr, unsigned char buf_offset, unsigned char len)
{
    unsigned char addrH = (addr >> 8) & 0xFF;
    unsigned char addrL = addr & 0xFF;
    unsigned char i;
    EN_485 = 1;
    Uart1_SendByte(0x5A);
    Uart1_SendByte(0xA5);
    Uart1_SendByte(len + 3);
    Uart1_SendByte(0x82);
    Uart1_SendByte(addrH);
    Uart1_SendByte(addrL);
    for (i = buf_offset; i < buf_offset + len; i++)
    {
        Uart1_SendByte(型号数组[i] == 0xFF ? 0x00 : 型号数组[i]);
    }
    EN_485 = 0;
    DelayMs(10);
}

void 发送中文(unsigned int address, const char *text, unsigned char len)
{
    unsigned char addrH = (address >> 8) & 0xFF;
    unsigned char addrL = address & 0xFF;
    unsigned char i;
    EN_485 = 1;
    Uart1_SendByte(0x5A);
    Uart1_SendByte(0xA5);
    Uart1_SendByte(len + 3);
    Uart1_SendByte(0x82);
    Uart1_SendByte(addrH);
    Uart1_SendByte(addrL);
    for (i = 0; i < len; i++)
    {
        Uart1_SendByte(text[i]);
    }
    EN_485 = 0;
    DelayMs(10);
}

void 发送序号(unsigned int seq)
{
    static const unsigned char err_text[] = {0xB4, 0xED, 0xCE, 0xF3, 0x45, 0x52, 0x52, 0x00};
    unsigned char offset;

    switch (seq)
    {
    case 0x100: offset = ADDR_MODEL_1; break;
    case 0x200: offset = ADDR_MODEL_2; break;
    case 0x300: offset = ADDR_MODEL_3; break;
    case 0x400: offset = ADDR_MODEL_4; break;
    case 0x500: offset = ADDR_MODEL_5; break;
    case 0x600: offset = ADDR_MODEL_6; break;
    case 0x700: offset = ADDR_MODEL_7; break;
    case 0x800: offset = ADDR_MODEL_8; break;
    default:
        发送中文(0x100, err_text, 8);
        return;
    }
    发送型号(0x100, offset, 8);
}
