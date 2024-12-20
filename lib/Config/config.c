#include <config.h>
#include <uart.h>
#include <position.h>
#include <delay.h>
#include "../../include/main.h"
#include "../../include/stc15.h"

// 配置变量定义 - 这些变量访问频率不高，可以放在片外RAM
__xdata int model = 0;
__xdata int core_diameter = 0;
__xdata int end_face_distance = 0;
__xdata int up_down_speed = 0;
__xdata int up_down_distance = 0;
__xdata int left_right_speed = 0;
__xdata int left_right_distance = 0;

// 标志位使用bit类型
volatile __bit all_data_flag = 0;
volatile __bit left_flag = 0;
volatile __bit right_flag = 0;
volatile __bit up_flag = 0;
volatile __bit down_flag = 0;
volatile __bit init_pos_flag = 0;

/**
 * @brief 向指定地址写入16位数据
 * @param address 目标地址
 * @param data 要写入的16位数据
 * @note 会自动将16位数据拆分为高低字节存入CFGBUF
 */
void InitValue(unsigned char address, unsigned int data)
{
    unsigned char low_byte = data & 0xFF;
    unsigned char high_byte = (data >> 8) & 0xFF;
    CFGBUF[address] = high_byte;
    CFGBUF[address + 1] = low_byte;
}

/**
 * @brief 初始化系统参数并发送到显示屏
 * @note 包括上下距离、速度等基本参数的初始化
 */
void InitData(void)
{
    static const unsigned char init_data[] = {
        0x5A, 0xA5, 0x59, 0x82, 0x00, 0x00};

    // 初始化基本参数
    InitValue(UP_DOWN_DISTANCE, 40);
    InitValue(UP_DOWN_SPEED, 200);
    InitValue(LEFT_RIGHT_SPEED, 200);
    InitValue(LEFT_RIGHT_DISTANCE, 50);
    InitValue(MODEL, 1);
    CFGBUF[0] = 1;

    // 发送固定头部数据
    for (unsigned char i = 0; i < sizeof(init_data); i++)
    {
        Uart1_SendByte(init_data[i]);
    }

    // 发送配置数据
    for (int i = 0; i < 89; i++)
    {
        Uart1_SendByte(CFGBUF[i]);
    }
}

/**
 * @brief 更新并上传单个参数到显示屏
 * @param address 参数地址
 * @param data 参数值
 * @note 使用串口发送数据到显示屏
 */

void UploadData(unsigned char address, unsigned int data)
{
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
}

/**
 * @brief 从显示屏更新所有参数到本地变量
 * @note 包括位置坐标、速度、距离等所有参数的更新
 */
void update_parameters(void)
{
    init_pos_flag = CFGBUF[0x13 * 2 + 1] >> 3;
    sign1_flag = CFGBUF[0x13 * 2] & 0x01;
    sign2_flag = CFGBUF[0x13 * 2] & 0x02;
    sign3_flag = CFGBUF[0x13 * 2] & 0x04;
    sign1_x = (CFGBUF[0x19 * 2] << 8) | CFGBUF[0x19 * 2 + 1];
    sign1_y = (CFGBUF[0x1A * 2] << 8) | CFGBUF[0x1A * 2 + 1];
    sign2_x = (CFGBUF[0x1B * 2] << 8) | CFGBUF[0x1B * 2 + 1];
    sign2_y = (CFGBUF[0x1C * 2] << 8) | CFGBUF[0x1C * 2 + 1];
    sign3_x = (CFGBUF[0x1D * 2] << 8) | CFGBUF[0x1D * 2 + 1];
    sign3_y = (CFGBUF[0x1E * 2] << 8) | CFGBUF[0x1E * 2 + 1];
    up_down_distance = (CFGBUF[0x23 * 2] << 8) | CFGBUF[0x23 * 2 + 1];
    left_right_speed = (CFGBUF[0x24 * 2] << 8) | CFGBUF[0x24 * 2 + 1];
    up_down_speed = (CFGBUF[0x25 * 2] << 8) | CFGBUF[0x25 * 2 + 1];
    left_right_distance = (CFGBUF[0x22 * 2] << 8) | CFGBUF[0x22 * 2 + 1];
    model = (CFGBUF[0x28 * 2] << 8) | CFGBUF[0x28 * 2 + 1];
    core_diameter = (CFGBUF[0x29 * 2] << 8) | CFGBUF[0x29 * 2 + 1];
    end_face_distance = (CFGBUF[0x2A * 2] << 8) | CFGBUF[0x2A * 2 + 1];
    all_data_flag = CFGBUF[0x16 * 2 + 1] & 0x01;
    left_flag = CFGBUF[0x16 * 2 + 1] & 0x02;
    right_flag = CFGBUF[0x16 * 2 + 1] & 0x04;
    up_flag = CFGBUF[0x16 * 2 + 1] & 0x08;
    down_flag = CFGBUF[0x16 * 2 + 1] & 0x10;
}