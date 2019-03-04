#include "edamame.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define HMC5883L_WRITE 0x3C
#define HMC5883L_READ 0x3D

#define HMC5883L_CONFIG_A    0x00
#define HMC5883L_CONFIG_B    0x01
#define HMC5883L_MODE        0x02

FATFS fs;


void ina226_init(I2C_HandleTypeDef *hi2c) {

}
long ina226_voltage(I2C_HandleTypeDef *hi2c) {
	return 0;
}
long ina226_current(I2C_HandleTypeDef *hi2c) {
	return 0;
}

void compass_init(I2C_HandleTypeDef *hi2c, struct xyza *data) {
	data->x_offset = 0;
	data->y_offset = 0;
	data->z_offset = 0;
	data->arg = 0;

	uint8_t pData;
	pData = 0x50; HAL_I2C_Mem_Write(hi2c, HMC5883L_WRITE, HMC5883L_CONFIG_A, 1, &pData, 1, 100);
	pData = 0x20; HAL_I2C_Mem_Write(hi2c, HMC5883L_WRITE, HMC5883L_CONFIG_B, 1, &pData, 1, 100);
	pData = 0x00; HAL_I2C_Mem_Write(hi2c, HMC5883L_WRITE, HMC5883L_MODE	   , 1, &pData, 1, 100);
}

void compass_update(struct xyza *data, I2C_HandleTypeDef *hi2c){
	int16_t x,y;
	uint8_t Data[14];  	
	HAL_I2C_Mem_Read(hi2c	,HMC5883L_READ	,0x03	,1	,Data,6,100);
	x = Data[0]<<8 ;x |= Data[1];
	y = Data[4]<<8 ;y |= Data[5];
	data-> x = (int)x - data->x_offset;
	data-> y = (int)y - data->y_offset;
	data-> arg = -1.0 * atan2((double)data-> y,(double)data-> x);
	return;
}


void set_gpsHiSpeed(UART_HandleTypeDef *huart) {
	char str[100];
	HAL_Delay(2000);
	sprintf(str, "$PMTK314,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n");
	HAL_UART_Transmit(huart, str, strlen(str), 2000);
	HAL_Delay(80);
	HAL_UART_Transmit(huart, str, strlen(str), 2000);
	HAL_Delay(80);
	HAL_UART_Transmit(huart, str, strlen(str), 2000);
	HAL_Delay(80);
	sprintf(str,"$PMTK220,100*2F\r\n");
	HAL_UART_Transmit(huart, str, strlen(str), 2000);
	HAL_Delay(80);
	HAL_UART_Transmit(huart, str, strlen(str), 2000);
	HAL_Delay(80);
	HAL_UART_Transmit(huart, str, strlen(str), 2000);
	HAL_Delay(80);

}

