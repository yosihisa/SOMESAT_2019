//モーターの速度変更
void motor(int l ,int r){
  if(l>=0){
    ledcWrite(0, l);
    ledcWrite(1, 0);
  }else{
    ledcWrite(0, 0);    
    ledcWrite(1, -1*l);
  }

  if(r>=0){
    ledcWrite(2, r);
    ledcWrite(3, 0);
    
  }else{
    ledcWrite(2, 0);    
    ledcWrite(3, -1*r);
  }
}

//電圧を読む
void get_powerState(Power *data){
  //電流
  Wire.beginTransmission(INA226);
  Wire.write(0x01);
  Wire.endTransmission(false);
  Wire.requestFrom(INA226, 2);
  data->a = 0;
  data->a |= (Wire.read()<<8);
  data->a |= Wire.read();

  //電圧
  Wire.beginTransmission(INA226);
  Wire.write(0x02);
  Wire.endTransmission(false);
  Wire.requestFrom(INA226, 2);
  data->v = 0;
  data->v |= (Wire.read()<<8);
  data->v |= Wire.read();

  data->current = 0.00025 * data->a;
  data->voltage = 0.00125 * data->v;
}

//地磁気の値を詠む
void get_compass(Compass *data){
  int16_t x, y, z;
  
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

  data -> x = x - data->x_offset;
  data -> y = y - data->y_offset;
  data -> z = z - data->z_offset;
  data -> arg = -1.0 * atan2((double) - 1 * data -> y, (double)data -> x);

  return;
}

//GPS座標取得
void get_gps(void *pvParameters) {
  char c, gps_msg[96];
  long GPS_stat = 0;
  long dd , mm, mmmm, ddd, dddd;
  while(1) {

    //GPSのセンテンスの始まりを探してデータタイプの種類を代入
    c = get_gps_1();
    if (c == '$') {
      for (long i = 0 ; i < 6 ; i++) {
        c = get_gps_1();
        gps_msg[i] = c;
      }
      //時刻・座標の取得計算
      if (strncmp(gps_msg, "GPGGA", 5) == 0) {
        for (long i = 0 ; c != '\n' ; i++) {
          c = get_gps_1();
          gps_msg[i] = c;

          //1行取得し終えたら
          if (c == '\n') {
            gps_data.latitude = 0;
            gps_data.longitude = 0;
            gps_data.hh = 0;
            gps_data.mm = 0;
            gps_data.ss = 0;
            gps_data.quality = -1;
            GPS_stat = 0;

            gps_data.hh = 10 * (gps_msg[0] - 48) + (gps_msg[1] - 48);
            gps_data.mm = 10 * (gps_msg[2] - 48) + (gps_msg[3] - 48);
            gps_data.ss = 10 * (gps_msg[4] - 48) + (gps_msg[5] - 48);
            if (gps_msg[6] != '.')GPS_stat |= 0b0000001;
            if (gps_data.hh < 0 || gps_data.hh > 24)GPS_stat = 0b0000001;

            //測位できていたら
            if (i > 40) {
              long n;
              dd = 0;
              for (n = 6; n < i; n++) {
                if (gps_msg[n] == ',') {
                  dd  = 10 * (gps_msg[n + 1] - 48) + (gps_msg[n + 2] - 48);
                  mm  = 10 * (gps_msg[n + 3] - 48) + (gps_msg[n + 4] - 48);
                  mmmm = 1000 * (gps_msg[n + 6] - 48) + 100 * (gps_msg[n + 7] - 48) + 10 * (gps_msg[n + 8] - 48) + (gps_msg[n + 9] - 48);

                  gps_data.latitude = (dd + (((double)mm + (double)mmmm / 10000) / 60 )) ;
                  if (gps_msg[n + 10] != ',' || gps_msg[n + 11] != 'N')GPS_stat |= 0b0000010;
                  break;
                }
              }
              for (n = n + 1 ; n < i; n++) {
                if (gps_msg[n] == 'N') {
                  break;
                }
              }
              for (n = n + 1 ; n < i; n++) {
                if (gps_msg[n] == ',') {
                  ddd  = 100 * (gps_msg[n + 1] - 48) + 10 * (gps_msg[n + 2] - 48) + (gps_msg[n + 3] - 48);
                  mm  = 10 * (gps_msg[n + 4] - 48) + (gps_msg[n + 5] - 48);
                  mmmm = 1000 * (gps_msg[n + 7] - 48) + 100 * (gps_msg[n + 8] - 48) + 10 * (gps_msg[n + 9] - 48) + (gps_msg[n + 10] - 48);
                  gps_data.longitude = (ddd + (((double)mm + (double)mmmm / 10000) / 60 )) ;
                  if (gps_msg[n + 11] != ',' || gps_msg[n + 12] != 'E')GPS_stat |= 0b0000100;
                  break;
                }
              }
              for (n = n + 1 ; n < i; n++) {
                if (gps_msg[n] == 'E') {
                  break;
                }
              }
              gps_data.quality = (gps_msg[n + 2] - 48);
            }
            //測位できていなかったら
            else   GPS_stat |= 0b0010000;
            gps_data.state= GPS_stat ;
            vTaskDelay(800);
          }
        }
      }
    }
  }
}

