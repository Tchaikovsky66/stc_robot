import numpy as np
import matplotlib.pyplot as plt

def calculate_s_curve(max_speed=500, min_speed=10, jerk=1600, max_acc=400):
    # 限制最大速度
    if max_speed > 800:
        max_speed = 800
    if max_speed < 10:
        max_speed = 10
        
    # 调整加速度和加加速度的计算
    #max_acc = 0.3 * max_speed + 200  # 这样当max_speed=800时，max_acc=600
    #jerk = 0.1 * max_acc + 500       # 这样当max_acc=600时，jerk=680
    max_acc = max_speed
    jerk = 600
    # 确保jerk不超过1600
    if jerk > 1600:
        jerk = 1600
    
    # 计算时间参数
    T1 = max_acc / jerk   # 加加速段时间
    v1 = min_speed + 0.5 * jerk * T1 * T1  # 第一段末速度
    T2 = (max_speed - (min_speed + jerk * T1 * T1)) / max_acc  # 匀加速段时间
    T3 = T1  # 减加加速段时间
    
    # 创建时间数组 
    t1 = np.linspace(0, T1, 100)  # 加加速段
    t2 = np.linspace(T1, T1+T2, 100)  # 匀加速段
    t3 = np.linspace(T1+T2, T1+T2+T3, 100)  # 减加加速段
    
    # 计算速度
    v1_array = min_speed + 0.5 * jerk * t1 * t1  # 加加速段速度
    v2_array = v1 + max_acc * (t2 - T1)  # 匀加速段速度
    v3_array = v2_array[-1] + max_acc * (t3 - (T1+T2)) - 0.5 * jerk * (t3 - (T1+T2)) * (t3 - (T1+T2))  # 减加加速段速度
    
    # 计算加速度
    a1_array = jerk * t1  # 加加速段加速度
    a2_array = np.full_like(t2, max_acc)  # 匀加速段加速度
    a3_array = max_acc - jerk * (t3 - (T1+T2))  # 减加加速段加速度
    
    # 计算加加速度
    j1_array = np.full_like(t1, jerk)  # 加加速段加加速度
    j2_array = np.zeros_like(t2)  # 匀加速段加加速度
    j3_array = np.full_like(t3, -jerk)  # 减加加速段加加速度
    
    # 合并时间和数据数组
    t = np.concatenate((t1, t2, t3))
    v = np.concatenate((v1_array, v2_array, v3_array))
    a = np.concatenate((a1_array, a2_array, a3_array))
    j = np.concatenate((j1_array, j2_array, j3_array))
    
    return t, v, a, j

# 计算曲线
t, v, a, j = calculate_s_curve()

# 创建图表
fig, (ax1, ax2, ax3) = plt.subplots(3, 1, figsize=(10, 12))

# 绘制速度曲线
ax1.plot(t, v, 'b-', label='Speed')
ax1.set_ylabel('Speed (mm/s)')
ax1.set_title('S-Curve Speed Profile')
ax1.grid(True)
ax1.legend()

# 绘制加速度曲线
ax2.plot(t, a, 'r-', label='Acceleration')
ax2.set_ylabel('Acceleration (mm/s²)')
ax2.grid(True)
ax2.legend()

# 绘制加加速度曲线
ax3.plot(t, j, 'g-', label='Jerk')
ax3.set_xlabel('Time (s)')
ax3.set_ylabel('Jerk (mm/s³)')
ax3.grid(True)
ax3.legend()

plt.tight_layout()
plt.show()

# 打印关键时间点和速度值
print(f"Total time: {t[-1]:.3f}s")
print(f"Max speed: {max(v):.1f}mm/s")
print(f"Max acceleration: {max(a):.1f}mm/s²")
print(f"Jerk: {max(j):.1f}mm/s³")