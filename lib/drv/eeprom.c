#include "eeprom.h"
#include "stc15.h"
#include "uart.h"

#define _nop_() __asm nop __endasm

/*=== 关闭 IAP ===*/
static void iap_idle(void)
{
    IAP_CONTR = 0;
    IAP_CMD   = 0;
    IAP_TRIG  = 0;
    IAP_ADDRH = 0x80;
    IAP_ADDRL = 0;
}

/*=== 单字节读 ===*/
unsigned char eeprom_read_byte(unsigned int addr)
{
    unsigned char dat;

    IAP_CONTR = ENABLE_IAP;
    IAP_CMD   = CMD_READ;
    IAP_ADDRL = addr;
    IAP_ADDRH = addr >> 8;
    IAP_TRIG  = 0x5A;
    IAP_TRIG  = 0xA5;
    _nop_(); _nop_(); _nop_();
    dat = IAP_DATA;
    iap_idle();
    return dat;
}

/*=== 单字节写 ===*/
void eeprom_write_byte(unsigned int addr, unsigned char dat)
{
    IAP_CONTR = ENABLE_IAP;
    IAP_CMD   = CMD_PROGRAM;
    IAP_ADDRL = addr;
    IAP_ADDRH = addr >> 8;
    IAP_DATA  = dat;
    IAP_TRIG  = 0x5A;
    IAP_TRIG  = 0xA5;
    _nop_();
    iap_idle();
}

/*=== 扇区擦除 ===*/
void eeprom_erase_sector(unsigned int addr)
{
    IAP_CONTR = ENABLE_IAP;
    IAP_CMD   = CMD_ERASE;
    IAP_ADDRL = addr;
    IAP_ADDRH = addr >> 8;
    IAP_TRIG  = 0x5A;
    IAP_TRIG  = 0xA5;
    _nop_();
    iap_idle();
}

/*=== 读扇区数据到 buf ===*/
unsigned char eeprom_read(unsigned int addr, unsigned int len, unsigned char *buf)
{
    unsigned int i;
    if (len > SECTOR_SIZE) return ERR;
    for (i = 0; i < len; i++)
        buf[i] = eeprom_read_byte(addr + i);
    return OK;
}

/*=== 写扇区（先比较，全部一致则跳过；否则擦除后重写） ===*/
unsigned char eeprom_write(unsigned int addr, unsigned int len, unsigned char *buf)
{
    unsigned int i;
    unsigned char need_write = 0;

    if (len > SECTOR_SIZE) return ERR;

    /* 比较：如果有任一字节不同，就需要擦写 */
    for (i = 0; i < len; i++)
    {
        if (eeprom_read_byte(addr + i) != buf[i])
        {
            need_write = 1;
            break;
        }
    }
    if (!need_write) return OK;

    eeprom_erase_sector(addr);

    for (i = 0; i < len; i++)
    {
        eeprom_write_byte(addr + i, buf[i]);
        if (eeprom_read_byte(addr + i) != buf[i])
            return ERR;
    }
    return OK;
}

/*=== 工件序号转 EEPROM 地址：(model_no - 0x100) * 2 ===*/
static unsigned int model_to_addr(unsigned int model_no)
{
    return (model_no - 0x100) * 2;
}

/*=== 保存参数到 EEPROM ===*/
unsigned char 保存参数(unsigned int model_no)
{
    return eeprom_write(model_to_addr(model_no), CFGBUF_SIZE, (unsigned char *)CFGBUF);
}

/*=== 从 EEPROM 读参数到 CFGBUF ===*/
unsigned char 切换参数(unsigned int model_no)
{
    return eeprom_read(model_to_addr(model_no), CFGBUF_SIZE, (unsigned char *)CFGBUF);
}

/*=== 型号名称保存/读取（IAP_ADDR_MODELS 扇区） ===*/
void 保存型号名称(void)
{
    eeprom_write(IAP_ADDR_MODELS, MODEL_BUF_SIZE, (unsigned char *)型号数组);
}

void 读取型号名称(void)
{
    eeprom_read(IAP_ADDR_MODELS, MODEL_BUF_SIZE, (unsigned char *)型号数组);
}

/*=== 首次启动检测 ===*/
unsigned char 是否首次启动(void)
{
    return (eeprom_read_byte(IAP_FIRST_BOOT_ADDR) != IAP_FIRST_BOOT_MARK);
}

void 标记已初始化(void)
{
    /* 这里依赖调用方已写完 CFGBUF[0]=0xCC 再调用保存参数(默认序号) */
    eeprom_write_byte(IAP_FIRST_BOOT_ADDR, IAP_FIRST_BOOT_MARK);
}
