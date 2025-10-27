#include "trot.h"

static float Ts = 0.5;
static float faai = 0.5;

/*#函数功能：使用trot步态，生成足端坐标
  #变量作用：
  #t为当前时间
  #xs为起始足端坐标
  #xf为终点足端坐标
  #h为抬腿高度
  #r1,r2,r3,r4取值可以为1或者-1，代表曲线生成的正逆，用于控制转弯
  #返回值：足端坐标数组，顺序：小腿大腿   右前、左前、右后、左后
*/
void trot_cal(float t,float xs,float xf,float h,float r1,float r4,float r2,float r3, float *legs_xy)   //小跑步态生成函数
{
    float sigma,zep,xep_z,xep_b;
    if(t<=Ts*faai)
    {
        sigma=2*pi*t/(faai*Ts);
        zep=h*(1-cos(sigma))/2;
        xep_z=(xf-xs)*((sigma-sin(sigma))/(2*pi))+xs;
        xep_b=(xs-xf)*((sigma-sin(sigma))/(2*pi))+xf;
        //输出y
        legs_xy[1]=zep;
        legs_xy[3]=0;
        legs_xy[7]=zep;
        legs_xy[5]=0;
        //输出x
        legs_xy[0]=-xep_b*r1;
        legs_xy[2]=-xep_z*r2;
        legs_xy[6]=-xep_b*r3;
        legs_xy[4]=-xep_z*r4;
    }
    else if((t>Ts*faai)&&(t<=Ts)){
        sigma=2*pi*(t-Ts*faai)/(faai*Ts);
        zep=h*(1-cos(sigma))/2;
        xep_z=(xf-xs)*((sigma-sin(sigma))/(2*pi))+xs;
        xep_b=(xs-xf)*((sigma-sin(sigma))/(2*pi))+xf;
        //输出y
        legs_xy[1]=0;
        legs_xy[3]=zep;
        legs_xy[7]=0;
        legs_xy[5]=zep;
        //输出x
        legs_xy[0]=-xep_z*r1;
        legs_xy[2]=-xep_b*r2;
        legs_xy[6]=-xep_z*r3;
        legs_xy[4]=-xep_b*r4;
    }
}
