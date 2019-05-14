#include "delay.h"
#include "servo.h"


/*======================PA6 & PA7 & PB0======================*/
MyServoAll::MyServoAll() {
    /*用的TIM3的PWM输出*/

    /*结构体和时钟初始化*/
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);    //使能PA,PD端口时钟
    
    /*PWM的GPIOA初始化*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /*PWM的GPIOB初始化*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    /*TIM3时钟初始化*/
    TIM_TimeBaseStructure.TIM_Period = 1999;  //Freq=72000÷(arr+1) 【单位：kHz】
    TIM_TimeBaseStructure.TIM_Prescaler = 719; //不分频
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
    
    /*PWM相关设置*/
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
    
    /*通道1、2、3初始化*/
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //CH1预装载使能

    TIM_OC2Init(TIM3, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

    TIM_OC3Init(TIM3, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

    TIM_CtrlPWMOutputs(TIM3, ENABLE);    //MOE 主输出使能

    TIM_ARRPreloadConfig(TIM3, ENABLE); //使能TIMx在ARR上的预装载寄存器

    TIM_Cmd(TIM3, ENABLE);  //使能TIM3

    /*初始化舵机角度*/
    initAngle(6);
}
void MyServoAll::setDuty(int i_pin_arg, float f_duty_arg) {
    /*越界处理*/
    if(f_duty_arg > 1.0) f_duty_arg = 1.0;
    if(f_duty_arg < 0.0) f_duty_arg = 0.0;
    /*后续处理*/
    int i_ccr_temp = 1999 * (1 - f_duty_arg);
    switch(i_pin_arg) {
        case 6: {// PA6对应TIM3的CH1
            TIM_SetCompare1(TIM3, i_ccr_temp);
            break;
        }
        case 7: {// PA7对应TIM3的CH2
            TIM_SetCompare2(TIM3, i_ccr_temp);
            break;
        }
        case 0: {// PB0对应TIM3的CH3
            TIM_SetCompare3(TIM3, i_ccr_temp);
            break;
        }
        default: {}
    }
}
void MyServoAll::setAngle(int i_pin_arg, int i_angle_arg) {
    /*越界处理*/
    if(i_angle_arg > 180) i_angle_arg = 180;
    if(i_angle_arg < 0) i_angle_arg = 0;
    /*做做数学*/
    setDuty(i_pin_arg, (0.5 + (i_angle_arg / 90)) / 20);
}
void MyServoAll::initAngle(int i_pin_arg) {
    setAngle(i_pin_arg, 0);
}
void MyServoAll::doBeckon(int i_pin_arg, int i_angle_arg, int i_delay_arg) {
    setAngle(i_pin_arg, i_angle_arg);
    delay_ms(i_delay_arg);
    setAngle(i_pin_arg, 0);
}


