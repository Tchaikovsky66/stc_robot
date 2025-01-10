#include <stdio.h>
#include <8051.h>
#include <main.h>
#include <uart.h>
#include <delay.h>
#include <button.h>
#include <isr.h>
#include <motor.h>
#include <position.h>
#include <config.h>
#include "../../include/stc15.h"
#include "../../include/main.h"

__xdata char buf[100] = {0};
__xdata char buf1[10] = {0xff};
float tmp_distance = 0;
// 函数原型声明
// char y1_get(void);
// void go_test(void);

void catch_core(void)
{
    Uart1_SendString("catch_core");
    DelayMs(100);
}

void put_core(void)
{
    Uart1_SendString("put_core");
    DelayMs(100);
}

// // 函数实现
void wait_material(void)
{
    MoveToPosition(sign2_x, 0, 0, left_right_speed);
    Uart1_SendString("wait_material");
    DelayMs(10);
}
void y1_get_material(void)
{
    Uart1_SendString("y1_get_material");
    DelayMs(10);
    MoveToPosition(sign1_x, 0, 0, left_right_speed);
    MoveToPosition(sign1_x, 60, 0, 100);
    MoveToPosition(sign1_x, sign1_y, 0, left_right_speed);
    catch_core();
    MoveToPosition(sign1_x, 0, 0, 100);
}
void y2_wait_processing(void)
{
    Uart1_SendString("y2_wait_processing");
    DelayMs(10);
    MoveToPosition(sign2_x, 0, 0, left_right_speed);
}
void y2_get_processing(void)
{
    Uart1_SendString("y2_get_processing");
    DelayMs(10);
    MoveToPosition(sign2_x - 320, 0, sign2_y2, up_down_speed);
    catch_core();
    MoveToPosition(sign2_x - 320, 0, 0, up_down_speed);
}
void y1_put_processing(void)
{
    Uart1_SendString("y1_put_processing");
    DelayMs(10);
    MoveToPosition(sign2_x, sign2_y, 0, up_down_speed);
    put_core();
    MoveToPosition(sign2_x, 0, 0, up_down_speed);
}
void y2_put_finish(void)
{
    Uart1_SendString("y2_put_finish");
    DelayMs(10);
    MoveToPosition(sign3_x, 0, sign3_y2, up_down_speed);
    put_core();
    MoveToPosition(sign3_x, 0, 0, up_down_speed);
}

