import math

def calculate_angles(x, y, ham=80, shank=70):
    """
    计算给定足端坐标 (x, y) 时的伺服电机角度。
    
    参数:
    x -- 足端的 x 坐标
    y -- 足端的 y 坐标
    ham -- 大腿长度 (默认值为 80)
    shank -- 小腿长度 (默认值为 70)
    
    返回:
    angles -- 包含小腿和大腿角度的列表
    """
    angles = []
    L = math.sqrt(x**2 + y**2)  # 足端到原点的距离
    fai = math.atan2(x, y) * (180 / math.pi)  # 计算与y轴的夹角，使用 atan2 处理所有象限

    # 使用余弦定理计算舵机角度
    cos_sita1 = (L**2 + ham**2 - shank**2) / (2 * L * ham)
    cos_sita2 = (shank**2 + ham**2 - L**2) / (2 * ham * shank)

    # 处理可能的数学错误
    if -1 <= cos_sita1 <= 1 and -1 <= cos_sita2 <= 1:
        sita1 = math.acos(cos_sita1) * (180 / math.pi)  # 大腿舵机角度
        sita2 = 180 - math.acos(cos_sita2) * (180 / math.pi)  # 小腿舵机角度
        
        if fai == sita1:
            angles.append(sita2)  # 小腿角度
            angles.append(90)      # 大腿角度
        else:
            angles.append(sita2)
            angles.append(90 - fai + sita1)
    else:
        print("无法计算角度，输入坐标超出可行范围。")
    
    return angles

if __name__ == "__main__":
    # 输入的足端坐标是机器狗左侧腿的坐标
    x1 = 40  # 足端 x 坐标
    y1 = 70  # 足端 y 坐标
    
    angles = calculate_angles(x1, y1)
    
    print(f"小腿角度: {angles[0]}°")
    print(f"大腿角度: {angles[1]}°")
