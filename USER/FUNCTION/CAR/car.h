#pragma once
#include "sys.h"

#define COS_60 0.5
#define SIN_60 0.866
#define NORM_FACTOR 0.73205

/*=====================�����ܽų�ʼ��=====================*/
void allMotorPinInit();

/*=====================��ͨ�ܿ���=====================*/
class MyPinNormal {
    /* ��Ա���� */
    // i_pin        ��ͨIO�Ĺܽź�
    // g_pin        ��ͨIO�Ĺܽ�������,GPIO_Pin_0��
    // b_high       �ܽŵ�ǰ�Ƿ�Ϊ�ߵ�ƽ
    /* ��Ա���� */
    // MyPinNormal()    ���캯��
    // setHigh()        �ܽ��ø�
    private:
        int i_pin;
        uint16_t g_pin;
        bool b_high;
    public:
        MyPinNormal(int i_pin_arg);
        void setHigh(bool b_high_arg);
};

/*=====================PWM���ܿ���=====================*/
class MyPinPWM {
    /* ��Ա���� */
    // i_pin        PWM����Ĺܽź�
    // i_arr        PWM��Ԥװ��ֵ
    // i_ccr        PWM��ǰ�ıȽ�ֵ
    // f_duty       PWM��ǰ��ռ�ձ�
    /* ��Ա���� */
    // MyPinPWM()   ���캯��
    // setDuty()    ����ռ�ձ�
    private:
        int i_pin;
        float f_duty;
    public:
        MyPinPWM(int i_pin_arg);
        void setDuty(float f_duty_arg);
};

/*=====================�����=====================*/
class MyMotor {
    /* ��Ա˵�� */
    // p_normal             �������ͨ�ܽ�
    // p_pwm                �����pwm�ܽ�
    // f_speed              ������ٶ�
    /* ��Ա���� */
    // MyMotor()            ���캯��
    // spinStop()           ���ֹͣת��
    // spinLose()           ������磨˲��ʧȥ�˵��������
    // spinLefttHand()      ���ת��������������
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

/*=====================С����=====================*/
class MyCar {
    /* ��Ա���� */
    // f_speed_x    С��x�����ٶȣ��Գ�ͷΪY��׼��
    // f_speed_y    С��y�����ٶȣ��Գ�ͷΪY��׼��
    // f_speed_w    С����ת���ٶ�
    // i_head       С����ͷ���ɵ����ָ����
    // f_temper     С����ģʽ���º� -> �񱩣�
    // m_0          ���0
    // m_1          ���1
    // m_2          ���2
    /* ��Ա���� */
    // MyCar()              ���캯��
    // moveStop()           С��ֹͣ����
    // moveStopSmooth()     ����ͣ��
    // setTemper()          С��Ƣ������
    // getHead()            ��ȡ��ǰ��ͷ
    // headChange()         ��ͷ�任����
    // setMotorSpeedX()     ����С������������ٶ�
    // headBaseMove()       ���ڳ�ͷ��λ��
    // balletClockwise()    ������ת
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

