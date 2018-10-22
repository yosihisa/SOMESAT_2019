#include <Wire.h>
#define LSM303AGR_A 0x19
#define LSM303AGR_M 0x1E
#define INA226      0x45

#define MOTOR_LF  25
#define MOTOR_LR  26
#define MOTOR_RF  14
#define MOTOR_RR  27
#define NICHROME  13
#define SD_CS     4

//HardwareSerial Serial2(2);
//TaskHandle_t th_gps;

struct Compass{
  int16_t x;
  int16_t y;
  int16_t z;
  int16_t x_offset=0;
  int16_t y_offset=0;
  int16_t z_offset=0;
  double arg;
};

struct Power{
  int32_t v;
  int32_t a;
  double voltage;
  double current;
};

struct gps {
  double latitude;
  double longitude;
  int8_t hh, mm, ss;
  long state;
  int16_t quality;

};

struct gps gps_data;

void setup() {

  struct Compass compass;
  struct Power power;

  //シリアルポート初期化
  Serial.begin(115200);
  Serial2.begin(9600);

  //I2C初期化
  Wire.begin(21, 22);
  //地磁気設定
  Wire.beginTransmission(LSM303AGR_M);
  Wire.write(0x60);
  Wire.write(0b00001101);  
  Wire.endTransmission(true);
  Wire.beginTransmission(LSM303AGR_M);
  Wire.write(0x61);
  Wire.write(0b00000100);  
  Wire.endTransmission(true);
  Wire.beginTransmission(LSM303AGR_M);
  Wire.write(0x62);
  Wire.write(0b00010000);  
  Wire.endTransmission(true);
  //電流電圧計設定
  Wire.beginTransmission(INA226);
  Wire.write(0x00);
  Wire.write(0b01000101);  
  Wire.write(0b00100111);    
  Wire.endTransmission(true);

  //PWM初期化
  ledcSetup(0, 1000, 10);
  ledcAttachPin(MOTOR_LF, 0);
  ledcAttachPin(MOTOR_LR, 1);
  ledcAttachPin(MOTOR_RF, 2);
  ledcAttachPin(MOTOR_RR, 3);
  
  ledcWrite(0, 0);
  ledcWrite(1, 0);
  ledcWrite(2, 0);
  ledcWrite(3, 0);

  //ピン初期化
  pinMode(NICHROME, OUTPUT);
  digitalWrite(NICHROME, LOW);

  //xTaskCreatePinnedToCore(get_gps, "GET_GPS", 4096, NULL, 2, &th_gps, 0);

  //地磁気キャリブレーション
  compass_calibration(&compass, 1000);
  uint32_t cnt=0;
  while(1){
    get_compass(&compass);
    get_powerState(&power);

    if(cnt%10==0){    
      Serial.printf("X:%4d Y:%4d Z:%4d arg:%5.2f %8.4f[A] %8.4f[V] ",compass.x,compass.y,compass.z,compass.arg ,power.current ,power.voltage );
      Serial.printf("GPS_mode=%d  %02d:%02d:%02d %12.7f %12.7f",gps_data.quality ,gps_data.hh, gps_data.mm ,gps_data.ss, gps_data.latitude ,gps_data.longitude );
      Serial.println("");
    }

    if(compass.arg < -0.3){
      motor(200,0);
    }

    if(compass.arg > 0.3){
      motor(0,200);
    }
    delay(100);

    cnt++;
  }
}

void loop()
{

}

void nichrome(int power){
    ledcWrite(4, power);
}

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
}