//GPS受信
int get_gps(struct gps *gps_data, UART_HandleTypeDef *huart, int TIMEOUT) {
	gps_data->latitude = 0;
	gps_data->longitude = 0;
	gps_data->hh = 0;
	gps_data->mm = 0;
	gps_data->ss = 0;
	gps_data->ms = 0;

	int timeout = 0;

	uint8_t c, gps_msg[100];
	long dd, mm, mmmm, ddd;

	while (TIMEOUT > timeout) {
		if (HAL_UART_Receive(huart, &c, 1, 5) != HAL_OK) {
			timeout++;
			continue;
		}
		if (c == '$') {
			for (int i = 0; i < 6; i++) {
				HAL_UART_Receive(huart, &c, 1, 10);
				gps_msg[i] = c;
			}
			if (strncmp(gps_msg, "GPGGA", 5) == 0) {
				for (int i = 0; c != '\n'; i++) {
					HAL_UART_Receive(huart, &c, 1, 10);
					gps_msg[i] = c;

					if (c == '\n') {
						gps_data->hh = 10 * (gps_msg[0] - 48) + (gps_msg[1] - 48);
						gps_data->mm = 10 * (gps_msg[2] - 48) + (gps_msg[3] - 48);
						gps_data->ss = 10 * (gps_msg[4] - 48) + (gps_msg[5] - 48);
						if (gps_msg[6] != '.')gps_data->mode = -1;
						gps_data->ms = 100 * (gps_msg[7] - 48) + 10 * (gps_msg[8] - 48) + (gps_msg[9] - 48);
						if (gps_data->hh < 0 || gps_data->hh > 24)gps_data->mode = -1;


						if (i > 40) {
							long n;
							dd = 0;
							for (n = 5; n < i; n++) {
								if (gps_msg[n] == ',') {
									dd = 10 * (gps_msg[n + 1] - 48) + (gps_msg[n + 2] - 48);
									mm = 10 * (gps_msg[n + 3] - 48) + (gps_msg[n + 4] - 48);
									mmmm = 1000 * (gps_msg[n + 6] - 48) + 100 * (gps_msg[n + 7] - 48) + 10 * (gps_msg[n + 8] - 48) + (gps_msg[n + 9] - 48);

									gps_data->latitude = (dd + (((double)mm + (double)mmmm / 10000) / 60)) * 1000000;
									if (gps_msg[n + 10] != ',' || gps_msg[n + 11] != 'N')gps_data->mode = -1;
									break;
								}
							}
							for (n = n + 1; n < i; n++) {
								if (gps_msg[n] == 'N') {
									break;
								}
							}
							for (n = n + 1; n < i; n++) {
								if (gps_msg[n] == ',') {
									ddd = 100 * (gps_msg[n + 1] - 48) + 10 * (gps_msg[n + 2] - 48) + (gps_msg[n + 3] - 48);
									mm = 10 * (gps_msg[n + 4] - 48) + (gps_msg[n + 5] - 48);
									mmmm = 1000 * (gps_msg[n + 7] - 48) + 100 * (gps_msg[n + 8] - 48) + 10 * (gps_msg[n + 9] - 48) + (gps_msg[n + 10] - 48);
									gps_data->longitude = (ddd + (((double)mm + (double)mmmm / 10000) / 60)) * 1000000;
									if (gps_msg[n + 11] != ',' || gps_msg[n + 12] != 'E')gps_data->mode = -1;
									break;
								}
							}
							for (n = n + 1; n < i; n++) {
								if (gps_msg[n] == 'E') {
									break;
								}
							}
							for (n = n + 1; n < i; n++) {
								if (gps_msg[n] == ',') {
									gps_data->mode = (gps_msg[n + 1] - 48);
									break;
								}
							}
						} else {
							gps_data->mode = 0;
						}
						if (gps_data->mode == -1) {
							gps_data->latitude = 0;
							gps_data->longitude = 0;
						} else {
							return gps_data->mode;
						}
					}
				}
			}
		}
	}
	return -1;
}

void calc_gps(struct gps *gps_data, long latitude, long longitude) {

	long dx = latitude - gps_data->latitude;	//南北　北が正
	long dy = longitude - gps_data->longitude;	//東西　東が正
	gps_data->arg = atan2(dy, dx);
	gps_data->dist = dx * dx + dy * dy;
}

