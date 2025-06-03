
/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC15F4K60S4 系列 内部EEPROM举例--------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966-------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.GXWMCU.com --------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了STC的资料及程序        */
/* 如果要在文章中应用此代码,请在文章中注明使用了STC的资料及程序        */
/*---------------------------------------------------------------------*/

// 本示例在Keil开发环境下请选择Intel的8058芯片型号进行编译
// 若无特别说明,工作频率一般为11.0592MHz

#include "../../include/stc15.h"
#include "eeprom.h"
#include "../../lib/Uart/uart.h"


//BYTE __xdata protect_buffer[USED_BYTE_QTY_IN_ONE_SECTOR];


/*----------------------------
关闭IAP
----------------------------*/
void IapIdle(void)
{
    IAP_CONTR = 0;    // 关闭IAP功能
    IAP_CMD = 0;      // 清除命令寄存器
    IAP_TRIG = 0;     // 清除触发寄存器
    IAP_ADDRH = 0x80; // 将地址设置到非IAP区域
    IAP_ADDRL = 0;
}

/*----------------------------
从ISP/IAP/EEPROM区域读取一字节
----------------------------*/
BYTE IapReadByte(WORD addr)
{
    BYTE dat; // 数据缓冲区

    IAP_CONTR = ENABLE_IAP; // 使能IAP
    IAP_CMD = CMD_READ;     // 设置IAP命令
    IAP_ADDRL = addr;       // 设置IAP低地址
    IAP_ADDRH = addr >> 8;  // 设置IAP高地址
    IAP_TRIG = 0x5a;        // 写触发命令(0x5a)
    IAP_TRIG = 0xa5;        // 写触发命令(0xa5)
    _nop_();                // 等待ISP/IAP/EEPROM操作完成
    _nop_();                // 等待ISP/IAP/EEPROM操作完成
    _nop_();                // 等待ISP/IAP/EEPROM操作完成

    dat = IAP_DATA;         // 读ISP/IAP/EEPROM数据
    IapIdle();              // 关闭IAP功能

    return dat; // 返回
}

/*----------------------------
写一字节数据到ISP/IAP/EEPROM区域
----------------------------*/
void IapProgramByte(WORD addr, BYTE dat)
{
    IAP_CONTR = ENABLE_IAP; // 使能IAP
    IAP_CMD = CMD_PROGRAM;  // 设置IAP命令
    IAP_ADDRL = addr;       // 设置IAP低地址
    IAP_ADDRH = addr >> 8;  // 设置IAP高地址
    IAP_DATA = dat;         // 写ISP/IAP/EEPROM数据
    IAP_TRIG = 0x5a;        // 写触发命令(0x5a)
    IAP_TRIG = 0xa5;        // 写触发命令(0xa5)
    _nop_();                // 等待ISP/IAP/EEPROM操作完成

    IapIdle();
}

/*----------------------------
扇区擦除
----------------------------*/
void IapEraseSector(WORD addr)
{
    IAP_CONTR = ENABLE_IAP; // 使能IAP
    IAP_CMD = CMD_ERASE;    // 设置IAP命令
    IAP_ADDRL = addr;       // 设置IAP低地址
    IAP_ADDRH = addr >> 8;  // 设置IAP高地址
    IAP_TRIG = 0x5a;        // 写触发命令(0x5a)
    IAP_TRIG = 0xa5;        // 写触发命令(0xa5)
    _nop_();                // 等待ISP/IAP/EEPROM操作完成
    IapIdle();
}

