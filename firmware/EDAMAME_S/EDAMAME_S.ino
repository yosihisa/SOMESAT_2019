/*

    重要
    ESP32のボードのバージョンは最新のmaster(85032b2)を使ってください。(2018/10/24)
    ボードマネージャから追加するのではなく、cloneしてget.exeを使うやつ
    
*/
#include <Wire.h>
#include "edamame_core.h"
#include "edamame_ex.h"


TaskHandle_t th_gps;


struct gps {
  double latitude;
  double longitude;
  int8_t hh, mm, ss;
  long state;
  int16_t quality;

};

struct gps gps_data;

void setup() {
  
  //起動時の安定を願って
  delay(500);

  //シリアルポート初期化
  Serial.begin(115200);
  Serial2.begin(9600);

  //I2C初期化
  Wire.begin(21, 22);

  //地磁気設定
  LSM303 compass(LSM303AGR_A,LSM303AGR_M);
  compass.init_m();

  //電流電圧計設定
  Power ina226(INA226);
  ina226.init();

  //PWM初期化
  Motor motor(0,1,2,3);
  motor.init(4000,10);

  Nichrome nichrome(4);

  //ピン初期化

  //xTaskCreatePinnedToCore(get_gps, "GET_GPS", 4096, NULL, 2, &th_gps, 0);

  //地磁気キャリブレーション
  //compass_calibration(&compass, 1000);
  uint32_t cnt=0;
  while(1){
    motor.speed(cnt%2048-1024,cnt%2048-1024);
    //nichrome.fire(cnt%255);
    compass.update_m();
    ina226.update();
    if(cnt%10==0){    
      Serial.printf("X:%4d Y:%4d Z:%4d arg:%5.2f %8.4f[A] %8.4f[V] ",compass.x,compass.y,compass.z,compass.arg ,ina226.current ,ina226.voltage );
      Serial.printf("GPS_mode=%d  %02d:%02d:%02d %12.7f %12.7f",gps_data.quality ,gps_data.hh, gps_data.mm ,gps_data.ss, gps_data.latitude ,gps_data.longitude );
      Serial.println("");
    }
/*
    if(compass.arg < -0.3){
      motor(200,0);
    }

    if(compass.arg > 0.3){
      motor(0,200);
    }

    if((compass.arg < 0.3)&&(compass.arg > -0.3)){
       motor(0,0);
    }
    */
    delay(10);

    cnt++;
  }
}

void loop()
{

}

/*
//地磁気センサのキャリブレーション
void compass_calibration(struct Compass * Compass , long cycle) {
  int16_t x, y,  z, x_max, x_min, y_max, y_min;
  motor(200, 600);

  Wire.beginTransmission(LSM303AGR_M);
  Wire.write(0x60);
  Wire.write(0b00001101);  
  Wire.endTransmission(true);

  //最小値・最大値を最初の測定値で初期化
  Wire.beginTransmission(LSM303AGR_M);
  Wire.write(0x68);
  Wire.endTransmission(false);
  Wire.requestFrom(LSM303AGR_M, 6);
  x = Wire.read();
  x += (Wire.read()<<8);
  y = Wire.read();
  y += (Wire.read()<<8);
  z = Wire.read();
  z += (Wire.read()<<8);

  x_max = x ; x_min = x ;
  y_max = y ; y_min = y ;

  for (long i = 0; i < cycle; i++) {
    Wire.beginTransmission(LSM303AGR_M);
    Wire.write(0x60);
    Wire.write(0b00001101);  
    Wire.endTransmission(true);
    
    Wire.beginTransmission(LSM303AGR_M);
    Wire.write(0x68);
    Wire.endTransmission(false);
    Wire.requestFrom(LSM303AGR_M, 6);
    x = Wire.read();
    x += (Wire.read()<<8);
    y = Wire.read();
    y += (Wire.read()<<8);
    z = Wire.read();
    z += (Wire.read()<<8);
    if (x > x_max)x_max = x;
    if (x < x_min)x_min = x;
    if (y > y_max)y_max = y;
    if (y < y_min)y_min = y;
    delay(10);
  }
  Compass -> x_offset = (x_max + x_min) / 2;
  Compass -> y_offset = (y_max + y_min) / 2;
  motor( 0, 0);
  return;
}*/