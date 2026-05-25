#include "process.h"
#include "stc15.h"
#include "gpio.h"
#include "config.h"
#include "delay.h"
#include "uart.h"
#include "dwin.h"
#include "motor.h"
#include "motion.h"
#include "eeprom.h"
#include "params.h"

/*=== 等待物料信号（RAW_PRESENT 或 屏幕模拟按钮） ===*/
static void 等待来料(void)
{
    GoToPage(20);
    while (1)
    {
        if (接收到数据)
        {
            从接收缓冲区更新数据();
            接收到数据 = 0;
        }
        if (模拟来料)
        {
            模拟来料 = 0;
            return;
        }
        if (RAW_PRESENT == 0)
        {
            DelayMs(20);
            if (RAW_PRESENT == 0)
                return;
        }
        DelayMs(10);
    }
}

/*=== 移动到物料位置并夹取 ===*/
static void 移动到物料位置(void)
{
    GoToPage(21);
    MoveToPosition(等待处位置 - E80, 0, 0, 左右速度);
    GoToPage(22);
    MoveToPosition(等待处位置 - E80 - F20, 0, 0, DEFAULT_F20);
    GoToPage(23);
    MoveToPosition(等待处位置 - H170, 0, 0, 左右速度);
    GoToPage(25);
    MoveToPosition(等待处位置 - H170, 两端上下距离, 0, 左右速度);
    夹取(SIDE_RIGHT);
    DelayMs(200);
}

/*=== 从物料位置回等待位置 ===*/
static void 从物料位置移动到等待位置(void)
{
    GoToPage(27);
    MoveToPosition(等待处位置 - H170, 0, 0, 左右速度);
    GoToPage(29);
    MoveToPosition(等待处位置, 0, 0, 左右速度);
}

/*=== 去加工设备夹取已加工物料（左爪） ===*/
static void 去加工设备夹取已加工物料(void)
{
    GoToPage(31);
    MoveToPosition(未加工到加工距离, 0, 加工位置上下距离, 左右速度);
    夹取(SIDE_LEFT);
    DelayMs(200);
    MoveToPosition(未加工到加工距离, 0, 0, 左右速度);
}

/*=== 去加工设备放待加工工件（右爪） ===*/
static void 去加工设备放待加工工件(void)
{
    GoToPage(35);
    MoveToPosition(未加工到加工距离, 0, 0, 左右速度);
    MoveToPosition(未加工到加工距离, 加工位置上下距离, 0, 左右速度);
    放工件(SIDE_RIGHT);
    DelayMs(200);
    MoveToPosition(未加工到加工距离, 0, 0, 左右速度);
}

/*=== 去成品位置放加工好工件 ===*/
static void 去成品位置放加工好工件(void)
{
    GoToPage(40);
    MoveToPosition(未加工到结束距离, 0, 两端上下距离, 左右速度);
    放工件(SIDE_LEFT);
    DelayMs(200);
    MoveToPosition(未加工到结束距离, 0, 0, 左右速度);
}

/*=== 回等待位置 ===*/
static void 回到等待位置(void)
{
    GoToPage(50);
    MoveToPosition(等待处位置, 0, 0, 左右速度);
}

/*=== 运行流程主函数：根据加工区是否有工件分首次/循环两条路径 ===*/
void process_run(void)
{
    电机复位();
    运行前复位();
    通知加工设备复位();
    放工件(SIDE_RIGHT);
    放工件(SIDE_LEFT);

    while (1)
    {
        /* 检查是否退出运行（迪文屏再次按下复位并运行） */
        if (复位并运行 == 0)
            return;

        等待来料();
        移动到物料位置();
        从物料位置移动到等待位置();

        if (MACHINE_HAS_PART && !MACHINE_ERROR)
        {
            /* 加工设备有工件：等加工完→取成品→放新料→通知运行→等取走→放成品区 */
            等待加工完成();
            去加工设备夹取已加工物料();
            去加工设备放待加工工件();
            通知加工设备运行();
            等待工件取走();
            去成品位置放加工好工件();
        }
        else
        {
            /* 首次：加工区空，直接放料→通知运行 */
            去加工设备放待加工工件();
            通知加工设备运行();
        }
        回到等待位置();
    }
}