//GPSから1文字受信
char get_gps_1() {
  while (1) {
    long data = Serial2.read();
    if (data != -1) {
      char c = (char)data;
      return data;
    }
  }
}

/*
//UARTで制御履歴を送る
void uart_send() {
  Serial.print(log_num);
  Serial.print(",");
  Serial.print(mode);
  Serial.print(",");
  Serial.print(gps.hh);
  Serial.print(",");
  Serial.print(gps.mm);
  Serial.print(",");
  Serial.print(gps.ss);
  Serial.print(",");
  Serial.print(gps.latitude);
  Serial.print(",");
  Serial.print(gps.longitude);
  Serial.print(",");
  Serial.print(gps_stat);
  Serial.print(",");
  Serial.print(Vattery_Voltage);
  Serial.print(",");
  Serial.print(HMC5883L.x);
  Serial.print(",");
  Serial.print(HMC5883L.y);
  Serial.print(",");
  Serial.print(HMC5883L.arg);
  Serial.print(",");
  Serial.print(dist);
  Serial.print(",");
  Serial.print(a);
  Serial.print(",");
  Serial.print(arg);
  Serial.print(",");
  Serial.print(motorL);
  Serial.print(",");
  Serial.print(motorR);
  Serial.print(",");
  Serial.print(US_Distance);
  Serial.println("");
  return;
}
//制御履歴を書き込む
void SD_write() {
  String SDString = "";
  dataFile = SD.open(path, FILE_WRITE);
  SDString.concat(log_num);
  SDString.concat(",");
  SDString.concat(mode);
  SDString.concat(",");
  SDString.concat(gps.hh);
  SDString.concat(",");
  SDString.concat(gps.mm);
  SDString.concat(",");
  SDString.concat(gps.ss);
  SDString.concat(",");
  SDString.concat(gps.latitude);
  SDString.concat(",");
  SDString.concat(gps.longitude);
  SDString.concat(",");
  SDString.concat(gps_stat);
  SDString.concat(",");
  SDString.concat(Vattery_Voltage);
  SDString.concat(",");
  SDString.concat(HMC5883L.x);
  SDString.concat(",");
  SDString.concat(HMC5883L.y);
  SDString.concat(",");
  SDString.concat(HMC5883L.arg);
  SDString.concat(",");
  SDString.concat(dist);
  SDString.concat(",");
  SDString.concat(a);
  SDString.concat(",");
  SDString.concat(arg);
  SDString.concat(",");
  SDString.concat(motorL);
  SDString.concat(",");
  SDString.concat(motorR);
  SDString.concat(",");
  SDString.concat(US_Distance);
  SDString.concat("\n");
  dataFile.print(SDString);
  dataFile.close();
  if (log_num % 2 == 0)digitalWrite(LED1, HIGH);
  digitalWrite(LED1, LOW);
  log_num++;
  return;
}
*/
