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
    /* ��Ա���� */
    // i_pin        ��ǰ���ƵĹܽź�
    /* ��Ա���� */
    // MyLEDBAll()      ���캯��
    // pinChange()      ��ǰ���ƹܽ��л�
    private:
        int i_pin;
    public:
        MyLEDBAll();
        void pinChange(int i_pin_arg);
};
