#include <ArduinoJson.h>
#include "SSD1306Wire.h"        
#include "myBlueTooth.h"
#include "motions.h"
#include "img.h"


bool flag =true;
static uint8_t run_mode = 0;

char* recBuff; 
StaticJsonDocument<200> doc;  //JSON缓冲段分配
SSD1306Wire display(0x3c, 18, 19);   // ADDRESS, SDA, SCL，iic接口在pca9685中定义

void drawFace()  //绘制表情
{
  display.setColor(WHITE);
  display.fillCircle(24, 20, 15);
  display.fillCircle(104, 20, 15);

}

void animation_wink()  //眨眼动画
{
    static uint8_t i = 0,j = 1,k;
    if(k == 70){
      k = 0;
    }
    display.drawXbm(0, 0, wink1_width, wink1_height, wink[k]);  //图片显示
    k++;
    //delay(10);
}

void animation_closeEye()//闭眼动画
{
    static uint8_t falling[19]={0,0,0,0,1,2,4,5,7,10,7,5,4,2,1,0,0,0,0};
    static uint8_t i = 0,h = 0;
  
    if(h == 19){
      h = 0;
    }
      display.drawXbm(0, falling[h], wink1_width, wink1_height, wink7_bits);  //图片显示
      h++;
      //delay(12);
}

void animation_happy()  //开心动画表情"^^"
{
  static uint8_t falling[19]={0,0,0,0,1,2,4,5,7,10,7,5,4,2,1,0,0,0,0};
  static uint8_t i = 0,h = 0;

  if(h == 19){
    h = 0;
  }
    display.drawXbm(0, falling[h], happy_width, happy_height, happy_bits);  //图片显示
    h++;
    //delay(12);
}

void animation_angry()  //生气动画表情"\ /"
{
    static int i = 0,j = 1;
    if(i == 3){
      j = -1;
    }
    if(i == 0){
      j = 1;
    }
    display.drawXbm(i, i, angry_width, angry_height, angry_bits);  //图片显示
    i += j;
    //delay(8);
}

void img_happyNewYear()  //新年快乐图片
{
    static int i = 128,j = 1;
    for(i = 128; i>0; i--){
        display.clear();
        display.drawXbm(i, 0, m_2023_width, m_2023_height, m_2023_bits);  //图片显示
        display.display();
        delay(1);
    }
    for(i = 128; i>0; i--){
        display.clear();
        display.drawXbm(i-128, 0, m_2023_width, m_2023_height, m_2023_bits);  //图片显示
        display.drawXbm(i, 0, xnkl_width, xnkl_height, xnkl_bits);  //图片显示
        delay(1);
        display.display();
    }
    delay(5000);
}

void animation_smile()  //微笑动画表情
{
    static uint8_t falling[19]={0,0,0,0,1,2,4,5,7,10,7,5,4,2,1,0,0,0,0};
    static uint8_t i = 0,h = 0;

    if(h == 19){
      h = 0;
    }
    display.drawXbm(0, falling[h], smile_left_width, smile_left_height, smile_left_bits);  //图片显示
    display.drawXbm(64, falling[h], smile_right_width, smile_right_height, smile_right_bits);  //图片显示
    h++;
    //delay(12);
}
//----------------------------------------------------
//----------------------舵机标定-----------------------

//-----------------------------------------------------
void setup() {
    Serial.begin(115200);
    BT_init("py-dog");
    
    display.init();
    display.flipScreenVertically();
    display.setFont(ArialMT_Plain_10);
    display.setColor(WHITE);
    arm_init();
}

