#include "car.h"
#include "delay.h"
#include "led.h"
#include "pstwo.h"
#include "servo.h"


int main(void) {
    /*==========关键函数初始化==========*/
    delay_init();
    allMotorPinInit();
    PS2_Init();
    /*==========类和变量初始化==========*/
    MyCar my_car;
    MyLEDBAll my_led_b;
    MyServoAll my_servo_all;
    
    u8 data_left_x, data_left_y, data_right_x, data_right_y, data_key;
    float f_left_x = 0.0;
    float f_left_y = 0.0;
    float f_right_x = 0.0;
    float f_right_y = 0.0;
    /*==========主循环==========*/
    while(1) {
        delay_ms(1);
        if(PS2_RedLight()) {
            /*==========红灯亮起时==========*/
            /*必不可少的延时*/
            delay_ms(60);
            /*读取PS2模拟键值*/
            data_left_x = PS2_AnologData(PSS_LX);
            data_left_y = PS2_AnologData(PSS_LY);
            data_right_x = PS2_AnologData(PSS_RX);
            data_right_y = PS2_AnologData(PSS_RY);
            /*读取PS2按键键值*/
            data_key = PS2_DataKey();
            /*模拟值归一化*/
            f_left_x = (float)(data_left_x - 127) / 128;
            f_left_y = (float)((-1.0)*(data_left_y - 127) / 128); //左手柄Y方向反转
            f_right_x = (float)(data_right_x - 127) / 128;
            f_right_y = (float)((-1.0)*(data_right_y - 127) / 128); //右手柄Y方向反转
            /*控制车*/
            if( (f_left_x * f_left_x + f_left_y * f_left_y) < 0.01 ) { // 左摇杆控制
                if( (f_right_x * f_right_x + f_right_y * f_right_y) < 0.04 ) { // 右摇杆控制
                    my_car.moveStop(); // 科学防抖
                } else {
                    my_car.headBaseTemperMove(0.5, f_right_x, f_right_y);
                }
            } else {
                my_car.headBaseTemperMove(0.8, f_left_x, f_left_y);
            }

            switch(data_key) {
                /*==========温和转圈==========*/
                case PSB_L1: {
                    delay_ms(60);
                    my_car.spinAntiClockwise(0.5);
                    break;
                }
                case PSB_R1: {
                    delay_ms(60);
                    my_car.spinAntiClockwise(-0.5);
                    break;
                }
                /*==========凶狠转圈==========*/
                case PSB_L2: {
                    my_car.spinAntiClockwise(0.8);
                    break;
                }
                case PSB_R2: {
                    my_car.spinAntiClockwise(-0.8);
                    break;
                }                
                /*==========车头切换==========*/
                case PSB_GREEN: {
                    my_car.headChange(0);
                    my_led_b.pinChange(my_car.getHead()+6);
                    break;
                }
                case PSB_RED: {
                    my_car.headChange(1);
                    my_led_b.pinChange(my_car.getHead()+6);
                    break;
                }
                case PSB_PINK: {
                    my_car.headChange(2);
                    my_led_b.pinChange(my_car.getHead()+6);
                    break;
                }
                /*==========掉电停车==========*/
                case PSB_CROSS: {
                    my_car.moveStopSmooth();
                    break;
                }
                /*==========车头冲刺==========*/
                case PSB_PAD_UP: {
                    my_car.headBaseStab(1.0);
                    break;
                }
                /*==========舵机招手==========*/
                case PSB_PAD_LEFT: {
                    my_servo_all.doBeckon(6, 180, 200);
                    break;
                }
            }
            
        } else {
            /*==========红灯未亮起时==========*/
            my_car.moveStop();
        }
    }
}