/* 写数据进 数据Flash存储器, 只在同一个扇区内写，不保留原有数据 */
/* begin_addr,被写数据Flash开始地址；counter,连续写多少个字节； array[]，数据来源   */
BYTE sequential_write_flash_in_one_sector(WORD begin_addr, WORD counter, BYTE array[])
{
    WORD i = 0;
    WORD in_sector_begin_addr = 0;
    WORD sector_addr = 0;

    /* 判是否是有效范围,此函数不允许跨扇区操作 */
    if (counter > USED_BYTE_QTY_IN_ONE_SECTOR)
        return ERROR;
    in_sector_begin_addr = begin_addr & 0x01ff;
    if ((in_sector_begin_addr + counter) > USED_BYTE_QTY_IN_ONE_SECTOR)
        return ERROR;

    /* 擦除 要修改/写入 的扇区 */
    IapEraseSector(begin_addr);

    for (i = 0; i < counter; i++)
    {
        /* 写一个字节 */
        IapProgramByte(begin_addr, array[i]);
        /*  比较对错 */
        if (IapReadByte(begin_addr) != array[i])
        {
            IapIdle();
            return ERROR;
        }
        begin_addr++;
    }
    IapIdle();
    return 1;
}

/* 写数据进数据Flash存储器(EEPROM), 只在同一个扇区内写，保留同一扇区中不需修改的数据    */
/* begin_addr,被写数据Flash开始地址；counter,连续写多少个字节； array[]，数据来源       */
// BYTE 写数据进扇区(WORD begin_addr, WORD counter, BYTE array[])
// {
//     WORD i = 0;
//     WORD in_sector_begin_addr = 0;
//     WORD sector_addr = 0;
//     WORD byte_addr = 0;

//     /* 判是否是有效范围,此函数不允许跨扇区操作 */
//     if (counter > USED_BYTE_QTY_IN_ONE_SECTOR)
//         return ERROR;
//     in_sector_begin_addr = begin_addr & 0x01ff;
//     /* 假定从扇区的第0个字节开始，到USED_BYTE_QTY_IN_ONE_SECTOR-1个字节结束,后面部分不用,程序易编写 */
//     if ((in_sector_begin_addr + counter) > USED_BYTE_QTY_IN_ONE_SECTOR)
//         return ERROR;

//     /* 将该扇区数据 0 - (USED_BYTE_QTY_IN_ONE_SECTOR-1) 字节数据读入缓冲区保护 */
//     sector_addr = (begin_addr & 0xfe00);
//     byte_addr = sector_addr;

//     for (i = 0; i < USED_BYTE_QTY_IN_ONE_SECTOR; i++)
//     {
//         protect_buffer[i] = IapReadByte(byte_addr++);
//     }

//     /* 将要写入的数据写入保护缓冲区的相应区域,其余部分保留 */
//     for (i = 0; i < counter; i++)
//     {
//         protect_buffer[in_sector_begin_addr++] = array[i];
//     }

//     /* 擦除 要修改/写入 的扇区 */
//     IapEraseSector(sector_addr);

//     /* 将保护缓冲区的数据写入 Data Flash, EEPROM */
//     byte_addr = sector_addr;
//     for (i = 0; i < USED_BYTE_QTY_IN_ONE_SECTOR; i++)
//     {
//         /* 写一个字节 */
//         IapProgramByte(byte_addr, protect_buffer[i]);
//         /*  比较对错 */
//         if (IapReadByte(begin_addr) != protect_buffer[i])
//         {
//             IapIdle();
//             return ERROR;
//         }
//         byte_addr++;
//     }
//     IapIdle();
//     return 1;
// }
/*----------------------------
从扇区读取指定字节数到数组中
参数:
    begin_addr: 起始地址
    counter: 要读取的字节数
    array[]: 存储读取数据的数组
返回值:
    OK: 读取成功
    ERROR: 读取失败
----------------------------*/
BYTE 从扇区中读取(WORD begin_addr, WORD counter, BYTE array[])
{
    WORD i = 0;
    WORD in_sector_begin_addr = 0;
    
    /* 判断是否是有效范围,此函数不允许跨扇区操作 */
    if (counter > USED_BYTE_QTY_IN_ONE_SECTOR)
        return ERROR;
        
    in_sector_begin_addr = begin_addr & 0x01ff;
    /* 判断是否会超出扇区范围 */
    if ((in_sector_begin_addr + counter) > USED_BYTE_QTY_IN_ONE_SECTOR)
        return ERROR;

    /* 读取数据到数组中 */
    for (i = 0; i < counter; i++)
    {
        array[i] = IapReadByte(begin_addr + i);
    }
    
    IapIdle();
    return 1;
}

