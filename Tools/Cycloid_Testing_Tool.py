import math

# 初始化腿部坐标
legs_xy = [0, 0, 0, 0, 0, 0, 0, 0]

# 大腿和小腿的长度
l1 = 80
l2 = 70

def ik(x1, y1, x2, y2, x3, y3, x4, y4, servos):
    lamda = 1.0  # 小腿连杆运动比

    # 计算每条腿的伺服电机角度
    for i, (x, y) in enumerate([(x1, y1), (x2, y2), (x3, y3), (x4, y4)]):
        x = -x  # 坐标取反
        shank = math.pi - math.acos((x ** 2 + y ** 2 - l1 ** 2 - l2 ** 2) / (-2 * l1 * l2))
        fai = math.acos((l1 ** 2 + x ** 2 + y ** 2 - l2 ** 2) / (2 * l1 * math.sqrt(x ** 2 + y ** 2)))

        if x > 0:
            ham = abs(math.atan(y / x)) - fai
        elif x < 0:
            ham = math.pi - abs(math.atan(y / x)) - fai
        else:
            ham = math.pi - 1.5707 - fai

        shank = 180 - 180 * shank / math.pi * lamda
        ham = 180 * ham / math.pi
        
        # 存储伺服电机角度
        servos[i * 2] = shank if i % 2 == 0 else 180.0 - shank
        servos[i * 2 + 1] = ham if i % 2 == 0 else 180.0 - ham

def trot_cal(t, xs, xf, h, r1, r4, r2, r3, legs_xy):
    faai = 0.5  # 假设这是预定义的常量
    Ts = 0.5    # 同样假设这是预定义的常量

    if t <= Ts * faai:
        sigma = 2 * math.pi * t / (faai * Ts)
        zep = h * (1 - math.cos(sigma)) / 2
        xep_z = (xf - xs) * ((sigma - math.sin(sigma)) / (2 * math.pi)) + xs
        xep_b = (xs - xf) * ((sigma - math.sin(sigma)) / (2 * math.pi)) + xf

        # 更新腿部坐标
        legs_xy[0] = -xep_b * r1
        legs_xy[1] = zep
        legs_xy[2] = -xep_z * r2
        legs_xy[3] = 0
        legs_xy[4] = -xep_z * r4
        legs_xy[5] = 0
        legs_xy[6] = -xep_b * r3
        legs_xy[7] = zep

    elif (t > Ts * faai) and (t <= Ts):
        sigma = 2 * math.pi * (t - Ts * faai) / (faai * Ts)
        zep = h * (1 - math.cos(sigma)) / 2
        xep_z = (xf - xs) * ((sigma - math.sin(sigma)) / (2 * math.pi)) + xs
        xep_b = (xs - xf) * ((sigma - math.sin(sigma)) / (2 * math.pi)) + xf

        # 更新腿部坐标
        legs_xy[0] = -xep_z * r1
        legs_xy[1] = 0
        legs_xy[2] = -xep_b * r2
        legs_xy[3] = zep
        legs_xy[4] = -xep_b * r4
        legs_xy[5] = zep
        legs_xy[6] = -xep_z * r3
        legs_xy[7] = 0

if __name__ == "__main__":
    turn = 1.0
    dir = -1
    legs = [0, 0, 0, 0, 0, 0, 0, 0]

    for i in range(25):
        print("第", i, "次迭代")
        
        # 更新腿部位置
        trot_cal(i * 0.02, 6, -24, 25, dir, dir, dir, dir, legs_xy)
        print("腿部坐标:", legs_xy)
        
        # 更新伺服电机角度
        ik(legs_xy[0] * turn, 93.6 - legs_xy[1], 
           legs_xy[2] / turn, 93.6 - legs_xy[3], 
           legs_xy[4] / turn, 93.6 - legs_xy[5], 
           legs_xy[6] * turn, 93.6 - legs_xy[7], 
           legs)
        
        print("伺服电机角度:", legs)