void init(myCansat *data) {

	FRESULT res_fs;

	data->motor_L = 0;
	data->motor_R = 0;
	data->nichrome = GPIO_PIN_RESET;
	data->mode = 0;

	compass_init(data->i2c, &data->compass_data);
	printf("Init compass\n");

	ina226_init(data->i2c);
	printf("Init INA226\n");

	set_gpsHiSpeed(data->uart_gps);
	data->gps_data.arg = 0;
	data->gps_data.dist = 0;
	data->gps_data.mode = -1;
	printf("Init GNSS\n");

	while (camera_init(&data->camera) != CAMERA_OK);
	printf("Init Camera\n");


	//SDカードを初期化できるまで待つ
	do {
		res_fs = f_mount(&fs, "", 1);
		if (res_fs != FR_OK) {
			printf("f_mount() : error %u\n", res_fs);
			HAL_Delay(120);
		}

		res_fs = f_opendir(&data->dir, "");
		if (res_fs != FR_OK) {
			printf("f_opendir() : error %u\n", res_fs);
			HAL_Delay(120);
		}
		HAL_Delay(800);
	} while (res_fs != FR_OK);
	printf("Init SDcard\n");
	
	//ディレクトリの作成
	char path[30];
	
	for (data->jpeg.dir_num = 0; data->jpeg.dir_num < MAX_DIR; data->jpeg.dir_num++) {
		sprintf(path, "/edmm%03d", data->jpeg.dir_num);
		res_fs = f_mkdir(path);
		if (res_fs == FR_OK) {
			printf("Make dir  /edmm%03d\n", data->jpeg.dir_num);
			break;
		} else {
			printf("f_mkdir():error %d     /%03d/ \n", res_fs, data->jpeg.dir_num);
		}
	}


	data->jpeg.file_num = 0;
	data->jpeg.mode = DISABLE;
	data->log.log_num = 0;
	//制御履歴ファイルの作成
	sprintf(path, "/edmm%03d/log.csv", data->jpeg.dir_num);
	res_fs = f_open(&data->log.fil, (char*)path, FA_WRITE | FA_CREATE_ALWAYS);
	if (res_fs != FR_OK) {
		printf("f_open() : %d\n", res_fs);
	}
	res_fs = f_sync(&data->log.fil);
	if (res_fs != FR_OK) {
		printf("f_sync() : %d\n", res_fs);
	}

}
void update(myCansat *data) {
	
	const float pi = 3.1415926535;

	get_gps(&data->gps_data, data->uart_gps, 400);
	calc_gps(&data->gps_data, GOAL_LATITUDE, GOAL_LONGITUDE);
	compass_update(&data->compass_data, data->i2c);

	data->voltage = ina226_voltage(data->i2c);
	data->current = ina226_current(data->i2c);

	data->flightPin = HAL_GPIO_ReadPin(FLIGHT_PIN_GPIO_Port, FLIGHT_PIN_Pin);

	data->jpeg.xc = 0;
	data->jpeg.yc = 0;
	data->jpeg.s = 0;

	if (data->jpeg.mode == ENABLE) {
		for (int n = 0; n < 5; n++) {
			while (snap_shot(&data->camera) != CAMERA_OK);
			get_picture(&data->camera, data->jpeg.io.jpeg_data, MAX_SIZE, &data->jpeg.data_size);
			if (data->jpeg.data_size > 1024)break;
		}
	}
	data->arg = data->gps_data.arg - data->compass_data.arg;
	while (data->arg > 1.0 * pi) data->arg -= 2 * pi;
	while (data->arg < -1.0 * pi) data->arg += 2 * pi;

}
void decode(myCansat *data) {
	//JPEGデコード
	JRESULT jpeg_res;
	uint8_t work[3100];
	JDEC jdec;

	data->jpeg.xc = 0;
	data->jpeg.yc = 0;
	data->jpeg.s = 0;

	data->jpeg.io.jpeg_data_seek = 0;
	jpeg_res = jd_prepare(&jdec, in_func, work, 3100, &data->jpeg.io.jpeg_data);
	memset(&data->jpeg.io.RED_bool, 0, HEIGHT*WIDTH/8);
	

	if (jpeg_res == JDR_OK) {
		
		//デコード開始
		jpeg_res = jd_decomp(&jdec, out_func, 0);

		if (jpeg_res == JDR_OK) {

			//重心計算
			TIM2->CNT = 0;
			for (UINT h = 0; h < jdec.height; h++) {
				for (UINT w = 0; w < jdec.width; w++) {
					if ((data->jpeg.io.RED_bool[h][w / 8] & (0b10000000 >> (w % 8))) != 0) {
						data->jpeg.xc += w;
						data->jpeg.yc += h;
						data->jpeg.s++;
					}
				}
			}
			data->jpeg.xc /= data->jpeg.s;
			data->jpeg.yc /= data->jpeg.s;
		}
	}
}

