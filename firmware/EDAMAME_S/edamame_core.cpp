//えだまめSコアボード用クラスライブラリ
//将来的にedamame_func.inoはこのファイルに移行します。

#include "edamame_core.h"


LSM303::LSM303(uint8_t data1,uint8_t data2){
    address_a =data1;
    address_m =data2;
    
}

void LSM303::init_m(){
    Wire.beginTransmission(address_m);
    Wire.write(0x60);
    Wire.write(0b00001101);  
    Wire.endTransmission(true);
    Wire.beginTransmission(address_m);
    Wire.write(0x61);
    Wire.write(0b00000100);  
    Wire.endTransmission(true);
    Wire.beginTransmission(address_m);
    Wire.write(0x62);
    Wire.write(0b00010000);  
    Wire.endTransmission(true);
}

void LSM303::update_m(){

    Wire.beginTransmission(address_m);
    Wire.write(0x60);
    Wire.write(0b00001101);  
    Wire.endTransmission(true);

    Wire.beginTransmission(address_m);
    Wire.write(0x68);
    Wire.endTransmission(false);
    Wire.requestFrom(address_m, 6);
    x = Wire.read();
    x += (Wire.read()<<8);
    y = Wire.read();
    y += (Wire.read()<<8);
    z = Wire.read();
    z += (Wire.read()<<8);

    x = x - x_offset;
    y = y - y_offset;
    z = z - z_offset;
    arg = -1.0 * atan2((double) - 1 * y, (double)x);

}

void LSM303::init_a(){
}

void LSM303::update_a(){
}
