//えだまめSコアボード用クラスライブラリ
//将来的にedamame_func.inoはこのファイルに移行します。

#ifndef _edamame_core_h
#define _edamame_core_h

#include <Arduino.h>
#include <Wire.h>


#define LSM303AGR_A 0x19
#define LSM303AGR_M 0x1E
#define SD_CS     4

//LSM303AGRと通信するクラス
class LSM303{
    public:
        LSM303(uint8_t,uint8_t);
        void init_a();
        void init_m();
        void update_a();
        void update_m();
        int16_t x,y,z;
        int16_t x_offset=0;
        int16_t y_offset=0;
        int16_t z_offset=0;
        double arg;
    private:
        uint8_t address_a;
        uint8_t address_m;
};



#endif