/*----------------------------
初始化串口
----------------------------*/
// void InitUart()
// {
//     SCON = 0x5a; // 设置串口为8位可变波特率
// #if URMD == 0
//     T2L = (65536 - (FOSC / 4 / BAUD));
//     T2H = (65536 - (FOSC / 4 / BAUD)) >> 8;
//     AUXR = 0x14;  // T2为1T模式, 并启动定时器2
//     AUXR |= 0x01; // 选择定时器2为串口1的波特率发生器
// #elif URMD == 1
//     AUXR = 0x40; // 定时器1为1T模式
//     TMOD = 0x00; // 定时器1为模式0(16位自动重载)
//     TL1 = (65536 - (FOSC / 4 / BAUD));
//     TH1 = (65536 - (FOSC / 4 / BAUD)) >> 8;
//     TR1 = 1; // 定时器1开始启动
// #else
//     TMOD = 0x20; // 设置定时器1为8位自动重装载模式
//     AUXR = 0x40; // 定时器1为1T模式
//     TH1 = TL1 = (256 - (FOSC / 32 / BAUD));
//     TR1 = 1;
// #endif
// }

// /*----------------------------
// 发送串口数据
// ----------------------------*/
// BYTE SendData(BYTE dat)
// {
//     while (!TI)
//         ;       // 等待前一个数据发送完成
//     TI = 0;     // 清除发送标志
//     SBUF = dat; // 发送当前数据

//     return dat;
// }
// BYTE write_flash_with_protect_in_one_sector(WORD begin_addr, WORD counter, BYTE array[])
// {
//     WORD i = 0;
//     WORD in_sector_begin_addr = 0;
//     WORD sector_addr = 0;
//     WORD byte_addr = 0;

//     /* 判是否是有效范围,此函数不允许跨扇区操作 */
//     if (counter > USED_BYTE_QTY_IN_ONE_SECTOR)
//         return ERROR;
//     in_sector_begin_addr = begin_addr & 0x01ff;
//     /* 假定从扇区的第0个字节开始，到USED_BYTE_QTY_IN_ONE_SECTOR-1个字节结束,后面部分不用,程序易编写 */
//     if ((in_sector_begin_addr + counter) > USED_BYTE_QTY_IN_ONE_SECTOR)
//         return ERROR;

//     /* 将该扇区数据 0 - (USED_BYTE_QTY_IN_ONE_SECTOR-1) 字节数据读入缓冲区保护 */
//     sector_addr = (begin_addr & 0xfe00);
//     byte_addr = sector_addr;

//     for (i = 0; i < USED_BYTE_QTY_IN_ONE_SECTOR; i++)
//     {
//         protect_buffer[i] = IapReadByte(byte_addr++);
//     }

//     /* 将要写入的数据写入保护缓冲区的相应区域,其余部分保留 */
//     for (i = 0; i < counter; i++)
//     {
//         protect_buffer[in_sector_begin_addr++] = array[i];
//     }

//     /* 擦除 要修改/写入 的扇区 */
//     IapEraseSector(sector_addr);

//     /* 将保护缓冲区的数据写入 Data Flash, EEPROM */
//     byte_addr = sector_addr;
//     for (i = 0; i < USED_BYTE_QTY_IN_ONE_SECTOR; i++)
//     {
//         /* 写一个字节 */
//         IapProgramByte(byte_addr, protect_buffer[i]);
//         /*  比较对错 */
//         if (Uart1_SendByte(IapReadByte(begin_addr)) != protect_buffer[i])
//         {
//             IapIdle();
//             return ERROR;
//         }
//         byte_addr++;
//     }
//     IapIdle();
//     return 1;
// }