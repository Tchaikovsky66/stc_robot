#ifndef _CONFIG_H
#define _CONFIG_H

/*=== 系统常量 ===*/
#define ERR    1
#define FINISH 0
#define OK     0
#define ERROR  0

/*=== 电机编号 ===*/
#define X_MOTOR    1
#define Y1_MOTOR   2
#define Y2_MOTOR   3
#define Y1Y2_MOTOR 4

/*=== 电机方向 ===*/
#define GO_LEFT  1
#define GO_RIGHT 0
#define GO_UP    0
#define GO_DOWN  1

/*=== 限位开关逻辑：0=到达，1=未到达 ===*/
#define LIMIT_REACHED 0

/*=== 电机使能逻辑：0=使能，1=不使能 ===*/
#define MOTOR_EN_ON  0
#define MOTOR_EN_OFF 1

/*=== 夹爪方向 ===*/
#define SIDE_LEFT  0
#define SIDE_RIGHT 1

/*=== EEPROM 地址 ===*/
#define IAP_ADDR_PARAMS     0x2400
#define IAP_ADDR_MODELS     0x2600
#define IAP_FIRST_BOOT_ADDR 0x0000
#define IAP_FIRST_BOOT_MARK 0xCC

/*=== EEPROM IAP 配置 (SYSCLK 11.0592MHz, <12MHz) ===*/
#define ENABLE_IAP   0x83
#define CMD_IDLE     0
#define CMD_READ     1
#define CMD_PROGRAM  2
#define CMD_ERASE    3
#define SECTOR_SIZE  512

/*=== 迪文屏 CFGBUF 地址宏（地址 * 2 = CFGBUF 偏移） ===*/
#define ADDR_DWIN_PULSE      (0x0B * 2)
#define ADDR_DWIN_10MS_CNT   (0x0C * 2)
#define ADDR_DWIN_CALC_SPEED (0x0D * 2)
#define ADDR_DWIN_FLAG14     (0x14 * 2)
#define ADDR_DWIN_AUTO_FACE  (0x15 * 2)
#define ADDR_DWIN_RAW_LIMIT  (0x16 * 2)
#define ADDR_DWIN_INIT_BAR   (0x17 * 2)
#define ADDR_DWIN_UD_DIST    (0x18 * 2)
#define ADDR_DWIN_PROC_DIST  (0x19 * 2)
#define ADDR_DWIN_RAW2PROC   (0x1A * 2)
#define ADDR_DWIN_RAW2END    (0x1B * 2)
#define ADDR_DWIN_LR_SPEED   (0x1C * 2)
#define ADDR_DWIN_UD_SPEED   (0x1D * 2)
#define ADDR_DWIN_ACCEL      (0x1E * 2)
#define ADDR_DWIN_DET_SPEED  (0x1F * 2)
#define ADDR_DWIN_CATCH_POS  (0x20 * 2)
#define ADDR_DWIN_CORE_DIA   (0x21 * 2)
#define ADDR_DWIN_MODEL_NO   (0x22 * 2)
#define ADDR_DWIN_CUR_X      (0x24 * 2)
#define ADDR_DWIN_CUR_Y      (0x25 * 2)
#define ADDR_DWIN_WAIT_POS   (0x28 * 2)
#define ADDR_DWIN_E80        (0x2C * 2)
#define ADDR_DWIN_H170       (0x2E * 2)
#define ADDR_DWIN_F20        (0x2F * 2)

/*=== 迪文屏 型号名称地址 ===*/
#define ADDR_MODEL_NAME  0x00
#define ADDR_MODEL_1     (0x08 * 2)
#define ADDR_MODEL_2     (0x10 * 2)
#define ADDR_MODEL_3     (0x18 * 2)
#define ADDR_MODEL_4     (0x20 * 2)
#define ADDR_MODEL_5     (0x28 * 2)
#define ADDR_MODEL_6     (0x30 * 2)
#define ADDR_MODEL_7     (0x38 * 2)
#define ADDR_MODEL_8     (0x40 * 2)

/*=== 图标标志位 ===*/
#define ADDR_DWIN_ICON   (0x11 * 2)
#define ICON_L_X1   0x01
#define ICON_L_X2   0x02
#define ICON_L_Y1   0x04
#define ICON_L_Y2   0x08
#define ICON_L_Y3   0x10
#define ICON_H_RIGHT_PART  0x01
#define ICON_H_LEFT_PART   0x02
#define ICON_H_PROC_PART   0x04
#define ICON_H_RIGHT_STATE 0x08
#define ICON_H_LEFT_STATE  0x10

/*=== 运动参数默认值 ===*/
#define DEFAULT_UD_DIST      (50 * 10)
#define DEFAULT_PROC_DIST    (400 * 10)
#define DEFAULT_RAW2PROC     (500 * 10)
#define DEFAULT_RAW2END      (760 * 10)
#define DEFAULT_LR_SPEED     600
#define DEFAULT_UD_SPEED     600
#define DEFAULT_ACCEL        20
#define DEFAULT_CATCH_POS    (70 * 10)
#define DEFAULT_CORE_DIA     (40 * 10)
#define DEFAULT_WAIT_POS     (180 * 10)
#define DEFAULT_RAW_LIMIT    (30 * 10)
#define DEFAULT_F20          200
#define DEFAULT_E80          800
#define DEFAULT_H170         1700
#define DEFAULT_CALC_SPEED   500
#define DEFAULT_MODEL_NO     0x100

/*=== CFGBUF 大小 ===*/
#define CFGBUF_SIZE  96
#define MODEL_BUF_SIZE 0x88

#endif /* _CONFIG_H */
