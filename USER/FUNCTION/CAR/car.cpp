#include "car.h"
#include "sys.h"
#include "delay.h"


/*=====================六个管脚初始化=====================*/
void allMotorPinInit() {
    /*结构体和时钟初始化*/
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);    //使能PA,PD端口时钟
    
    /*普通GPIO初始化*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        //IO口速度为50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA, GPIO_InitStructure.GPIO_Pin);  //初始置高
    
    /*PWM的GPIO初始化*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /*TIM时钟初始化*/
    TIM_TimeBaseStructure.TIM_Period = 899;  //Freq=72000÷(arr+1) 【单位：kHz】
    TIM_TimeBaseStructure.TIM_Prescaler = 0; //不分频
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
    
    /*PWM相关设置*/
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
    
    /*通道1、2、3初始化*/
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH1预装载使能

    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);

    TIM_OC3Init(TIM1, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);

    TIM_CtrlPWMOutputs(TIM1,ENABLE);    //MOE 主输出使能

    TIM_ARRPreloadConfig(TIM1, ENABLE); //使能TIMx在ARR上的预装载寄存器

    TIM_Cmd(TIM1, ENABLE);  //使能TIM1
}

/*=====================普通受口类=====================*/
MyPinNormal::MyPinNormal(int i_pin_arg) {
    /*私有变量赋值*/
    i_pin = i_pin_arg;
    /*初始置高*/
    b_high = true;
    /*gpio名称赋值*/
    switch(i_pin) {
        case 0: {
            g_pin = GPIO_Pin_0;
            break;
        }
        case 1: {
            g_pin = GPIO_Pin_1;
            break;
        }
        case 2: {
            g_pin = GPIO_Pin_2;
            break;
        }
        default: {}
    }
}
void MyPinNormal::setHigh(bool b_high_arg) {
    b_high = b_high_arg;
    if(b_high) {
        GPIO_SetBits(GPIOA, g_pin);
    } else {
        GPIO_ResetBits(GPIOA, g_pin);
    }
}

/*=====================PWM波受口类=====================*/
MyPinPWM::MyPinPWM(int i_pin_arg) {
    /*私有变量赋值*/
    i_pin = i_pin_arg;
    f_duty = 0.0;
}
void MyPinPWM::setDuty(float f_duty_arg) {
    /*越界处理*/
    if(f_duty_arg > 1.0) f_duty_arg = 1.0;
    if(f_duty_arg < 0.0) f_duty_arg = 0.0;
    /*后续处理*/
    f_duty = f_duty_arg;
    int i_ccr_temp = 899 * (1 - f_duty);
    switch(i_pin) {
        case 8: {// PA8对应TIM1的通道1
            TIM_SetCompare1(TIM1, i_ccr_temp);
            break;
        }
        case 9: {// PA9对应TIM1的通道2
            TIM_SetCompare2(TIM1, i_ccr_temp);
            break;
        }
        case 10: {// PA10对应TIM1的通道3
            TIM_SetCompare3(TIM1, i_ccr_temp);
            break;
        }
        default: {}
    }
}

/*=====================电机类=====================*/
MyMotor::MyMotor(int i_motor) 
:p_normal(i_motor), p_pwm(i_motor+8), f_speed(0.0)
{
    // 用了初始化列表
}
void MyMotor::spinStop() {
    //Normal接高，PWM接1.0占空比
    f_speed = 0.0;
    p_normal.setHigh(true);
    p_pwm.setDuty(1.0);
}
void MyMotor::spinLose() {
    //Normal接低，PWM接0.0占空比
    f_speed = 0.0;
    p_normal.setHigh(false);
    p_pwm.setDuty(0.0);    
}
void MyMotor::spinLeftHand(float f_speed_arg) {
    /*越界处理*/
    if(f_speed_arg > 1.0) f_speed_arg = 1.0;
    if(f_speed_arg < -1.0) f_speed_arg = -1.0;
    f_speed = f_speed_arg;
    /*正反转情况分流*/
    if(f_speed >= 0.0 ) {
        p_normal.setHigh(true);
        p_pwm.setDuty(1.0 - f_speed);
    } else {
        p_normal.setHigh(false);
        p_pwm.setDuty((1.0 - f_speed) / 2.0);
    }
}

/*=====================小车类=====================*/
MyCar::MyCar() 
:f_speed_x(0.0), f_speed_y(0.0), f_speed_w(0.0), i_head(0), f_temper(0.8), m_0(0), m_1(1), m_2(2)
{
    // 用了初始化列表
}
void MyCar::moveStop() {
    m_0.spinStop();
    m_1.spinStop();
    m_2.spinStop();
}
void MyCar::moveStopSmooth() {
    m_0.spinLose();
    m_1.spinLose();
    m_2.spinLose();
}
void MyCar::headChange(int i_head_arg) {
    i_head = i_head_arg;
}
void MyCar::setTemper(float f_temper_arg) {
    f_temper = f_temper_arg;
}
int MyCar::getHead() {
    return i_head;
}
void MyCar::setMotorSpeedX(int i_motor_arg, float f_speed_arg) {
    switch(i_motor_arg) {
        case 0: {
            m_0.spinLeftHand(f_speed_arg);
            break;
        }
        case 1: {
            m_1.spinLeftHand(f_speed_arg);
            break;
        }        
        case 2: {
            m_2.spinLeftHand(f_speed_arg);
            break;
        }        
        default: {}
    }
}
void MyCar::headBaseMove(float f_speed_x_arg, float f_speed_y_arg) {
    /*成员变量赋值*/
    f_speed_x = f_speed_x_arg;
    f_speed_y = f_speed_y_arg;
    
    float f_speed_motor_head_base[3] = {0.0, 0.0, 0.0};
    
    /*运动学分析后的计算公式*/
    float f_normalize = NORM_FACTOR; // 归一化系数
    f_speed_motor_head_base[0] = f_speed_x * (-1.0) + f_speed_y * (0.0);
    f_speed_motor_head_base[1] = f_speed_x * (COS_60)  + f_speed_y * (SIN_60);
    f_speed_motor_head_base[2] = f_speed_x * (COS_60)  + f_speed_y * (-SIN_60);
    
    /*计算结果作用于电机*/
    for(int i=0; i<3; i++) {
        f_speed_motor_head_base[i] *= f_normalize * f_temper;
        setMotorSpeedX( ((i_head + i) % 3) , f_speed_motor_head_base[i]);
    }
}
void MyCar::spinAntiClockwise(float f_speed_w_arg) {
    /*成员变量赋值*/
    f_speed_w = f_speed_w_arg;
    
    for(int i=0; i<3; i++) {
       setMotorSpeedX(i, f_speed_w);
    }
}
void MyCar::headBaseStab(float f_speed_arg) {
    headBaseMove(0.0, f_speed_arg);
    delay_ms(150);
    headBaseMove(0.0, -f_speed_arg);
    delay_ms(150);
}

