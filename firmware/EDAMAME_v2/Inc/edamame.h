
#include "stm32f4xx_hal.h"
#include "math.h"
#include "tjpgd.h"


//色相の範囲
#define H_MIN_1 0	//固定
#define H_MAX_1 10

#define H_MIN_2 330	
#define H_MAX_2 360 //固定

//彩度の範囲
#define S_MIN 50
#define S_MAX 100

//明度の範囲
#define V_MIN 30
#define V_MAX 100

//面積
#define SQUARE 120 

//ゴール判定(過去５回の重心の移動距離の2乗の合計がこの値以下だったらゴール)
#define GOAL 100

#define HEIGHT 480
#define WIDTH  640



//ゴールの座標を指定(dd.dddddd , ddd,dddddd)
//九工大校庭
//#define GOAL_LATITUDE	 33.8903550
//#define GOAL_LONGITUDE	130.8402200

//種子島
#define GOAL_LATITUDE	 30.375900
#define GOAL_LONGITUDE	130.960693

//能代
//#define GOAL_LATITUDE	 33.8903550
//#define GOAL_LONGITUDE	130.8402200

//33.890335,130.840225
//ゴールとするエリアの半径(dd.dddddd形式）
#define GOAL_ALEA	0.00001
#define GOAL_ALEA2	GOAL_ALEA * GOAL_ALEA

//低電圧警告（単位[V])
#define LOW_BATTERY 7.0

//磁気偏角(西偏が正）
#define MAGNETIC_DECLINATION 7.71/360 * 3.1415926535

#define HMC5883L_WRITE 0x3C
#define HMC5883L_READ 0x3D

#define HMC5883L_CONFIG_A    0x00
#define HMC5883L_CONFIG_B    0x01
#define HMC5883L_MODE        0x02


uint8_t RED_bool[HEIGHT][WIDTH/8];

typedef struct {
	uint8_t jpeg_data[40000];
	uint32_t jpeg_data_seek ;
    uint32_t jpeg_data_size ;
    uint8_t RED_bool[HEIGHT][WIDTH / 8];
} IODEV;


//uint32_t in_func(JDEC* jd, uint8_t* buff, uint32_t nbyte);
//uint32_t out_func(JDEC* jd, void* bitmap, JRECT* rect);

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

struct compass {
	int x, y, z;
	int x_offset, y_offset;
	double arg;
};

struct gps {
	float latitude;
	float longitude;
	int8_t hh, mm, ss;
};

//電池電圧を返すやつ
int get_battery(ADC_HandleTypeDef* hadc);

//GPS受信するやつ
int get_gps(struct gps *gps_data, UART_HandleTypeDef *huart, int TIMEOUT);

//地磁気キャリブレーションするやつ
void compass_calibrate(struct compass *Compass, I2C_HandleTypeDef *hi2c, TIM_HandleTypeDef *htimL, TIM_HandleTypeDef *htimR, UART_HandleTypeDef *huart, ADC_HandleTypeDef* hadc, int cycle);

//地磁気の値を読む
void get_compass(struct compass *Compass, I2C_HandleTypeDef *hi2c);

//モーター速度変更
void motor(TIM_HandleTypeDef *htimL ,TIM_HandleTypeDef *htimR,int L ,int R);


uint32_t in_func(JDEC* jd, uint8_t* buff, uint32_t nbyte);
uint32_t out_func(JDEC* jd, void* bitmap, JRECT* rect);
