#ifndef _EEPROM_H
#define _EEPROM_H

#include "config.h"

/*=== 单字节读写 ===*/
unsigned char eeprom_read_byte(unsigned int addr);
void eeprom_write_byte(unsigned int addr, unsigned char dat);
void eeprom_erase_sector(unsigned int addr);

/*=== 扇区读写（不跨扇区，最多 SECTOR_SIZE 字节） ===*/
unsigned char eeprom_read(unsigned int addr, unsigned int len, unsigned char *buf);
unsigned char eeprom_write(unsigned int addr, unsigned int len, unsigned char *buf);

/*=== 参数保存/切换（对应工件序号 0x100~0x800） ===*/
unsigned char 保存参数(unsigned int model_no);
unsigned char 切换参数(unsigned int model_no);

/*=== 型号名称保存/读取 ===*/
void 保存型号名称(void);
void 读取型号名称(void);

/*=== 首次启动检测 ===*/
unsigned char 是否首次启动(void);
void 标记已初始化(void);

#endif /* _EEPROM_H */
