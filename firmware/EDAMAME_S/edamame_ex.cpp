//えだまめS拡張ボード用クラスライブラリ
//将来的にedamame_func.inoはこのファイルに移行します。

#include "edamame_ex.h"

Power::Power(uint8_t data){
    address =data;
}

void Power::init(){
    Wire.beginTransmission(address);
    Wire.write(0x00);
    Wire.write(0b01000101);  
    Wire.write(0b00100111);    
    Wire.endTransmission(true);
}
void Power::update(){
    //電流
    Wire.beginTransmission(address);
    Wire.write(0x01);
    Wire.endTransmission(false);
    Wire.requestFrom(address, 2);
    a = 0;
    a |= (Wire.read()<<8);
    a |= Wire.read();

    //電圧
    Wire.beginTransmission(address);
    Wire.write(0x02);
    Wire.endTransmission(false);
    Wire.requestFrom(address, 2);
    v = 0;
    v |= (Wire.read()<<8);
    v |= Wire.read();

    current = 0.00025 * a;
    voltage = 0.00125 * v;
}

Motor::Motor(int ch0,int ch1,int ch2,int ch3){
    ch[0]=ch0;
    ch[1]=ch1;
    ch[2]=ch2;
    ch[3]=ch3;
}

void Motor::init(int hz=1000,int bit=10){
    pwm_hz = hz;
    pwm_bit = bit;
    pinMode(MOTOR_LF, OUTPUT);
    pinMode(MOTOR_LR, OUTPUT);
    pinMode(MOTOR_RF, OUTPUT);
    pinMode(MOTOR_RR, OUTPUT);

    ledcSetup(ch[0], pwm_hz, pwm_bit);
    ledcSetup(ch[1], pwm_hz, pwm_bit);
    ledcSetup(ch[2], pwm_hz, pwm_bit);
    ledcSetup(ch[3], pwm_hz, pwm_bit);
    ledcAttachPin(MOTOR_LF, ch[0]);
    ledcAttachPin(MOTOR_LR, ch[1]);
    ledcAttachPin(MOTOR_RF, ch[2]);
    ledcAttachPin(MOTOR_RR, ch[3]);
    ledcWrite(ch[0], 0);
    ledcWrite(ch[1], 0);
    ledcWrite(ch[2], 0);
    ledcWrite(ch[3], 0);
}

void Motor::speed(int L ,int R){
    if(L>=0){
        ledcWrite(ch[0], L);
        ledcWrite(ch[1], 0);
    }else{
        ledcWrite(ch[0], 0);    
        ledcWrite(ch[1], -1*L);
    }

    if(R>=0){
        ledcWrite(ch[2], R);
        ledcWrite(ch[3], 0);
    }else{
        ledcWrite(ch[2], 0);    
        ledcWrite(ch[3], -1*R);
    }
}

Nichrome::Nichrome(int ch){
    pwm_ch=ch;
    pinMode(NICHROME, OUTPUT);
    ledcSetup(pwm_ch, 400, 8);
    ledcAttachPin(NICHROME, pwm_ch);
    ledcWrite(pwm_ch,0);
}

void Nichrome::fire(int power){
    ledcWrite(pwm_ch,power);
}