#include <uart.h>
#include <dwin.h>
#include <common.h>
#include "delay.h"
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
    static const unsigned char get_all_adata[] = {0x5A, 0xA5, 0x04, 0x83, 0x00, 0x10, 0x2B};
    Uart1_SendBuffer(get_all_adata, sizeof(get_all_adata));
    // Uart1_SendBuffer(get_all_data,7)
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
void 从接收缓冲区更新数据(void)
{
    测试运行 = (~CFGBUF[0x14 * 2]) & 0x01;   // 14 8
    开始调试 = (~CFGBUF[0x14 * 2]) & 0x04;   // 14A
    复位并运行 = (~CFGBUF[0x14 * 2]) & 0x10; // 14C
    参数调试 = (~CFGBUF[0x14 * 2]) & 0x20;   // 14D
    保存 = (~CFGBUF[0x14 * 2]) & 0x80;       // 14F
    两端上下距离 = (CFGBUF[Addr两端上下距离] << 8) | CFGBUF[Addr两端上下距离 + 1];
    加工位置上下距离 = (CFGBUF[Addr加工位置上下距离] << 8) | CFGBUF[Addr加工位置上下距离 + 1];
    未加工到加工距离 = (CFGBUF[Addr未加工到加工距离] << 8) | CFGBUF[Addr未加工到加工距离 + 1];
    未加工到结束距离 = (CFGBUF[Addr未加工到结束距离] << 8) | CFGBUF[Addr未加工到结束距离 + 1];
    左右速度 = (CFGBUF[Addr左右速度] << 8) | CFGBUF[Addr左右速度 + 1];
    上下速度 = (CFGBUF[Addr上下速度] << 8) | CFGBUF[Addr上下速度 + 1];
    加减速系数 = (CFGBUF[Addr加减速系数] << 8) | CFGBUF[Addr加减速系数 + 1];
    夹取位置 = (CFGBUF[Addr夹取位置] << 8) | CFGBUF[Addr夹取位置 + 1];
    铁芯直径 = (CFGBUF[Addr铁芯直径] << 8) | CFGBUF[Addr铁芯直径 + 1];
    当前X位置 = (CFGBUF[Addr当前X位置] << 8) | CFGBUF[Addr当前X位置 + 1];
    当前Y位置 = (CFGBUF[Addr当前Y位置] << 8) | CFGBUF[Addr当前Y位置 + 1];
}
void 从迪文更新数据(void)
{
    获取迪文所有数据();
    DelayMs(20);
    测试运行 = (~CFGBUF[0x14 * 2]) & 0x01;   // 14 8
    开始调试 = (~CFGBUF[0x14 * 2]) & 0x04;   // 14A
    复位并运行 = (~CFGBUF[0x14 * 2]) & 0x10; // 14C
    参数调试 = (~CFGBUF[0x14 * 2]) & 0x20;   // 14D
    保存 = (~CFGBUF[0x14 * 2]) & 0x80;       // 14F
    两端上下距离 = (CFGBUF[Addr两端上下距离] << 8) | CFGBUF[Addr两端上下距离 + 1];
    加工位置上下距离 = (CFGBUF[Addr加工位置上下距离] << 8) | CFGBUF[Addr加工位置上下距离 + 1];
    未加工到加工距离 = (CFGBUF[Addr未加工到加工距离] << 8) | CFGBUF[Addr未加工到加工距离 + 1];
    未加工到结束距离 = (CFGBUF[Addr未加工到结束距离] << 8) | CFGBUF[Addr未加工到结束距离 + 1];
    左右速度 = (CFGBUF[Addr左右速度] << 8) | CFGBUF[Addr左右速度 + 1];
    上下速度 = (CFGBUF[Addr上下速度] << 8) | CFGBUF[Addr上下速度 + 1];
    加减速系数 = (CFGBUF[Addr加减速系数] << 8) | CFGBUF[Addr加减速系数 + 1];
    夹取位置 = (CFGBUF[Addr夹取位置] << 8) | CFGBUF[Addr夹取位置 + 1];
    铁芯直径 = (CFGBUF[Addr铁芯直径] << 8) | CFGBUF[Addr铁芯直径 + 1];
    当前X位置 = (CFGBUF[Addr当前X位置] << 8) | CFGBUF[Addr当前X位置 + 1];
    当前Y位置 = (CFGBUF[Addr当前Y位置] << 8) | CFGBUF[Addr当前Y位置 + 1];
}

void 初始化迪文数据(void)
{
    static const char init_data[] = {
        0x5A, 0xA5, 0x59, 0x82, 0x00, 0x00};

    // 初始化基本参数
    InitValue(Addr迪文14标志, 0xffff);
    InitValue(Addr左右速度, 左右速度);
    InitValue(Addr上下速度, 上下速度);
    InitValue(Addr加减速系数, 加减速系数);
    InitValue(Addr两端上下距离, 两端上下距离);
    InitValue(Addr加工位置上下距离, 加工位置上下距离);
    InitValue(Addr未加工到加工距离, 未加工到加工距离);
    InitValue(Addr未加工到结束距离, 未加工到结束距离);
    InitValue(Addr夹取位置, 夹取位置);
    InitValue(Addr铁芯直径, 铁芯直径);
    InitValue(Addr当前X位置, 当前X位置);
    InitValue(Addr当前Y位置, 当前Y位置);

    // CFGBUF[0] = 1;

    // 发送固定头部数据
    // for (unsigned char i = 0; i < sizeof(init_data); i++)
    // {
    //     Uart1_SendByte(init_data[i]);
    // }
    Uart1_SendBuffer(init_data, 6);

    // 发送配置数据
    // for (int i = 0; i < 89; i++)
    // {
    //     Uart1_SendByte(CFGBUF[i]);
    // }
    Uart1_SendBuffer(CFGBUF, 89);
}