void write(myCansat *data) {

	FRESULT res_fs;
	char str[300];
	char path[30];
	unsigned int writeBytes;

	if (data->jpeg.mode == ENABLE) {
		sprintf(path, "/edmm%03d/%03d.jpg", data->jpeg.dir_num, data->jpeg.file_num);
		f_open(&data->jpeg.fil, path, FA_WRITE | FA_CREATE_ALWAYS);
		res_fs = f_write(&data->jpeg.fil, &data->jpeg.io.jpeg_data, data->jpeg.data_size, &writeBytes);
		f_close(&data->jpeg.fil);
		data->jpeg.file_num++;
		if (res_fs != FR_OK) {
			printf("f_write error:%d\n", res_fs);
		}
	}

	sprintf(str, "%ld,%d, %02d,%02d,%02d.%03d, %ld,%ld, %lu,%lu,%f,%d, %d,%d,%f,%lu,%f, %d,%d, %d, %lu,%lu,%lu\n",
		data->log.log_num, data->mode,
		data->gps_data.hh, data->gps_data.mm, data->gps_data.ss, data->gps_data.ms,
		data->voltage, data->current,
		data->gps_data.latitude, data->gps_data.longitude, data->gps_data.arg, data->gps_data.mode,
		data->compass_data.x, data->compass_data.y,data->compass_data.arg,
		data->gps_data.dist, data->arg,
		data->motor_L, data->motor_R,
		data->jpeg.file_num,
		data->jpeg.xc, data->jpeg.yc, data->jpeg.s
	);

	res_fs = f_write(&data->log.fil, str, strlen(str), &writeBytes);
	f_sync(&data->log.fil);
	data->log.log_num++;

	if (res_fs != FR_OK) {
		printf("f_write error:%d\n", res_fs);
	}

}
void print(myCansat *data) {
	
	printf("\nmode:%d V=%4ldmV F:%d log_num:%3ld /%03d/%03d.jpg\n", data->mode,data->voltage,data->flightPin, data->log.log_num, data->jpeg.dir_num, data->jpeg.file_num);
	printf("GPS:%d TIME:%02d:%02d:%02d.%03d\n", data->gps_data.mode,data->gps_data.hh, data->gps_data.mm, data->gps_data.ss, data->gps_data.ms);
	printf("dist:%9lu   N %lu E %lu \n", data->gps_data.dist, data->gps_data.latitude, data->gps_data.longitude);
	printf("X:%3d Y:%3d xc:%3lu yc:%3lu s:%3lu\n", data->compass_data.x, data->compass_data.y, data->jpeg.xc, data->jpeg.yc, data->jpeg.s);
	printf("arg:%5.3f (GPS:%5.3f compass:%5.3f)\n", data->arg, data->gps_data.arg, data->compass_data.arg);
	printf("motor( %4d , %4d ) nichrome:%d \n", data->motor_L, data->motor_R,data->nichrome);
	
}


void calibration(myCansat *data, const int n, unsigned long cnt) {

	static long x_max, y_max;
	static long x_min, y_min;

	data->motor_L = 100;
	data->motor_R = 130;

	if (cnt == 1) {
		x_max = data->compass_data.x;
		x_min = data->compass_data.x;
		y_max = data->compass_data.y;
		y_min = data->compass_data.y;
	}

	x_max = data->compass_data.x > x_max ? data->compass_data.x : x_max;
	x_min = data->compass_data.x < x_min ? data->compass_data.x : x_min;
	y_max = data->compass_data.y > y_max ? data->compass_data.y : y_max;
	y_min = data->compass_data.y < y_min ? data->compass_data.y : y_min;


	if (cnt >= n * (1000 / LOOP_TIME)) {
		data->compass_data.x_offset = (x_max + x_min) / 2;
		data->compass_data.y_offset = (y_max + y_min) / 2;
		data->motor_L = 0;
		data->motor_R = 0;
		data->mode++;
	}

	return;
}
