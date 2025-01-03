#ifndef _CONFIG_H
#define _CONFIG_H

// 配置相关函数声明
void InitValue(unsigned char address, unsigned int data);
void InitData(void);
void update_parameters(void);

// 配置变量声明 - 需要保持与.c文件中的定义完全一致
extern __xdata int model;
extern __xdata int core_diameter;
extern __xdata int core_distance;
extern __xdata int end_face_distance;
extern __xdata int up_down_speed;
extern __xdata int up_down_distance;
extern __xdata int left_right_speed;
extern __xdata int left_right_distance;

// 标志位声明
extern volatile __bit all_data_flag;
extern volatile __bit left_flag;
extern volatile __bit right_flag;
extern volatile __bit up_flag;
extern volatile __bit down_flag;
extern volatile __bit init_pos_flag;
extern volatile __bit init_pos_end_flag;
extern volatile __bit go_flag;


    /**
     * @brief 上传单个参数到显示屏
     * @param address 参数地址
     * @param data 参数值
     */
    void
    UploadData(unsigned char address, unsigned int data);

#endif