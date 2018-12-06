//えだまめS拡張ボード用クラスライブラリ
//将来的にedamame_func.inoはこのファイルに移行します。

#ifndef _edamame_ex_h
#define _edamame_ex_h

#include <Arduino.h>
#include <Wire.h>


#define INA226    0x45
#define MOTOR_LF  25
#define MOTOR_LR  26
#define MOTOR_RF  14
#define MOTOR_RR  27
#define NICHROME  13


//INA226と通信するクラス
class Power{
    public:
        Power(uint8_t);
        void update();
        void init();
        double voltage;
        double current;
    private:
        uint8_t address;
        int32_t v;
        int32_t a;
};

//モータ
class Motor{
    public:
        Motor(int ch0=0,int ch1=1,int ch2=2,int ch3=3);
        void init(int,int);
        void speed(int ,int);
    private:
        uint8_t ch[4];
        uint8_t pwm_bit;
        uint16_t pwm_hz;
};

//ニクロム線
class Nichrome{
    public:
        Nichrome(int ch=4);
        void fire(int);
    private:
        uint8_t pwm_ch;
};

#endif