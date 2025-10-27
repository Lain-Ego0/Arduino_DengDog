#include "ik.h"

#define pi 3.14159265

static float l1 = 80.0;
static float l2 = 69.0;


/*函数功能：使用运动学逆解，生成舵机执行角度
# 变量作用：
# l1为大腿长度
# l2为小腿长度
# x,y为足端坐标值，x为横向坐标,y为竖直坐标
# 内部参数lamda:小腿连杆运动比
# 返回值：ham大腿舵机角度，shank小腿舵机角度
# 顺序：右前、左前、右后、左后,左右角度对称，相差180度
*/
void ik(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4, float *servos)
{
    float lamda = 1.0; //小腿连杆运动比
    float shank1, ham1, fai1,shank2, ham2, fai2,shank3, ham3, fai3,shank4, ham4, fai4;
    x1=-x1;
    shank1=pi-acos((x1*x1+y1*y1-l1*l1-l2*l2)/(-2*l1*l2));
    fai1=acos((l1*l1+x1*x1+y1*y1-l2*l2)/(2*l1*sqrt(x1*x1+y1*y1)));
    if(x1>0){
        ham1=fabsf(atan(y1/x1))-fai1;
    }
    else if(x1<0){
        ham1=pi-fabsf(atan(y1/x1))-fai1;
    }
    else{
        ham1=pi-1.5707-fai1;
    }
    shank1=180-180*shank1/pi*lamda;
    ham1=180*ham1/pi;
    servos[0] = shank1;
    servos[1] = ham1;

    x2=-x2;
    shank2=pi-acos((x2*x2+y2*y2-l1*l1-l2*l2)/(-2*l1*l2));
    fai2=acos((l1*l1+x2*x2+y2*y2-l2*l2)/(2*l1*sqrt(x2*x2+y2*y2)));
    if(x2>0){
        ham2=fabsf(atan(y2/x2))-fai2;
    }
    else if(x2<0){
        ham2=pi-fabsf(atan(y2/x2))-fai2;
    }
    else{
        ham2=pi-1.5707-fai2;
    }
    shank2=180-180*shank2/pi*lamda;
    ham2=180*ham2/pi;
    servos[2] = 180.0-shank2;
    servos[3] = 180.0-ham2;

    x3=-x3;
    shank3=pi-acos((x3*x3+y3*y3-l1*l1-l2*l2)/(-2*l1*l2));
    fai3=acos((l1*l1+x3*x3+y3*y3-l2*l2)/(2*l1*sqrt(x3*x3+y3*y3)));
    if(x3>0){
        ham3=fabsf(atan(y3/x3))-fai3;
    }
    else if(x3<0){
        ham3=pi-fabsf(atan(y3/x3))-fai3;
    }
    else{
        ham3=pi-1.5707-fai3;
    }
    shank3=180-180*shank3/pi*lamda;
    ham3=180*ham3/pi;
    servos[4] = shank3;
    servos[5] = ham3;

    x4=-x4;
    shank4=pi-acos((x4*x4+y4*y4-l1*l1-l2*l2)/(-2*l1*l2));
    fai4=acos((l1*l1+x4*x4+y4*y4-l2*l2)/(2*l1*sqrt(x4*x4+y4*y4)));
    if(x4>0){
        ham4=fabsf(atan(y4/x4))-fai4;
    }
    else if(x4<0){
        ham4=pi-fabsf(atan(y4/x4))-fai4;
    }
    else{
        ham4=pi-1.5707-fai4;
    }
    shank4=180-180*shank4/pi*lamda;
    ham4=180*ham4/pi;
    servos[6] = 180.0-shank4;
    servos[7] = 180.0-ham4;
}
