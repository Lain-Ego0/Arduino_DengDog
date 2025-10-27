#ifndef _MOTIONS_H_
#define _MOTIONS_H_

#include<math.h>
#include<Arduino.h>
#include "ik.h"
#include "trot.h"

#define WIDTH 60
#define LENGTH 70 //重心到腿的长宽

typedef struct{
  uint8_t pin;
  uint8_t channel;
}servo_t;

void arm_init();  //舵机初始化
void dog_standby();  //站立
void dog_rest();  //收腿
//////////////xy////////////
void niaoniao();
void servo_to_0();
void servo_to_90();
void servo_to_120();
void angles_set(uint8_t channel,uint8_t angle);
void add_2_angle(int channel,int servo_num);
void sub_2_angle(int channel,int servo_num);

/////////////xy/////////////
void dog_standup();  //起立
void dog_setdown();  //坐下
void dog_step();  //原地踏步
void dog_walk(float turn, float direct);  //小跑步态行走,turn 为转向坐标倍率系数,turn<1 右转，turn> 左转,direct为前进后退参数
void dog_back();  //后退
void dog_ballance(float roll, float pitch);  //陀螺仪遥控姿态
void angle_set(int i, float angle);  

#endif /* _MOTIONS_H_ */