/*=== 参数调试流程：4 步分别记录 4 个参数 ===*/
void process_debug(void)
{
    电机复位();
    给迪文上传数据(ADDR_DWIN_FLAG14, 0xFFFF);
    放工件(SIDE_RIGHT);
    放工件(SIDE_LEFT);
    GoToPage(15);

    /* 第1步：测量工件/夹取位置 */
    while (1)
    {
        if (接收到数据)
        {
            从接收缓冲区更新数据();
            接收到数据 = 0;
        }
        DelayMs(20);
        if (开始调试)
        {
            开始调试 = 0;
            给迪文上传数据(ADDR_DWIN_FLAG14, 0xFFFF);
            DelayMs(20);
            GoToPage(16);
            MoveToPosition(300, 900 - 铁芯直径 - 50, 0, 200);
            break;
        }
    }

    /* 第2步：调试抓取点 */
    while (1)
    {
        if (接收到数据)
        {
            从接收缓冲区更新数据();
            接收到数据 = 0;
            MoveToPosition(当前X位置, 当前Y位置, 0, 200);
        }
        if (开始调试)
        {
            开始调试 = 0;
            未加工到右限位距离 = 当前x坐标;
            两端上下距离       = 当前Y位置;
            给迪文上传数据(ADDR_DWIN_RAW_LIMIT, 未加工到右限位距离);
            给迪文上传数据(ADDR_DWIN_UD_DIST,   两端上下距离);
            给迪文上传数据(ADDR_DWIN_FLAG14, 0xFFFF);
            当前x坐标 = 0;
            铁芯直径 = 900 - 两端上下距离;
            给迪文上传数据(ADDR_DWIN_CORE_DIA, 铁芯直径);

            MoveToPosition(0, 两端上下距离 - 20, 0, 左右速度);
            MoveToPosition(夹取位置 + 500, 两端上下距离 - 30, 0, 左右速度);
            实时测量夹取位置_脉冲数 = 0;
            MoveToPosition(0, 两端上下距离 - 20, 0, 150);
            夹取位置 = 实时测量夹取位置_脉冲数 - 300;
            给迪文上传数据(ADDR_DWIN_CATCH_POS, 夹取位置);
            实时测量夹取位置_脉冲数 = 0;
            MoveToPosition(0, 两端上下距离, 0, 150);

            夹取(SIDE_RIGHT);
            MoveToPosition(0, 0, 0, 200);
            MoveToPosition(5000, 4000, 0, 200);
            GoToPage(17);
            break;
        }
    }

    /* 第3步：调试加工位置 */
    while (1)
    {
        if (接收到数据)
        {
            从接收缓冲区更新数据();
            接收到数据 = 0;
            MoveToPosition(当前X位置, 当前Y位置, 0, 200);
        }
        if (开始调试)
        {
            开始调试 = 0;
            未加工到加工距离 = 当前X位置;
            加工位置上下距离 = 当前Y位置;
            给迪文上传数据(ADDR_DWIN_RAW2PROC,  未加工到加工距离);
            给迪文上传数据(ADDR_DWIN_PROC_DIST, 加工位置上下距离);
            给迪文上传数据(ADDR_DWIN_FLAG14, 0xFFFF);

            MoveToPosition(当前X位置, 0, 0, 200);
            MoveToPosition(7600, 0, 两端上下距离, 200);
            GoToPage(18);
            break;
        }
    }

    /* 第4步：调试结束位置 */
    while (1)
    {
        if (接收到数据)
        {
            从接收缓冲区更新数据();
            接收到数据 = 0;
            MoveToPosition(当前X位置, 0, 当前Y位置, 200);
        }
        if (开始调试)
        {
            开始调试 = 0;
            未加工到结束距离 = 当前X位置;
            给迪文上传数据(ADDR_DWIN_RAW2END, 未加工到结束距离);
            MoveToPosition(当前X位置, 0, 0, 200);
            MoveToPosition(0, 两端上下距离, 0, 200);
            放工件(SIDE_RIGHT);
            MoveToPosition(0, 0, 0, 200);
            GoToPage(2);
            break;
        }
    }
}

/*=== 测试运行 ===*/
void process_test(void)
{
    给迪文上传数据(ADDR_DWIN_FLAG14, 0xFFFF);
    DelayMs(20);
    GoToPage(70);

    while (1)
    {
        获取迪文所有数据();
        DelayMs(20);
        从接收缓冲区更新数据();

        if (测试运行 == 0)
        {
            给迪文上传数据(ADDR_DWIN_FLAG14, 0xFFFF);
            从迪文更新数据();
            GoToPage(2);
            return;
        }

        给迪文上传数据(ADDR_DWIN_CALC_SPEED, 计算速度);
        DelayMs(1000);
        MoveToPosition(未加工到加工距离, 0, 0, 左右速度);
        DelayMs(1000);
        MoveToPosition(0, 0, 0, 左右速度);
    }
}

/*=== 保存参数到 EEPROM ===*/
void process_save(void)
{
    给迪文上传数据(ADDR_DWIN_FLAG14, 0xFFFF);
    DelayMs(20);
    GoToPage(57);
    DelayMs(20);
    获取迪文所有数据();
    DelayMs(800);

    if (eeprom_write(IAP_ADDR_PARAMS, CFGBUF_SIZE, (unsigned char *)CFGBUF) != OK)
        Blick();

    DelayMs(20);
    if (保存参数(工件序号) != OK)
        Blick();

    从缓存区发送数据给迪文();
    读取型号名称();
    发送型号(0x100, 0x00, MODEL_BUF_SIZE);
    发送序号(工件序号);
    GoToPage(2);
}

/*=== 保存工件名称 ===*/
void process_save_name(void)
{
    给迪文上传数据(ADDR_DWIN_FLAG14, 0xFFFF);
    DelayMs(20);
    GoToPage(57);
    DelayMs(20);
    获取所有工件名称();
    DelayMs(500);

    保存型号名称();
    DelayMs(500);
    发送序号(工件序号);
    DelayMs(200);
    GoToPage(2);
}

/*=== 恢复出厂设置 ===*/
void process_factory_reset(void)
{
    DelayMs(20);
    恢复出厂设置();
    InitValue(ADDR_DWIN_MODEL_NO, 工件序号);
    DelayMs(1000);
    DelayMs(20);
    GoToPage(3);
    DelayMs(20);

    if (eeprom_write(IAP_ADDR_PARAMS, CFGBUF_SIZE, (unsigned char *)CFGBUF) != OK)
        Blick();
    if (保存参数(工件序号) != OK)
        Blick();

    DelayMs(20);
    从接收缓冲区更新数据();
    从缓存区发送数据给迪文();

    保存型号名称();
    发送序号(工件序号);
    GoToPage(3);
}

/*=== 切换工件型号（接收到新序号时调用） ===*/
void process_switch_model(void)
{
    if (切换参数(工件序号) != OK)
        Blick();

    InitValue(ADDR_DWIN_MODEL_NO, 工件序号);
    if (eeprom_write(IAP_ADDR_PARAMS, CFGBUF_SIZE, (unsigned char *)CFGBUF) != OK)
        Blick();

    DelayMs(10);
    发送序号(工件序号);
    DelayMs(10);
    获取所有工件名称();
    保存型号名称();
}
