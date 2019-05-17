#pragma once
#include "sys.h"

#define COS_60 0.5
#define SIN_60 0.866
#define NORM_FACTOR 0.73205

/*=====================六个管脚初始化=====================*/
void allMotorPinInit();

/*=====================普通受口类=====================*/
class MyPinNormal {
    /* 成员变量 */
    // i_pin        普通IO的管脚号
    // g_pin        普通IO的管脚名（如,GPIO_Pin_0）
    // b_high       管脚当前是否为高电平
    /* 成员函数 */
    // MyPinNormal()    构造函数
    // setHigh()        管脚置高
    private:
        int i_pin;
        uint16_t g_pin;
        bool b_high;
    public:
        MyPinNormal(int i_pin_arg);
        void setHigh(bool b_high_arg);
};

/*=====================PWM波受口类=====================*/
class MyPinPWM {
    /* 成员变量 */
    // i_pin        PWM输出的管脚号
    // i_arr        PWM的预装载值
    // i_ccr        PWM当前的比较值
    // f_duty       PWM当前的占空比
    /* 成员函数 */
    // MyPinPWM()   构造函数
    // setDuty()    设置占空比
    private:
        int i_pin;
        float f_duty;
    public:
        MyPinPWM(int i_pin_arg);
        void setDuty(float f_duty_arg);
};

/*=====================电机类=====================*/
class MyMotor {
    /* 成员说明 */
    // p_normal             电机的普通管脚
    // p_pwm                电机的pwm管脚
    // f_speed              电机的速度
    /* 成员函数 */
    // MyMotor()            构造函数
    // spinStop()           电机停止转动
    // spinLose()           电机掉电（瞬间失去了电的依靠）
    // spinLefttHand()      电机转动（左手螺旋）
    private:
        MyPinNormal p_normal;
        MyPinPWM p_pwm;
        float f_speed;
    public:
        MyMotor(int i_motor);
        void spinStop();
        void spinLose();
        void spinLeftHand(float f_speed_arg);
};

/*=====================小车类=====================*/
class MyCar {
    /* 成员变量 */
    // f_speed_x    小车x方向速度（以车头为Y基准）
    // f_speed_y    小车y方向速度（以车头为Y基准）
    // f_speed_w    小车旋转角速度
    // i_head       小车的头（由电机号指代）
    // f_temper     小车的模式（温和 -> 狂暴）
    // m_0          电机0
    // m_1          电机1
    // m_2          电机2
    /* 成员函数 */
    // MyCar()              构造函数
    // moveStop()           小车停止函数
    // moveStopSmooth()     惯性停车
    // setTemper()          小车脾气函数
    // getHead()            获取当前车头
    // headChange()         车头变换函数
    // setMotorSpeedX()     设置小车单个电机的速度
    // headBaseMove()       基于车头的位移
    // balletClockwise()    芭蕾旋转
    private:
        float f_speed_x;
        float f_speed_y;
        float f_speed_w;
        int i_head;
        float f_temper;
        MyMotor m_0;
        MyMotor m_1;
        MyMotor m_2;
    public:
        MyCar();
        void moveStop();
        void moveStopSmooth();
        void headChange(int i_head_arg);
        void setTemper(float f_temper_arg);
        int getHead();
        void setMotorSpeedX(int i_motor_arg, float f_speed_arg);
        void headBaseMove(float f_speed_x_arg, float f_speed_y_arg);
        void spinAntiClockwise(float f_speed_w_arg);
        void headBaseStab(float f_speed_arg);
        void headBaseTemperMove(float f_temper_arg, float f_speed_x_arg, float f_speed_y_arg);
};

