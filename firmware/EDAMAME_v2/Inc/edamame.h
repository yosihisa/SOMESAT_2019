#include "stm32f4xx_hal.h"
#include "math.h"
#include "jpeg_SW.h"
#include "C1098.h"
#include "fatfs.h"
#include "GOAL.h"

#define MAX_DIR 500
#define ENABLE 1
#define DISABLE 0
struct xyza {
	int16_t x, y, z;
	int16_t x_offset;
	int16_t y_offset;
	int16_t z_offset;
	float arg;
};

struct gps {
	uint32_t latitude;
	uint32_t longitude;
	uint8_t hh, mm, ss;
	uint16_t ms;

	int mode;

	float arg;
	unsigned long dist;
};

typedef struct {
	FIL fil;
	long log_num;
}log_t;


typedef struct {

	int mode;
	int dir_num; //ディレクトリ番号
	int file_num;//ファイル番号

	FIL fil;
	IODEV io;
	uint32_t data_size;

	uint32_t xc, yc, s;

}jpeg_t;


typedef struct {

	log_t log;
	jpeg_t jpeg;

	DIR dir;

	int mode;
	int flightPin;

	long voltage;
	long current;

	struct gps gps_data;
	struct xyza compass_data;

	float arg;

	int motor_L;
	int motor_R;
	int nichrome;

	I2C_HandleTypeDef *i2c;
	UART_HandleTypeDef *uart_gps;
	UART_HandleTypeDef *uart_wifi;
	c1098_handle camera;

}myCansat;

void init(myCansat *data);
void update(myCansat *data);
void decode(myCansat *data);
void apply(myCansat *data);
void write(myCansat *data);
void print(myCansat *data);