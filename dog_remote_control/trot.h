#ifndef _TROT_H_
#define _TROT_H_

#include<math.h>

#define pi 3.14159265

/*#函数功能：使用trot步态，生成足端坐标
  #变量作用：
  #t为当前时间
  #xs为起始足端坐标
  #xf为终点足端坐标
  #h为抬腿高度
  #r1,r2,r3,r4取值可以为1或者-1，代表曲线生成的正逆，用于控制转弯
  #返回值：足端坐标数组，顺序：小腿大腿   右前、左前、右后、左后
*/
void trot_cal(float t,float xs,float xf,float h,float r1,float r4,float r2,float r3, float *legs_xy);   //小跑步态生成函数

#endif /* _TROT_H_ */
