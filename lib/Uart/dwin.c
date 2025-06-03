#include <uart.h>
#include <dwin.h>
#include <common.h>
#include "delay.h"
#include "eeprom.h"
static const unsigned char 错误[8] = {0xB4, 0xED, 0xCE, 0xF3, 0x45, 0x52, 0x52};
__xdata char tmp[8];
void Blick(void)
{
    EN_485 = 0;
    DelayMs(500);
    EN_485 = 1;
    DelayMs(500);
    EN_485 = 0;
    DelayMs(500);
    EN_485 = 1;
    DelayMs(500);
}
void InitValue(unsigned char address, unsigned int data)
{
    unsigned char low_byte = data & 0xFF;
    unsigned char high_byte = (data >> 8) & 0xFF;
    CFGBUF[address] = high_byte;
    CFGBUF[address + 1] = low_byte;
}

/**
 * @brief 获取显示屏所有数据
 * @note 发送获取数据命令到显示屏
 */
void 获取迪文所有数据(void)
{
    static const unsigned char get_all_adata[] = {0x5A, 0xA5, 0x04, 0x83, 0x00, 0x00, 0x30};
    Uart1_SendBuffer(get_all_adata, sizeof(get_all_adata));
    // Uart1_SendBuffer(get_all_data,7)
}
void 获取所有工件名称(void)
{
    static const unsigned char get_all_adata[] = {0x5A, 0xA5, 0x04, 0x83, 0x01, 0x00, 0x44};
    Uart1_SendBuffer(get_all_adata, sizeof(get_all_adata));
}
/**
 * @brief 更新并上传单个参数到显示屏
 * @param address 参数地址
 * @param data 参数值
 * @note 使用串口发送数据到显示屏
 */