void loop() {
    display.clear();

    static uint8_t last_cmd = 0;  //保存上个指令
    static uint8_t acc_flag = 0;  //打开陀螺仪标志
    static float pitch,roll;

    static float legs[8],height_roll,height_pitch;
    static int lean_left = 0, lean_right = 0;
    //static float circle[20] = {0.00,8.72,12.00,14.28,16.00,17.32,18.33,19.08,19.60,19.90,20.00,19.90,19.60,19.08,18.33,17.32,16.00,14.28,12.00,8.72,};
    
    recBuff = getDataBT();
    //Serial.println(recBuff);
  
    DeserializationError error = deserializeJson(doc, recBuff); //Json字符解析
    if(!error){
        uint8_t cmd = doc["mode"];  //发送格式举例：{"mode":1}
        //run_mode = cmd;
        if(cmd != last_cmd){
            if((cmd != 6)){
                run_mode = cmd;
            }            
            last_cmd = cmd;
        }
    }

    if(acc_flag == 1){//打开陀螺仪  //接收roll、pitch角度,发送格式{"mode":6,"angpr":[roll,pitch]}//roll为横滚角值，pitch为俯仰角值
        roll = doc["angpr"][0]; 
        pitch = doc["angpr"][1];
    }
    switch(run_mode){
        case 0:
            dog_standby();  //站立
            animation_wink();  //眨眼动画            
            acc_flag = 0;
            run_mode = 7;
            delay(12);
            break;
        case 1:  //前进模式
            dog_walk(1.0, 1.0);  //小跑步态行走  
            animation_wink();  //眨眼动画 
            delay(15 );
            break;
        case 2://踏步模式
            animation_wink();  //眨眼动画 
            dog_step();  //原地踏步
            delay(15);
            break;
        case 3: //起立模式
            dog_standup();  //起立
            run_mode = 7;
            break;
        case 4: //卧倒模式
            dog_rest();  //收腿
            run_mode = 7;
            break;
        case 5:  //手机陀螺仪同步模式
            acc_flag = 1;
            if(roll != 0.0){  //去除空值
                dog_ballance(roll,pitch);
                
                Serial.print("pitch: ");
                Serial.println(pitch);

                Serial.print("roll: ");
                Serial.println(roll);

                display.drawString(2, 2, "roll:    "+String(roll));
                display.drawString(2, 13, "pitch   :"+String(pitch));

                display.drawString(2, 24, "roll_height:    "+String(height_roll));
                display.drawString(2, 35, "pitch_height   :"+String(height_pitch));
                
            }
            break;
        case 7:  //空模式
            animation_closeEye(); //闭眼动画           
            acc_flag = 0;
            flag = true;
            delay(23);
            break;
        case 8:  //左转模式
            dog_walk(0.5, 1.0);  //小跑步态行走  
            animation_wink();  //眨眼动画  
            delay(15);
            break;
        case 9:  //右转模式
            dog_walk(2.0, 1.0);  //小跑步态行走  
            animation_wink();  //眨眼动画  
            delay(15);
            break;
        case 10:  //后退模式
            dog_back();  //后退 
            animation_wink();  //眨眼动画  
            delay(18);
            break;
        case 11:  //坐下
            dog_setdown();
            img_happyNewYear();  //新年快乐图片
            delay(3000);
            run_mode = 7;
            break;
        case 12:  //左偏
            drawFace();  //绘制表情
            lean_left += 5;
            ik(0,137,0,140-lean_left,0,145,0,140-lean_left,legs);
            for(int i=0;i<8;i++){
              angle_set(i,legs[i]);  
            }
            run_mode = 7;
            break;
        case 13:  //右偏
            drawFace();  //绘制表情
            lean_right += 5;
            ik(0,137-lean_right,0,140,0,145-lean_right,0,140,legs);
            for(int i=0;i<8;i++){
              angle_set(i,legs[i]);  
            }
            run_mode = 7;
            break;
        case 14:  

            dog_setdown();
            img_happyNewYear();  //新年快乐图片
            run_mode = 0;
            break;
        case 15:  //清除倾斜
            lean_right = 0;
            lean_left = 0;
            break;
        case 16:
            animation_smile();
            delay(15);
            break;
        // case 17:
        //     while(true)
        //     {
        //       switch(run_mode)
        //       {
        //         case 1:  //左前大腿
        //           break;
        //         case 2:  //右前大腿
        //           break;
        //         case 3:  //左后大腿
        //           break;
        //         case 4:  //右后大腿
        //           break;
        //         case 5:  //左前小腿
        //           break;
        //         case 6:  //右前小腿
        //           break;
        //         case 7:  //左后小腿
        //           break;
        //         case 8:  //右后小腿
        //           break;
        //       }
        //     }
          case 18:
            if(last_cmd == 4||last_cmd == 11)
            {
              dog_standup();
              delay(1000);
              niaoniao();
              break;
            }
            else
            {
              niaoniao();
              break;
            }
          case 19:
            servo_to_0();
            delay(100);
            break;
          case 20:
            servo_to_90();
            delay(100);
            break;
          case 21:
            servo_to_120();
            delay(100);
            break;
          case 22:
            niaoniao();
            delay(100);
            break;
          case 23:
            Serial.println("开始标定");
            //flag = true;
            while(flag)
            { 
                recBuff = getDataBT();
                //Serial.println(recBuff);
                DeserializationError error = deserializeJson(doc, recBuff); //Json字符解析
                if(!error){
                    uint8_t cmd = doc["mode"];  //发送格式举例：{"mode":1}
                    //run_mode = cmd;
                    if(cmd != last_cmd){
                        if((cmd != 6)){
                            run_mode = cmd;
                        }            
                        last_cmd = cmd;
                    }
                }
              switch(run_mode)
                {
                  case 1: //右前大 +
                    add_2_angle(1,1);
                    run_mode = 23;
                    break;
                  case 2:
                    sub_2_angle(1,1); //右前大 -
                    run_mode = 23;
                    break;
                  case 3:
                    add_2_angle(3,3);//左前 +
                    run_mode = 23;
                    break;
                  case 4:
                    sub_2_angle(3,3);//左前 -
                    run_mode = 23;
                    break;
                  case 5:
                    add_2_angle(5,5);//右后 +
                    run_mode = 23;
                    break;
                  case 6:
                    sub_2_angle(5,5);//右后 -
                    run_mode = 23;
                    break;
                  case 7:
                    add_2_angle(7,7);//左后 +
                    run_mode = 23;
                    break;
                  case 8:
                    sub_2_angle(7,7);//左后 -
                    run_mode = 23;
                    break;
                  case 9:
                    add_2_angle(0,0);
                    run_mode = 23;
                    break;
                  case 10:
                    sub_2_angle(2,2);
                    run_mode = 23;
                    break;
                  case 11:
                    add_2_angle(4,4);
                    run_mode = 23;
                    break;
                  case 12:
                    sub_2_angle(6,6);
                    run_mode = 23;
                    break;
                  case 17:
                    flag = false;
                    //run_mode = 7;
                    break;
                }
            }
            break;           
        default:
            break;
        }

    //Serial.println(run_mode);
    display.display();
    //delay(100);
}
