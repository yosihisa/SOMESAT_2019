#include <Wire.h>
#include <SD.h>

#define INA226    0x45
#define MOTOR_LF  25
#define MOTOR_LR  26
#define MOTOR_RF  14
#define MOTOR_RR  27
#define NICHROME  13
#define SD_CS     4


double current,voltage;

void setup(){
    Serial.begin(115200);
    Wire.begin(21, 22);

    delay(3000);
    pinMode(SD_CS, OUTPUT);
    SD.begin(SD_CS, SPI, 24000000, "/sd");

    Power_init();
    Motor_init();
    Nichrome_init();

    SD.remove("/test_battery.csv") ;
    File dataFile = SD.open("/test_battery.csv", FILE_WRITE);
    if(dataFile){
        Serial.println("open");
    }else{
        while(1){
            Serial.println("open failed");
        }
    }
    int cnt =0;

    while(1){

        if(cnt>50 && cnt <60){
            Nichromefire(255);
        }else{
            Nichromefire(0);
        }

        if(cnt > 100){
            if(cnt%30<20){
                Motor_speed(1000,1000);
            }else{
                Motor_speed(0,0);
            }
        }
        Power_update();

        Serial.printf("%4d %8.4f[A] %8.4f[V] \n",cnt,current ,voltage);
        dataFile.print(cnt);
        dataFile.print(",");
        dataFile.print(current);
        dataFile.print(",");
        dataFile.println(voltage);

        delay(100);
        cnt++;
        if(cnt>=300)break;
    }
    dataFile.close();
}

void loop(){

}

void Power_init(){
    Wire.beginTransmission(INA226);
    Wire.write(0x00);
    Wire.write(0b01000101);  
    Wire.write(0b00100111);    
    Wire.endTransmission(true);
}
void Power_update(){
    int a,v;
    //電流
    Wire.beginTransmission(INA226);
    Wire.write(0x01);
    Wire.endTransmission(false);
    Wire.requestFrom(INA226, 2);
    a = 0;
    a |= (Wire.read()<<8);
    a |= Wire.read();

    //電圧
    Wire.beginTransmission(INA226);
    Wire.write(0x02);
    Wire.endTransmission(false);
    Wire.requestFrom(INA226, 2);
    v = 0;
    v |= (Wire.read()<<8);
    v |= Wire.read();

    current = 0.00025 * a;
    voltage = 0.00125 * v;
}

void Motor_init(){
    const int pwm_hz = 1000;
    const int pwm_bit = 10;
    const int ch[4]={0,1,2,3};

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

void Motor_speed(int L ,int R){
    const int ch[4]={0,1,2,3};
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

void Nichrome_init(){
    pinMode(NICHROME, OUTPUT);
    ledcSetup(4, 400, 8);
    ledcAttachPin(NICHROME, 4);
    ledcWrite(4,0);
}

void Nichromefire(int power){
    ledcWrite(4,power);
}