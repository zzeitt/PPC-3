#include "car.h"
#include "delay.h"
#include "led.h"
#include "pstwo.h"


int main(void) {
    /*==========�ؼ�������ʼ��==========*/
    delay_init();
    allMotorPinInit();
    PS2_Init();
    /*==========��ͱ�����ʼ��==========*/
    MyCar my_car;
    MyLEDBAll my_led_b;
    u8 data_left_x, data_left_y, data_key;
    float f_left_x = 0.0;
    float f_left_y = 0.0;
    /*==========��ѭ��==========*/
    while(1) {
        delay_ms(1);
        if(PS2_RedLight()) {
            /*==========�������ʱ==========*/
            /*�ز����ٵ���ʱ*/
            delay_ms(60);
            /*��ȡPS2ģ���ֵ*/
            data_left_x = PS2_AnologData(PSS_LX);
            data_left_y = PS2_AnologData(PSS_LY);
            /*��ȡPS2������ֵ*/
            data_key = PS2_DataKey();
            /*ģ��ֵ��һ��*/
            f_left_x = (float)(data_left_x - 127) / 128;
            f_left_y = (float)((-1.0)*(data_left_y - 127) / 128); //�ֱ�Y����ת
            /*���Ƴ�*/
            if( (f_left_x * f_left_x + f_left_y * f_left_y) < 0.01 ) {
                my_car.moveStop(); // ��ѧ����
            } else {
                my_car.headBaseMove(f_left_x, f_left_y);
            }
            switch(data_key) {
                /*==========�º�תȦ==========*/
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
                /*==========�׺�תȦ==========*/
                case PSB_L2: {
                    my_car.spinAntiClockwise(0.8);
                    break;
                }
                case PSB_R2: {
                    my_car.spinAntiClockwise(-0.8);
                    break;
                }                
                /*==========��ͷ�л�==========*/
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
                /*==========����ͣ��==========*/
                case PSB_CROSS: {
                    my_car.moveStopSmooth();
                    break;
                }
                /*==========��ͷ���==========*/
                case PSB_PAD_UP: {
                    my_car.headBaseStab(1.0);
                    break;
                }
            }
            
        } else {
            /*==========���δ����ʱ==========*/
            my_car.moveStop();
        }
    }
}
