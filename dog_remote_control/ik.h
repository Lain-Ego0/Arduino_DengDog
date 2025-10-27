#ifndef _IK_H_
#define _IK_H_

#include<math.h>

/*函数功能：使用运动学逆解，生成舵机执行角度
# 变量作用：
# x,y为足端坐标值，x为横向坐标,y为竖直坐标
# 内部参数lamda:小腿连杆运动比
# 返回值：ham大腿舵机角度，shank小腿舵机角度
# 顺序：右前、左前、右后、左后,左右角度对称，相差180度
*/
void ik(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4, float *servos);

#endif /* _IK_H_ */
