import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import CubicSpline

def calculate_s_curve(max_speed=100, min_speed=5, step_distance=0.05):
    max_acc = 200
    jerk = 800
    
    # 计算时间参数
    T1 = max_acc / jerk  # 加加速时间
    T3 = T1              # 减加速时间
    
    # 计算第一段末速度和加速度
    v1 = min_speed + 0.5 * jerk * T1 * T1  # 第一段末速度
    a1 = jerk * T1                         # 第一段末加速度
    
    # 计算第二段时间，确保加速度不超过max_acc
    T2 = max(0, (max_speed - (min_speed + jerk * T1 * T1)) / a1)    # 匀加速段时间
    
    # 计算每段需要的点数
    total_points = 300
    s1_points = int(total_points * T1 / (T1 + T2 + T3))
    s3_points = s1_points
    s2_points = total_points - s1_points - s3_points
    
    # 创建时间数组
    t1 = np.linspace(0, T1, s1_points)
    t2 = np.linspace(T1, T1+T2, s2_points)
    t3 = np.linspace(T1+T2, T1+T2+T3, s3_points)
    
    # 计算第一段（加加速段）
    v1_array = min_speed + 0.5 * jerk * t1 * t1
    a1_array = jerk * t1
    
    # 计算第二段（匀加速段）
    v2_array = v1_array[-1] + a1 * (t2 - T1)
    a2_array = np.full_like(t2, a1)
    
    # 计算第三段（减加速段）
    t3_rel = t3 - (T1+T2)
    v3_start = v2_array[-1]
    a3_array = a1 - jerk * t3_rel
    v3_array = v3_start + a1 * t3_rel - 0.5 * jerk * t3_rel * t3_rel
    
    # 合并数组
    t = np.concatenate((t1, t2, t3))
    v = np.concatenate((v1_array, v2_array, v3_array))
    acc = np.concatenate((a1_array, a2_array, a3_array))
    
    # 计算加加速度
    dt = t[1] - t[0]
    jerk_array = np.gradient(acc, dt)
    
    # 计算总距离
    s = np.cumsum(v) * dt
    total_distance = s[-1]
    
    # 创建均匀的距离数组
    s_uniform = np.arange(0, total_distance, step_distance)
    
    # 对速度进行插值，得到均匀距离下的速度
    v_uniform = np.interp(s_uniform, s, v)
    
    # 计算实际的延时值
    delay_array = 230400 / v_uniform  # 转换为定时器计数值
    
    # 保存为C语言数组格式
    with open('speed_table.h', 'w') as f:
        f.write("// 自动生成的S曲线速度表\n")
        f.write(f"// 最大速度: {max_speed}mm/s\n")
        f.write(f"// 最小速度: {min_speed}mm/s\n")
        f.write(f"// 加速度: {max_acc}mm/s²\n")
        f.write(f"// 加加速度: {jerk}mm/s³\n")
        f.write(f"// 总步数: {len(delay_array)}\n\n")
        
        # 定义数组大小
        f.write(f"#define SPEED_TABLE_SIZE {len(delay_array)}\n\n")
        
        # 延时值数组（用于定时器）
        f.write("const unsigned int delay_table[SPEED_TABLE_SIZE] = {\n    ")
        for i, delay in enumerate(delay_array):
            if i > 0 and i % 10 == 0:  # 每10个数换一行
                f.write("\n    ")
            f.write(f"{int(delay)}, ")
        f.write("\n};\n\n")
        
        # 速度值数组（可选）
        f.write("const unsigned int speed_table[SPEED_TABLE_SIZE] = {\n    ")
        for i, speed in enumerate(v_uniform):
            if i > 0 and i % 10 == 0:
                f.write("\n    ")
            f.write(f"{int(speed)}, ")
        f.write("\n};\n")
    
    # 修改图表布局为2x3
    fig = plt.figure(figsize=(15, 12))
    
    # 绘制速度-时间曲线 (左上)
    ax1 = plt.subplot(231)
    ax1.plot(t, v, 'b-', label='Speed')
    ax1.set_ylabel('Speed (mm/s)')
    ax1.set_xlabel('Time (s)')
    ax1.set_title('Speed-Time Profile')
    ax1.grid(True)
    ax1.legend()
    
    # 绘制加速度-时间曲线 (中上)
    ax2 = plt.subplot(232)
    ax2.plot(t, acc, 'r-', label='Acceleration')
    ax2.set_xlabel('Time (s)')
    ax2.set_ylabel('Acceleration (mm/s²)')
    ax2.set_title('Acceleration-Time Profile')
    ax2.grid(True)
    ax2.legend()
    
    # 绘制加加速度-时间曲线 (右上)
    ax3 = plt.subplot(233)
    ax3.plot(t, jerk_array, 'g-', label='Jerk')
    ax3.set_xlabel('Time (s)')
    ax3.set_ylabel('Jerk (mm/s³)')
    ax3.set_title('Jerk-Time Profile')
    ax3.grid(True)
    ax3.legend()
    
    # 绘制延时-距离关系曲线 (左下)
    ax4 = plt.subplot(234)
    ax4.plot(s_uniform, delay_array, 'm-', label='Step Delay')
    ax4.set_xlabel('Distance (mm)')
    ax4.set_ylabel('Delay (us)')
    ax4.set_title('Step Delay-Distance Profile')
    ax4.grid(True)
    ax4.legend()
    
    # 绘制速度-距离关系曲线 (中下)
    ax5 = plt.subplot(235)
    ax5.plot(s, v, 'c-', label='Speed')
    ax5.set_xlabel('Distance (mm)')
    ax5.set_ylabel('Speed (mm/s)')
    ax5.set_title('Speed-Distance Profile')
    ax5.grid(True)
    ax5.legend()
    
    # 绘制加速度-距离关系曲线 (右下)
    ax6 = plt.subplot(236)
    ax6.plot(s, acc, 'y-', label='Acceleration')
    ax6.set_xlabel('Distance (mm)')
    ax6.set_ylabel('Acceleration (mm/s²)')
    ax6.set_title('Acceleration-Distance Profile')
    ax6.grid(True)
    ax6.legend()
    
    plt.tight_layout()
    plt.show()
    
    # 打印关键信息
    print(f"\nSummary:")
    print(f"Total time: {t[-1]:.3f}s")
    print(f"Total distance: {total_distance:.1f}mm")
    print(f"Max speed: {max(v):.1f}mm/s")
    print(f"Max acceleration: {max(abs(acc)):.1f}mm/s²")
    print(f"Max jerk: {max(abs(jerk_array)):.1f}mm/s³")
    print(f"Min delay: {min(delay_array):.1f}us")
    print(f"Max delay: {max(delay_array):.1f}us")
    print(f"Total steps: {len(s_uniform)}")
    
    # 生成speed_profile.h文件
    num_samples = 100
    sample_indices = np.linspace(0, len(v_uniform)-1, num_samples, dtype=int)
    sampled_speeds = v_uniform[sample_indices]
    
    with open('speed_profile.h', 'w') as f:
        f.write("// 自动生成的速度-距离采样数据\n")
        f.write(f"// 最大速度: {max_speed}mm/s\n")
        f.write(f"// 最小速度: {min_speed}mm/s\n")
        f.write(f"// 采样点数: {num_samples}\n\n")
        
        f.write(f"#define SPEED_PROFILE_SIZE {num_samples}\n\n")
        
        # 原始速度值数组（浮点数）
        f.write("// 原始速度值（浮点数）\n")
        f.write("const float speed_profile_float[SPEED_PROFILE_SIZE] = {\n    ")
        for i, speed in enumerate(sampled_speeds):
            if i > 0 and i % 5 == 0:  # 每5个数换一行
                f.write("\n    ")
            f.write(f"{speed:.2f}f, ")
        f.write("\n};\n\n")
        
        # 四舍五入后的速度值数组（整数）
        f.write("// 四舍五入后的速度值（整数）\n")
        f.write("const unsigned int speed_profile_int[SPEED_PROFILE_SIZE] = {\n    ")
        rounded_speeds = np.round(sampled_speeds).astype(int)  # 四舍五入并转换为整数
        for i, speed in enumerate(rounded_speeds):
            if i > 0 and i % 5 == 0:  # 每5个数换一行
                f.write("\n    ")
            f.write(f"{speed}, ")
        f.write("\n};\n")
    
    print(f"\n已生成speed_profile.h文件，包含{num_samples}个速度采样点")
    print("包括原始浮点数值和四舍五入后的整数值")
    
    return t, v, s_uniform, delay_array

