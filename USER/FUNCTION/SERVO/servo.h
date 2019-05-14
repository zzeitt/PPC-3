#pragma once
#include "sys.h"


/*======================PA6 & PA7 & PB0======================*/
class MyServoAll {
    /* 成员函数 */
    // MyServoAll()     构造函数
    // setDuty()        调节占空比
    // setAngle()       设置转动角度
    // initAngle()      初始化角度
    // doBeckon()       招手动作
    public:
        MyServoAll();
        void setDuty(int i_pin_arg, float f_duty_arg);
        void setAngle(int i_pin_arg, int i_angle_arg);
        void initAngle(int i_pin_arg);
        void doBeckon(int i_pin_arg, int i_angle_arg, int i_delay_arg);
};
