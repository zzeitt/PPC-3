#pragma once
#include "sys.h"

/*======================PC13======================*/
class MyLEDC {
  public:
    MyLEDC();
    void flash(int time_flash);
};

/*======================PB7 & PB8 & PB9======================*/
class MyLEDBAll {
    /* 成员变量 */
    // i_pin        当前亮灯的管脚号
    /* 成员函数 */
    // MyLEDBAll()      构造函数
    // pinChange()      当前亮灯管脚切换
    private:
        int i_pin;
    public:
        MyLEDBAll();
        void pinChange(int i_pin_arg);
};