# 计算曲线
t, v, s, delays = calculate_s_curve()

# 创建2x2布局的图表
fig = plt.figure(figsize=(15, 12))

# 绘制速度-时间曲线 (左上)
ax1 = plt.subplot(221)
ax1.plot(t, v, 'b-', label='Speed')
ax1.set_ylabel('Speed (mm/s)')
ax1.set_xlabel('Time (s)')
ax1.set_title('Speed-Time Profile')
ax1.grid(True)
ax1.legend()

# 绘制速度-距离关系曲线 (右上)
ax2 = plt.subplot(222)
ax2.plot(s, v, 'm-', label='Speed vs Distance')
ax2.set_xlabel('Distance (mm)')
ax2.set_ylabel('Speed (mm/s)')
ax2.set_title('Speed-Distance Profile')
ax2.grid(True)
ax2.legend()

# 绘制延时-距离关系曲线 (左下)
ax3 = plt.subplot(223)
ax3.plot(s, delays, 'g-', label='Step Delay')
ax3.set_xlabel('Distance (mm)')
ax3.set_ylabel('Delay (us)')
ax3.set_title('Step Delay Profile')
ax3.grid(True)
ax3.legend()

plt.tight_layout()
plt.show()

# 打印关键信息
print(f"\nSummary:")
print(f"Total time: {t[-1]:.3f}s")
print(f"Total distance: {s[-1]:.1f}mm")
print(f"Max speed: {max(v):.1f}mm/s")
print(f"Min delay: {min(delays):.1f}us")
print(f"Max delay: {max(delays):.1f}us")
print(f"Total steps: {len(s)}")