void go_test(void)
{
    if (RCVOK == 0xff)
    {
        update_parameters();
        RCVOK = 0x00;
        Uart1_SendString("update_parameters\r\n");
        DelayMs(10);
    }
    // core_diameter = (float)31.9;
    // tmp_distance = 0;
    // 等待来料
    GoToPage(0x14); // page20
    wait_material();
    // 2未加工工件抓取
    GoToPage(0x15); // page21
    GoToPage(0x16); // page22
    y1_get_material();
    // 3等待加工完成
    GoToPage(0x17); // page23
    y2_wait_processing();
    // 4加工工件抓取
    GoToPage(0x18); // page24
    y2_get_processing();
    // 5放加工工件
    GoToPage(0x19); // page25
    y1_put_processing();
    // 6等待工件取走
    GoToPage(0x1A); // page26
    // y2_wait_finish();
    // 7放加工工件
    GoToPage(0x1B); // page27
    y2_put_finish();
    // 8回到零位
    GoToPage(0x1C); // page28
}
char set_sign1(void)
{
    Uart1_SendString("set_sign1\r\n");
    GoToPage(0x10); // 16

    sign1_x = 200;
    sign1_y = 60;
    sign1_y2 = 0;
    MoveToPosition(sign1_x, sign1_y, sign1_y2, 200);
    UploadPosition(1, sign1_x, sign1_y, sign1_y2);

    sign1_x = 20;
    MoveToPosition(sign1_x, sign1_y, sign1_y2, 100);
    UploadPosition(1, sign1_x, sign1_y, sign1_y2);
    end_face_distance = set_to_catch_steps / 20;

    while (1)
    {
        if (RCVOK == 0xff)
        {
            update_parameters();
            RCVOK = 0x00;
        }
        if (sign2_flag == 1)
        {
            break;
        }
        else if (left_flag == 1)
        {
            sign1_x += left_right_distance;
            MoveToPosition(sign1_x, sign1_y, sign1_y2, 50);
            UploadPosition(1, sign1_x, sign1_y, sign1_y2);
            left_flag = 0;
            CFGBUF[0x16 * 2] = 0;
            UploadData(0x16 * 2, 0x00);
        }
        else if (right_flag == 1)
        {
            sign1_x -= left_right_distance;
            MoveToPosition(sign1_x, sign1_y, sign1_y2, 50);
            UploadPosition(1, sign1_x, sign1_y, sign1_y2);
            right_flag = 0;
            CFGBUF[0x16 * 2] = 0;
            UploadData(0x16 * 2, 0x00);
        }
        else if (up_flag == 1)
        {
            sign1_y -= left_right_distance;
            MoveToPosition(sign1_x, sign1_y, sign1_y2, 50);
            UploadPosition(1, sign1_x, sign1_y, sign1_y2);
            up_flag = 0;
            CFGBUF[0x16 * 2] = 0;
            UploadData(0x16 * 2, 0x00);
        }
        else if (down_flag == 1)
        {
            sign1_y += left_right_distance;
            MoveToPosition(sign1_x, sign1_y, sign1_y2, 50);
            UploadPosition(1, sign1_x, sign1_y, sign1_y2);
            down_flag = 0;
            CFGBUF[0x16 * 2] = 0;
            UploadData(0x16 * 2, 0x00);
        }
    }
    end_face_distance = 200 - sign1_x - end_face_distance;
    core_diameter = 115 - sign1_y;
    UploadData(CORE_DIAMETER_ADDRESS, (int)core_diameter);
    UploadData(END_FACE_DISTANCE_ADDRESS, (int)end_face_distance);
    catch_core();
    MoveToPosition(sign1_x, 0, 0, 100);
    return FINISH;
}
char set_sign2(void)
{
    Uart1_SendString("set_sign2\r\n");
    GoToPage(0x11); // 17

    Allow_Move = 1;

    MoveToPosition(500, 0, 0, 200);
    DelayMs(10);
    MoveToPosition(500, 200, 0, 200);
    Allow_Move = 0;
    sign2_x = 500;
    sign2_y = 200;
    UploadPosition(2, sign2_x, sign2_y, 0);
    while (1)
    {
        if (RCVOK == 0xff)
        {
            update_parameters();
        }
        DelayMs(10);
        if (sign3_flag == 1)
        {
            break;
        }
        else if (left_flag == 1)
        {
            sign2_x += left_right_distance;
            MoveToPosition(sign2_x, sign2_y, 0, 50);
            UploadPosition(2, sign2_x, sign2_y, 0);

            left_flag = 0;
            CFGBUF[0x16 * 2] = 0;
            UploadData(0x16 * 2, 0x00);
        }
        else if (right_flag == 1)
        {
            sign2_x -= left_right_distance;
            MoveToPosition(sign2_x, sign2_y, 0, 50);
            UploadPosition(2, sign2_x, sign2_y, 0);
            right_flag = 0;
            CFGBUF[0x16 * 2] = 0;
            UploadData(0x16 * 2, 0x00);
        }
        else if (up_flag == 1)
        {
            sign2_y -= left_right_distance;
            MoveToPosition(sign2_x, sign2_y, 0, 50);
            UploadPosition(2, sign2_x, sign2_y, 0);
            up_flag = 0;
            CFGBUF[0x16 * 2] = 0;
            UploadData(0x16 * 2, 0x00);
        }
        else if (down_flag == 1)
        {
            sign2_y += left_right_distance;
            MoveToPosition(sign2_x, sign2_y, 0, 50);
            UploadPosition(2, sign2_x, sign2_y, 0);
            down_flag = 0;
            CFGBUF[0x16 * 2] = 0;
            UploadData(0x16 * 2, 0x00);
        }
    }
    put_core();
    MoveToPosition(sign2_x, 0, 0, 100);
    sign2_y2 = sign2_y;
    UploadPosition(2, sign2_x, sign2_y, sign2_y2);
    MoveToPosition(sign2_x - 320, 0, 0, 100);
    MoveToPosition(sign2_x - 320, 0, sign2_y2, 100);
    catch_core();
    MoveToPosition(sign2_x - 320, 0, 0, 100);
    return FINISH;
}
char set_sign3(void)
{
    Uart1_SendString("set_sign3\r\n");
    GoToPage(0x12); // 18
    sign3_x = 785;
    sign3_y = 0;
    sign3_y2 = 0;
    MoveToPosition(sign3_x, sign3_y, sign3_y2, 200);
    UploadPosition(3, sign3_x, sign3_y, sign3_y2);
    sign3_y2 = sign1_y;
    MoveToPosition(sign3_x, sign3_y, sign3_y2, 100);
    UploadPosition(3, sign3_x, sign3_y, sign3_y2);
    while (1)
    {
        if (RCVOK == 0xff)
        {
            update_parameters();
        }
        DelayMs(10);
        if (init_pos_end_flag == 1)
        {
            break;
        }
        else if (left_flag == 1)
        {
            sign3_x += left_right_distance;
            MoveToPosition(sign3_x, sign3_y, sign3_y2, 50);
            UploadPosition(3, sign3_x, sign3_y, sign3_y2);
            left_flag = 0;
            CFGBUF[0x16 * 2] = 0;
            UploadData(0x16 * 2, 0x00);
        }
        else if (right_flag == 1)
        {
            sign3_x -= left_right_distance;
            MoveToPosition(sign3_x, sign3_y, sign3_y2, 50);
            UploadPosition(3, sign3_x, sign3_y, sign3_y2);
            right_flag = 0;
            CFGBUF[0x16 * 2] = 0;
            UploadData(0x16 * 2, 0x00);
        }
        else if (up_flag == 1)
        {
            sign3_y2 -= left_right_distance;
            MoveToPosition(sign3_x, sign3_y, sign3_y2, 50);
            UploadPosition(3, sign3_x, sign3_y, sign3_y2);
            up_flag = 0;
            CFGBUF[0x16 * 2] = 0;
            UploadData(0x16 * 2, 0x00);
        }
        else if (down_flag == 1)
        {
            sign3_y2 += left_right_distance;
            MoveToPosition(sign3_x, sign3_y, sign3_y2, 50);
            UploadPosition(3, sign3_x, sign3_y, sign3_y2);
            down_flag = 0;
            CFGBUF[0x16 * 2] = 0;
            UploadData(0x16 * 2, 0x00);
        }
    }
    put_core();
    return FINISH;
}
/*
**************************************************
            main函数
**************************************************
*/
void main(void)
{
    // 初始化代码
    DelayMs(50);
    Uart1_Init();
    GoToPage(0x3A); // page58
    EN_485 = 0;
    DelayMs(500);
    EN_485 = 1;
    DelayMs(500);
    EN_485 = 0;
    DelayMs(50);
    EN_485 = 1; // 使能485发送
    Interrupt0_Init();

    DelayMs(50);
    Motor_init();

    DelayMs(10);
    GoToPage(0x02);
    DelayMs(10);
    InitData();
    DelayMs(10);
    Uart1_SendString("start!\r\n");
    DelayMs(20);
    GetAllData();
    // EN_485 = 0; // 禁用发送，可接收
    DelayMs(10);
    update_parameters();
    UploadPosition(0, 0, 0, 0);

    CFGBUF[0] = 1;
    CFGBUF[1] = 1;

    /*
    ## 一。右抓取
    1.回到原点
    2.向左移动，向右找端面
    3.移动到铁芯（距离输入）
    4.抓取
    ## 二。中间夹取

    ## 三。 中间放置

    ## 四。左边放置

    */
    while (1)
    {
        DelayMs(10);
        if (go_flag == 1)
        {
            Uart1_SendString("go_test\r\n");
            sign1_x = 200;
            sign1_y = 60;
            sign1_y2 = 0;
            sign2_x = 500;
            sign2_y = 200;
            sign2_y2 = 0;
            sign3_x = 785;
            sign3_y = 0;
            sign3_y2 = 0;
            go_flag = 0;
            // UploadData();
            left_right_speed = 200;
            while (1)
            {
                if (RCVOK == 0xff)
                {
                    update_parameters();
                    RCVOK = 0x00;
                    Uart1_SendString("update_parameters\r\n");
                    DelayMs(10);
                }
                MotorGo(X_MOTOR, GO_LEFT, 400, left_right_speed);
                DelayMs(10);
                MotorGo(X_MOTOR, GO_RIGHT, 400, left_right_speed);
                DelayMs(10);
                // Uart1_SendString("GO TEST!!!\r\n");
                // go_test();
            }
        }
        // Uart1_SendString("go_test\r\n");
        //go_test();
        if (all_data_flag)
        {
            WriteData(0x16, 0x00, 0x00);
            GetAllData();
            update_parameters();
            RCVOK = 0x00;
        }
        else if (RCVOK == 0xff)
        {
            update_parameters();
            RCVOK = 0x00;

            // #ifdef DEBUG
            Uart1_SendString("receive data\r\n");
            DelayMs(10);
            // sprintf(buf, "up_flag = %d \r,down_flag = %d \r",
            //         up_flag, down_flag);
            // Uart1_SendString(buf);
            // #endif
        }

        if (init_pos_flag)
        {
            Uart1_SendString("init pos\r\n");
            DelayMs(10);
            GoToPage(0x0F); // 15
            DelayMs(10);
            if (MotorGo(X_MOTOR, GO_RIGHT, 5000, 150) == ERR)
            {
                Allow_Move = 1;
                UploadPosition(0, 0, 0, 0);
                MoveToPosition(200, 0, 0, 100);
                UploadPosition(1, 200, 0, 0);
                Allow_Move = 0;
            }

            // SetReferencePoint(0, 0, 0, 1, 100); // 增加Y2参数
            // SetReferencePoint(0, 0, 0, 2, 100);
            // SetReferencePoint(0, 0, 0, 3, 100);设定目标点并上传
            UploadData(0x13 * 2, 0x00);
            // 初始化画面
            UploadData(0x16 * 2, 0x00);
            update_parameters();
            DelayMs(10);

            // Uart1_SendString("start init\r\n");
            // Uart1_SendString("start sign1\r\n");
            while (1)
            {
                update_parameters();
                DelayMs(10);
                if (sign1_flag == 1)
                {
                    sign1_flag = 0;
                    break;
                }
            }
            set_sign1();
            DelayMs(10);
            while (1)
            {
                update_parameters();
                DelayMs(10);
                if (sign2_flag == 1)
                {
                    sign2_flag = 0;
                    break;
                }
            }
            set_sign2();
            DelayMs(10);
            while (1)
            {
                update_parameters();
                DelayMs(10);
                if (sign3_flag == 1)
                {
                    sign3_flag = 0;
                    break;
                }
            }
            set_sign3();
            DelayMs(10);
            // while (1)
            // {
            //     if (HandleReferencePointSetting(left_right_distance, 100) == FINISH)
            //     {
            //         break;
            //     }
            // }
            MoveToPosition(sign2_x, 0, 0, 100);
            UploadData(0x16 * 2, 0x00);
            UploadData(0x13 * 2, 0x00);
            update_parameters();
            DelayMs(3);
            Uart1_SendString("init finish\r\n");
            GoToPage(0x02);
        }
        DelayMs(50);
        EN_485 = 0;
    }
}
