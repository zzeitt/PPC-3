#include "delay.h"
#include "led.h"
#include "pwm.h"

/*======================PC13======================*/
MyLEDC::MyLEDC() {
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

    GPIO_Init(GPIOC, &GPIO_InitStructure);
}
void MyLEDC::flash(int time_flash) {
    delay_init() ;
    while (1) {
        PCout(13)=1;
        delay_ms(time_flash);
        PCout(13)=0;
        delay_ms(time_flash);
    }
}

/*======================PB6 & PB7 & PB8======================*/
MyLEDBAll::MyLEDBAll()
:i_pin(6)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB, GPIO_Pin_6);
}
void MyLEDBAll::pinChange(int i_pin_arg) {
    i_pin = i_pin_arg;
    GPIO_ResetBits(GPIOB, GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8);
    switch(i_pin) {
        case 6: {
            GPIO_SetBits(GPIOB, GPIO_Pin_6);
            break;
        }
        case 7: {
            GPIO_SetBits(GPIOB, GPIO_Pin_7);
            break;
        }        
        case 8: {
            GPIO_SetBits(GPIOB, GPIO_Pin_8);
            break;
        }
        default: {}
    }
}