void 给迪文上传数据(unsigned char address, unsigned int data)
{
    EN_485 = 1;
    InitValue(address, data);
    // 固定发送8字节数据
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
void 从缓存更新型号(void)
{
    // 从CFGBUF更新型号数组
    for (unsigned char i = 0; i < 8; i++)
    {
        型号1[i] = 型号数组[Addr型号1 + i];
        型号2[i] = 型号数组[Addr型号2 + i];
        型号3[i] = 型号数组[Addr型号3 + i];
        型号4[i] = 型号数组[Addr型号4 + i];
        型号5[i] = 型号数组[Addr型号5 + i];
        型号6[i] = 型号数组[Addr型号6 + i];
        型号7[i] = 型号数组[Addr型号7 + i];
        型号8[i] = 型号数组[Addr型号8 + i];
    }
}
void 将型号发送给迪文(void)
{
    static const char init_data[] = {
        0x5A, 0xA5, 0x40, 0x82, 0x01, 0x00};
    for (unsigned char i = 0; i < 8; i++)
    {
        型号数组[Addr工件名称 + i] = 工件名称[i];
        型号数组[Addr型号1 + i] = 型号1[i];
        型号数组[Addr型号2 + i] = 型号2[i];
        型号数组[Addr型号3 + i] = 型号3[i];
        型号数组[Addr型号4 + i] = 型号4[i];
        型号数组[Addr型号5 + i] = 型号5[i];
        型号数组[Addr型号6 + i] = 型号6[i];
        型号数组[Addr型号7 + i] = 型号7[i];
        型号数组[Addr型号8 + i] = 型号8[i];
    }
    Uart1_SendBuffer(init_data, 6);
    for (unsigned int j = 0; j < sizeof(型号数组); j++)
    {
        Uart1_SendByte(型号数组[j]);
    }
}

void 从接收缓冲区更新数据(void)
{
    测试运行 = (~CFGBUF[0x14 * 2]) & 0b00000001;     // 14 8
    保存工件名称 = (~CFGBUF[0x14 * 2]) & 0b00000010; // 14 9
    开始调试 = (~CFGBUF[0x14 * 2]) & 0x04;           // 14A
    复位并运行 = (~CFGBUF[0x14 * 2]) & 0x10;         // 14C
    参数调试 = (~CFGBUF[0x14 * 2]) & 0x20;           // 14D
    恢复出厂设置标志位 = (~CFGBUF[0x14 * 2]) & 0x40; // 14E
    保存 = (~CFGBUF[0x14 * 2]) & 0b10000000;         // 14F
    工件序号 = (CFGBUF[Addr工件序号] << 8) | 0;      // 22h
    两端上下距离 = (CFGBUF[Addr两端上下距离] << 8) | CFGBUF[Addr两端上下距离 + 1];
    加工位置上下距离 = (CFGBUF[Addr加工位置上下距离] << 8) | CFGBUF[Addr加工位置上下距离 + 1];
    未加工到加工距离 = (CFGBUF[Addr未加工到加工距离] << 8) | CFGBUF[Addr未加工到加工距离 + 1];
    未加工到结束距离 = (CFGBUF[Addr未加工到结束距离] << 8) | CFGBUF[Addr未加工到结束距离 + 1];
    未加工到右限位距离 = (CFGBUF[Addr未加工到右限位距离] << 8) | CFGBUF[Addr未加工到右限位距离 + 1];
    左右速度 = (CFGBUF[Addr左右速度] << 8) | CFGBUF[Addr左右速度 + 1];
    上下速度 = (CFGBUF[Addr上下速度] << 8) | CFGBUF[Addr上下速度 + 1];
    加减速系数 = (CFGBUF[Addr加减速系数] << 8) | CFGBUF[Addr加减速系数 + 1];
    夹取位置 = (CFGBUF[Addr夹取位置] << 8) | CFGBUF[Addr夹取位置 + 1];
    铁芯直径 = (CFGBUF[Addr铁芯直径] << 8) | CFGBUF[Addr铁芯直径 + 1];
    当前X位置 = (CFGBUF[Addr当前X位置] << 8) | CFGBUF[Addr当前X位置 + 1];
    当前Y位置 = (CFGBUF[Addr当前Y位置] << 8) | CFGBUF[Addr当前Y位置 + 1];
    计算速度 = (CFGBUF[Addr计算速度] << 8) | CFGBUF[Addr计算速度 + 1];
    F20 = (CFGBUF[AddrF20] << 8) | CFGBUF[AddrF20 + 1];
    E80 = (CFGBUF[AddrE80] << 8) | CFGBUF[AddrE80 + 1];
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
void 恢复出厂设置(void)
{

    __xdata static const char init_data[] = {
        0x5A, 0xA5, 0x65, 0x82, 0x00, 0x00};

    // 初始化基本参数
    CFGBUF[0x00] = 0xcc;
    InitValue(Addr迪文14标志, 0xffff);
    InitValue(Addr工件序号, 0x100);
    InitValue(Addr左右速度, 500);
    InitValue(Addr上下速度, 500);
    InitValue(Addr加减速系数, 20);
    InitValue(Addr两端上下距离, 50 * 10);
    InitValue(Addr加工位置上下距离, 400 * 10);
    InitValue(Addr未加工到加工距离, 500 * 10);
    InitValue(Addr未加工到结束距离, 760 * 10);
    InitValue(Addr未加工到右限位距离, 30 * 10);
    InitValue(Addr检测速度, 200);
    InitValue(Addr夹取位置, 70 * 10);
    InitValue(Addr铁芯直径, 40 * 10);
    InitValue(Addr当前X位置, 0);
    InitValue(Addr当前Y位置, 0);
    InitValue(AddrE80, 800);
    InitValue(AddrF20, 200);
    Uart1_SendBuffer(init_data, 6);
    Uart1_SendBuffer(CFGBUF, sizeof(CFGBUF));

    // 发送型号(0x100, 0x00, sizeof(型号数组1));

    // 将型号发送给迪文();
}
void 从缓存区发送数据给迪文(void)
{
    // 初始化基本参数
    InitValue(AddrE80, E80);
    InitValue(AddrF20, F20);

    Uart1_SendByte(0x5A);
    Uart1_SendByte(0xA5);
    Uart1_SendByte(sizeof(CFGBUF) + 3);
    Uart1_SendByte(0x82);
    Uart1_SendByte(0x00);
    Uart1_SendByte(0x00);
    Uart1_SendBuffer(CFGBUF, sizeof(CFGBUF));
}

void 发送型号(unsigned int 迪文地址, unsigned char 型号数组地址, unsigned char 长度)
{
    unsigned char addrH = (迪文地址 >> 8) & 0xFF; // 获取地址高字节
    unsigned char addrL = 迪文地址 & 0xFF;        // 获取地址低字节
    EN_485 = 1;
    Uart1_SendByte(0x5A);
    Uart1_SendByte(0xA5);
    Uart1_SendByte(长度 + 3); // 数据长度 = 文本长度 + 3个字节(地址和命令)
    Uart1_SendByte(0x82);     // 写命令
    Uart1_SendByte(addrH);    // 地址高字节
    Uart1_SendByte(addrL);    // 地址低字节
    for (unsigned char i = 型号数组地址; i < 型号数组地址 + 长度; i++)
    {
        if (型号数组[i] == 0xff)
            Uart1_SendByte(0x00);
        else
            Uart1_SendByte(型号数组[i]);
    }
    EN_485 = 0;
    DelayMs(10);
}

void 发送中文(unsigned int address, char *text, unsigned char len)
{
    for (unsigned char i = 0; i < len; i++)
    {
        tmp[i] = text[i];
    }
    // GBK
    //  https://www.toolhelper.cn/EncodeDecode/EncodeDecode
    unsigned char addrH = (address >> 8) & 0xFF; // 获取地址高字节
    unsigned char addrL = address & 0xFF;        // 获取地址低字节
    EN_485 = 1;
    Uart1_SendByte(0x5A);
    Uart1_SendByte(0xA5);
    Uart1_SendByte(len + 3); // 数据长度 = 文本长度 + 3个字节(地址和命令)
    Uart1_SendByte(0x82);    // 写命令
    Uart1_SendByte(addrH);   // 地址高字节
    Uart1_SendByte(addrL);   // 地址低字节

    // // 发送文本内容
    for (unsigned char i = 0; i < len; i++)
    {
        Uart1_SendByte(tmp[i]);
    }
    EN_485 = 0;
    DelayMs(10);
}

void 发送序号(unsigned int 工件序号)
{
    switch (工件序号)
    {
    case 0x100:
        /* code */
        发送型号(0x100, Addr型号1, 8);
        break;
    case 0x200:
        发送型号(0x100, Addr型号2, 8);
        break;
    case 0x300:
        发送型号(0x100, Addr型号3, 8);
        break;
    case 0x400:
        发送型号(0x100, Addr型号4, 8);
        break;
    case 0x500:
        发送型号(0x100, Addr型号5, 8);
        break;
    case 0x600:
        发送型号(0x100, Addr型号6, 8);
        break;
    case 0x700:
        发送型号(0x100, Addr型号7, 8);
        break;
    case 0x800:
        发送型号(0x100, Addr型号8, 8);
        break;

    default:
        发送中文(0x100, 错误, 8);
        break;
    }
}
unsigned char 保存数据(unsigned int num)
{
    num = (num - 0x100) * 2;
    // if (num < 0x1000 || num > 0x1E00)
    // {
    //     num = IAP_ADDRESS;
    //     return ERROR;
    // }
    if (sequential_write_flash_in_one_sector(num, sizeof(CFGBUF), CFGBUF) == ERROR)
    {
        Blick();
    }

    DelayMs(20);
    从接收缓冲区更新数据();
    DelayMs(20);
    从缓存区发送数据给迪文();
    return 1;
}
unsigned char 切换数据(unsigned int num)
{
    unsigned iap_num;
    iap_num = (num - 0x100) * 2;

    DelayMs(10);
    if (从扇区中读取(iap_num, sizeof(CFGBUF), CFGBUF) == ERROR)
    {
        Blick();
        // Uart1_SendString("READ SHAN");
    }

    DelayMs(20);
    给迪文上传数据(Addr迪文14标志, 0xffff);
    DelayMs(20);
    InitValue(Addr工件序号, num); // 下次重启能知道当前序号
    DelayMs(20);
    从接收缓冲区更新数据();
    从缓存区发送数据给迪文();
    DelayMs(20);
    return 1;
}