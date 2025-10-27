#include "motions.h"


//腿部序号0-7 顺序：小腿大腿   右前、左前、右后、左后


static uint8_t servo_pin[8]={23, 22, 32, 25, 17, 16, 12, 13}; //顺序：小腿大腿   右前、左前、右后、左后

// static float bias[8] = {42,2,-24,2,32,-4,-44,-4};  //初始角度校正
static float bias[8] = {0,0,0,0,0,0,0,0};  //初始角度校正


static servo_t arm[8];

int current_angle[8] = {90,0,90,0,90,0,90,0};

bool servo_add(servo_t *servo, int pin)
{
    static int servo_num = 0;
    if(servo_num>15){
      return false;  //数量超出
    }
    servo->pin = pin;
    servo->channel = servo_num;
    
    ledcSetup(servo_num++, 50, 12);  //设置LEDC通道频率为50，分辨率为12位，即占空比可选0~4095
    ledcAttachPin(pin, servo->channel); //绑定通道输出IO口
    return true;
}

void angle_set(int i, float angle)
{
  if(angle>180) angle = 180;
  if(angle<0) angle = 0;
  ledcWrite(arm[i].channel, 2.275*(angle+bias[i])+102.4);
}

void arm_init()
{
  uint8_t i = 0;
  for(i=0;i<8;i++)
  {
    servo_add(&arm[i],servo_pin[i]);
  }
}
void chang_gravity_little_leg()
{
  for(int i=0;i<4;i+=2)
  {
    ledcWrite(arm[i].channel,2);
  }
  
}
// void chang_gravity_big_leg()
// {
//   ledcWrite(arm[i].channel,2);
// }
void dog_standby()  //站立
{
  float legs[8];
  // ik(0,137,0,140,0,145,0,140,legs);
  ik(64,54,64,54,64,54,64,54,legs);

  for(int i=0;i<8;i++){
    angle_set(i,legs[i]);
    Serial.println(legs[i]);  
  }
  Serial.println("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
}
void niaoniao()
{
  // ledcWrite(16,2.275*0+102.4);
  // angle_set(7,90);
  // delay(100);
  // angle_set(7,160);
  // delay(100);
  // angle_set(7,90);
  // delay(100);
  // angle_set(7,160);
  // delay(100);
  // angle_set(7,90);
  // delay(100);
  for(int a =0;a<4;a++)
  {
    
    for(int i=90;i<170;i+=20)
    {
      ledcWrite(7,2.275*i+102.4);
      delay(100);
    }
    for(int j =170;j>90;j-=20)
    {
      ledcWrite(7,2.275*j+102.4);
      delay(100);
    }

  }
  

}
void servo_to_0()
{
  // ledcWrite(1,2.275*0+102.4);
  // ledcWrite(3,2.275*0+102.4);
  // ledcWrite(5,2.275*0+102.4);
  // ledcWrite(7,2.275*0+102.4);
  // angle_set(1,0);
  // angle_set(3,0);
  // angle_set(5,0);
  // angle_set(7,0);

  angle_set(0,0);
  angle_set(2,0);
  angle_set(4,0);
  angle_set(6,0);

}
void servo_to_90()
{
  // ledcWrite(1,2.275*90+102.4);
  // ledcWrite(3,2.275*90+102.4);
  // ledcWrite(5,2.275*90+102.4);
  // ledcWrite(7,2.275*90+102.4);

  angle_set(1,90);
  angle_set(3,90);
  angle_set(5,90);
  angle_set(7,90);

  angle_set(0,90);
  angle_set(2,90);
  angle_set(4,90);
  angle_set(6,90);

}
void servo_to_120()
{
  // ledcWrite(1,2.275*120+102.4);
  // ledcWrite(3,2.275*120+102.4);
  // ledcWrite(5,2.275*120+102.4);
  // ledcWrite(7,2.275*120+102.4);
  // angle_set(1,110);         //右前
  // angle_set(3,70);          //左前
  // angle_set(5,110);         //右后
  // angle_set(7,70);          //左后

  angle_set(1,30);         //右前
  angle_set(3,150);          //左前
  angle_set(5,30);         //右后
  angle_set(7,150);          //左后


  angle_set(0,150);
  angle_set(2,30);
  angle_set(4,150);
  angle_set(6,30);

}
void add_2_angle(int channel,int servo_num)
{
  current_angle[servo_num] =current_angle[servo_num]+2;
  ledcWrite(channel,2.275*current_angle[servo_num]+102.4);
  Serial.println("增加成功");
  Serial.println(current_angle[servo_num]);
  delay(100);
}
void sub_2_angle(int channel,int servo_num)
{
  current_angle[servo_num] =current_angle[servo_num]-2;
  ledcWrite(channel,2.275*current_angle[servo_num]+102.4);
  Serial.println("减小成功");
  Serial.println(current_angle[servo_num]);
  delay(100);
}
void dog_rest()  //收腿休息
{
  float legs[8];
  for(int j=0;j<30;j++){
    // ik(-5,137-2.5*j,-5,135-2.5*j,-5,148-2.5*j,-5,140-2.5*j,legs);
    ik(-5,140-2.5*j,-5,140-2.5*j,-5,140-2.5*j,-5,140-2.5*j,legs);
    for(int i=0;i<8;i++){
      angle_set(i,legs[i]);
      Serial.println(legs[i]);  
    }
    Serial.println("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
    
    delay(20);
  }
}
void dog_standup()  //起立
{
  float legs[8];
  for(int j=0;j<30;j++){
    ik(-5,60+2.5*j,-5,60+2.5*j,-5,60+2.5*j,-5,60+2.5*j,legs);    //右前、左前、右后、左后
    // ik(-15,65+2.5*j,-15,65+2.5*j,-15,65+2.5*j,-15,65+2.5*j,legs);

    for(int i=0;i<8;i++){
      angle_set(i,legs[i]);
      current_angle[i] = legs[i];  
      Serial.println(legs[i]);
    }
    Serial.println("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
    delay(20);

  }
}

void dog_setdown()  //坐下
{
  float legs[8];
  ik(0,137,0,140,0,75,0,70,legs);
  for(int i=0;i<8;i++){
    angle_set(i,legs[i]);  
  }
}

void dog_step()  //原地踏步
{
  float legs[8];
  static int i = 0, j = 1,l=1,k=10,m;
  if(i == 10){
    j = -1;
  }
  if(i == 0){
    j = 1;
  }
  if(k == 10){
    l = -1;
  }
  if(k == 0){
    l = 1;
  }
  ik(0,i*2+117,0,k*2+120.0,0,k*2+130,0,i*2+125,legs);
  for(m=0;m<8;m++){
    angle_set(m,legs[m]);  
  }
  i += j;
  k += l;
}

void dog_walk(float turn, float direct)  //小跑步态行走,turn 为转向坐标倍率系数,turn<1 右转，turn> 左转,direct为前进后退参数
{
  float legs_xy[8], legs[8];
  static int i = 0, j = 0;
  if(i == 25){
    i = 0;
  }
  trot_cal(i*0.02, 50,-10,15,direct,direct,direct,direct,legs_xy);
  ik(legs_xy[0]*turn-4,legs_xy[1]+122.0,legs_xy[2]/turn-4,legs_xy[3]+122.0,legs_xy[4]/turn-4,legs_xy[5]+132,legs_xy[6]-3,legs_xy[7]*turn+129,legs);
  for(j=0;j<8;j++){
    angle_set(j,legs[j]);
    Serial.println(legs[i]);  
  }
  ++i;
  //delay(10);
}

void dog_back()  //后退
{
  float legs[8];
  static int i = 0, j = 1,l=1,k=10,m;
  if(i == 10){
    j = -1;
  }
  if(i == 0){
    j = 1;
  }
  if(k == 10){
    l = -1;
  }
  if(k == 0){
    l = 1;
  }
  ik(0,i*2+117,0,k*2+120.0,0,k*2+130,0,i*2+125,legs);
  for(m=0;m<8;m++){
    angle_set(m,legs[m]);  
  }
  i += j;
  k += l;
}

void dog_ballance(float roll, float pitch)  //陀螺仪遥控姿态
{
  float legs[8],height_roll,height_pitch;
  static float roll_all,pitch_all;
  static int i = 0;
  if(i<3){
  roll_all+=roll;
  pitch_all+=pitch;
  i++;
  }
  else{
    roll_all = roll_all/3;
    pitch_all = pitch_all/3;
    i = 0;
  }
  
  height_roll = 0.5*tan(roll_all*pi/180.0)*WIDTH;  //横滚角计算左右腿高度差
  height_pitch = 0.5*tan(pitch_all*pi/180.0)*LENGTH;  //俯仰角计算前后腿高度差
  ik(0,137-height_roll+height_pitch,0,140+height_roll+height_pitch,0,145-height_roll-height_pitch,0,140+height_roll-height_pitch,legs);
  for(int i=0;i<8;i++){
    angle_set(i,legs[i]);  
  }


}
