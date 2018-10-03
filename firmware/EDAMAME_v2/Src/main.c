
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "fatfs.h"

/* USER CODE BEGIN Includes */
#include "edamame.h"
#include "GPS.h"
#include "C1098.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;

SD_HandleTypeDef hsd;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart6;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_UART4_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART6_UART_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_I2C1_Init(void);
static void MX_SDIO_SD_Init(void);
static void MX_USART3_UART_Init(void);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
                                
                                

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
uint8_t nichrome_t;
#define I2C_BUF_SIZE  8
#define I2C_TIMEOUT 100
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	HAL_StatusTypeDef stat;
	uint8_t c;
	uint8_t gps_msg[50];
	uint8_t GPS_stat = 0;
	struct gps gps_data;
	struct compass HMC5883L;
	char    str[300];
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_UART4_Init();
  MX_ADC1_Init();
  MX_USART6_UART_Init();
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_I2C1_Init();
  MX_SDIO_SD_Init();
  MX_USART3_UART_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */
	HAL_Delay(1000);
	sprintf(str, "EDAMAME Battery = %.3f V \r\n", (float)get_battery(&hadc1) / 1000);
	HAL_UART_Transmit(&huart3, str, strlen(str), 0x00FF);

	//PWM初期化
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, 0);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, 0);
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 0);
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, 0);

	//I2C(地磁気)初期化
	int tx_buf[I2C_BUF_SIZE] = { 0x00 }, rx_buf[I2C_BUF_SIZE] = { 0x00 };
	uint8_t pData;
	pData = 0x50; HAL_I2C_Mem_Write(&hi2c1, HMC5883L_WRITE, HMC5883L_CONFIG_A, 1, &pData, 1, 100);
	pData = 0x20; HAL_I2C_Mem_Write(&hi2c1, HMC5883L_WRITE, HMC5883L_CONFIG_B, 1, &pData, 1, 100);
	pData = 0x00; HAL_I2C_Mem_Write(&hi2c1, HMC5883L_WRITE, HMC5883L_MODE, 1, &pData, 1, 100);


	//カメラ初期化
	int ce = camera_inital_HiSpeed(&huart4, QVGA, 460800);
	sprintf(str, "\r\n\r\ncamera_Init %d\r\n", ce);
	HAL_UART_Transmit(&huart3, str, strlen(str), 0xFFFF);


	//SDカード初期化
	FRESULT res_fs;
	FATFS fs;
	DIR dir;
	char buf[128];
	int ret;
	const int MAX_DIR = 100;
	unsigned int testBytes;

	uint32_t data_size;
	uint8_t path[13] = "test.txt";

	//SDカードを初期化できるまで待つ
	do {
		res_fs = f_mount(&fs, "", 1);
		if (res_fs != FR_OK) {
			sprintf(str, "f_mount() : error %u\n", res_fs);
			HAL_UART_Transmit(&huart3, str, strlen(str), 0x00FF);
		}

		res_fs = f_opendir(&dir, "");
		if (res_fs != FR_OK) {
			sprintf(str, "f_opendir() : error %u\n", res_fs);
			HAL_UART_Transmit(&huart3, str, strlen(str), 0x00FF);
		}
		HAL_Delay(800);
	} while (res_fs != FR_OK);

	//ディレクトリの作成
	unsigned int dir_num = 0;
	for (dir_num = 0; dir_num < MAX_DIR; dir_num++) {
		sprintf(str, "/edmm%d", dir_num);
		res_fs = f_mkdir(str);
		if (res_fs == FR_OK) {
			sprintf(str, "f_mkdir() : edmm%d\r\n", dir_num);
			HAL_UART_Transmit(&huart3, str, strlen(str), 0x00FF);
			break;
		} else {
			sprintf(str, "f_mkdir() : edmm%d error %d\r\n", dir_num, res_fs);
			HAL_UART_Transmit(&huart3, str, strlen(str), 0x00FF);
		}
	}

	//画像記録用のJPEGファイル名
	JDEC jdec;        /* Decompression object */
	JRESULT jpeg_res;      /* Result code of TJpgDec API */
	IODEV jpeg;      /* User defined device identifier */

	unsigned int file_num = 0;
	unsigned int log_num = 0;//ログの番号
	FIL fil_jpeg;


	//制御履歴記録用
	FIL fil_log;
	sprintf(path, "/edmm%d/log.csv", dir_num);
	res_fs = f_open(&fil_log, (char*)path, FA_WRITE | FA_CREATE_ALWAYS);

	sprintf(str, "log_num,mode,hh,mm,ss,latitude,longitude,GPS_error,Voltage,HMC5883L.x,HMC5883L.y,HMC5883L.arg,dist,arg,GPS_arg,motorL,motorR,,file_num,cx,cy,s\n");
	ret = f_puts(str, &fil_log);
	if (ret == EOF) {
		sprintf(str, "f_puts() : error\n\n");
		HAL_UART_Transmit(&huart3, str, strlen(str), 0x00FF);
	}
	f_sync(&fil_log);



	//容量不足の時の例外処理を後でここに書く

	//

  /*
	 while (1)  {

	 sprintf(str, "%.3f V ", (float)get_battery(&hadc1) / 1000);
	  HAL_UART_Transmit(&huart3, str, strlen(str), 0x00FF);
	  //撮影
	  while(snap_shot(&huart4) != CAMERA_OK);

	  //ファイルを開く
	  sprintf(path,"/edmm%d/%d.jpg",dir_num,file_num);
	  res_fs = f_open(&fil_jpeg, (char*)path, FA_WRITE | FA_CREATE_ALWAYS);

	  sprintf(str,"/edmm%d/%d.jpg f_open:%d ",dir_num,file_num,res_fs);
	  HAL_UART_Transmit( &huart3, str, strlen( str ), 0xFFFF );

	  //写真を転送
	  int e = get_picture(&huart4,&jpeg.jpeg_data,(uint32_t)40000,&data_size);
	  sprintf(str,"get_picture:%d %dB", e,data_size);
	  HAL_UART_Transmit( &huart3, str, strlen( str ), 0xFFFF );

	  //SDカードに書き込み
	  res_fs = f_write(&fil_jpeg, &jpeg.jpeg_data, data_size, &testBytes);
	  sprintf(str," f_write:%d %dB   ",res_fs,testBytes);
	  HAL_UART_Transmit( &huart3, str, strlen( str ), 0xFFFF );

	  //ファイルを保存して閉じる
	  res_fs = f_close(&fil_jpeg);
	  file_num++;

	  //JPEGデコード
	  uint8_t work[3100];
	  jpeg.jpeg_data_seek =0;
	  jpeg_res = jd_prepare(&jdec, in_func, work, 3100, &jpeg);
	  sprintf(str,"jd_prepare:%d", jpeg_res);
	  HAL_UART_Transmit( &huart3, str, strlen( str ), 0xFFFF );
	  memset(jpeg.RED_bool, 0, sizeof(jpeg.RED_bool));

	  if (jpeg_res == JDR_OK && data_size%100 != 0) {
		sprintf(str," %ux%upx ", jdec.width, jdec.height);
		HAL_UART_Transmit( &huart3, str, strlen( str ), 0xFFFF );
		//デコード開始
		jpeg_res = jd_decomp(&jdec, out_func, 0);
		sprintf(str,"jd_decomp:%d ", jpeg_res);
		HAL_UART_Transmit( &huart3, str, strlen( str ), 0xFFFF );
		if (jpeg_res == JDR_OK) {

		  //重心計算
		  UINT xc = 0, yc = 0, s = 0;
		  for (UINT h = 0; h < jdec.height; h++) {
			for (UINT w = 0; w < jdec.width; w++) {
			  if ((jpeg.RED_bool[h][w / 8] & (0b10000000 >> (w % 8))) != 0) {
				xc += w;
				yc += h;
				s++;
			  }
			}
		  }
		  if (s != 0) {
			xc = xc / s;
			yc = yc / s;
			sprintf(str,"(x,y)=(%d,%d)  s=%d\n", xc,yc,s);
			HAL_UART_Transmit( &huart3, str, strlen( str ), 0xFFFF );

		  } else {
			sprintf(str,"(x,y) = Null \n");
			HAL_UART_Transmit( &huart3, str, strlen( str ), 0xFFFF );
		  }

		}
		//JPEGのデコードに失敗した場合
		else {
		  sprintf(str,"Failed to decompress: rc=%d  \n", jpeg_res);
		  HAL_UART_Transmit( &huart3, str, strlen( str ), 0xFFFF );
		}
	  } else{
		sprintf(str,"error\n", jpeg_res);
		HAL_UART_Transmit( &huart3, str, strlen( str ), 0xFFFF );
	  }

	}
  */
  /*
	//GPS直接出力
	  while(1){
		  stat = HAL_UART_Receive(&huart6, &c, 1, 1);
			  if (stat == HAL_OK) {
			  stat = HAL_UART_Transmit(&huart3, &c, 1, -1);
		  }
	  }
  */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	//フライトピンが抜けてなければ

	//フライトピンが抜けるまで待つ
	int n = 0, e;
	nichrome_t =0;
	if (HAL_GPIO_ReadPin(FLIGHT_PIN_GPIO_Port, FLIGHT_PIN_Pin) == GPIO_PIN_RESET){
		while (1) {
			HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);//動作確認用

			//電圧の取得
			double v = (double)get_battery(&hadc1) / 1000.0;
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
			if (v < LOW_BATTERY)HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

			//GPSの座標取得
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
			e = get_gps(&gps_data, &huart6, 1000);
			if (e == 0)HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);//取得できてたらLED点滅

			sprintf(str, "%d,%d,%02d,%02d,%02d,%9.6f,%10.6f,%x,%.3lf, , , , , , ,%d,%d,,%d,%d,%d,%d\n",
				log_num,
				3,
				gps_data.hh,
				gps_data.mm,
				gps_data.ss,
				gps_data.latitude,
				gps_data.longitude,
				e,
				v,
				0,
				0,
				file_num,
				-1,
				-1,
				-1);

			HAL_UART_Transmit(&huart3, &str, strlen(str), 0x00FF);
			HAL_UART_Transmit(&huart1, &str, strlen(str), 0x00FF);
			HAL_UART_Transmit(&huart1, "\r", 1, 0x00FF);
			ret = f_puts(str, &fil_log);
			if (ret == EOF) {
				sprintf(str, "f_puts() : error\n\n");
				HAL_UART_Transmit(&huart3, str, strlen(str), 0x00FF);
			}
			f_sync(&fil_log);
			log_num++;

			//フライトピンが抜けたら
			if (HAL_GPIO_ReadPin(FLIGHT_PIN_GPIO_Port, FLIGHT_PIN_Pin) == GPIO_PIN_SET){
				nichrome_t=0;
				break;
			}
		}

		//着地するまで待機
		nichrome_t=0;
		//int nt = 0;
		while(1){
			HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);//動作確認用

			//電圧の取得
			double v = (double)get_battery(&hadc1) / 1000.0;
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
			if (v < LOW_BATTERY)HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

			//GPSの座標取得
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
			e = get_gps(&gps_data, &huart6, 1000);
			if (e == 0)HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);//取得できてたらLED点滅

			sprintf(str, "%d,%d,%02d,%02d,%02d,%9.6f,%10.6f,%x,%.3lf, , , , , , ,%d,%d,,%d,%d,%d,%d\n",
				log_num,
				2,
				gps_data.hh,
				gps_data.mm,
				gps_data.ss,
				gps_data.latitude,
				gps_data.longitude,
				e,
				v,
				0,
				0,
				file_num,
				-1,
				-1,
				-1);

			HAL_UART_Transmit(&huart3, &str, strlen(str), 0x00FF);
			HAL_UART_Transmit(&huart1, &str, strlen(str), 0x00FF);
			HAL_UART_Transmit(&huart1, "\r", 1, 0x00FF);
			ret = f_puts(str, &fil_log);
			if (ret == EOF) {
				sprintf(str, "f_puts() : error\n\n");
				HAL_UART_Transmit(&huart3, str, strlen(str), 0x00FF);
			}
			f_sync(&fil_log);
			log_num++;
			nichrome_t ++;
			if(nichrome_t >=20)break;
		}
		//パラシュート切り離し
			HAL_GPIO_WritePin(NICHROME_GPIO_Port, NICHROME_Pin, GPIO_PIN_SET);
			HAL_Delay(1000);
			HAL_GPIO_WritePin(NICHROME_GPIO_Port, NICHROME_Pin, GPIO_PIN_RESET);
			HAL_Delay(1000);
	}


	//地磁気キャリブレーション
	compass_calibrate(&HMC5883L, &hi2c1, &htim2, &htim3, &huart3, &hadc1, 2000);

	n = 0;//ゴール判定に使うやつ

	int mode = 1;//モード 0 GPS 1 画像認識
	int xp; //画像検出時の1つ前の回転状態
	int dc[5];//重心の変化量(過去5回)
	int xc_p = 0, yc_p = 0;//１つ前の重心
	while (1) {
		e = 1;
		UINT xc = 0, yc = 0, s = 0;
		int motorL, motorR;
		HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);//動作確認用

		//電圧の取得
		double v = (double)get_battery(&hadc1) / 1000.0;
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
		if (v < LOW_BATTERY)HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
		//v = 1.235;

		//GPSの座標取得
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
		e = get_gps(&gps_data, &huart6, 1000);
		if (e == 0)HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);//取得できてたらLED点滅

		//地磁気取得
		get_compass(&HMC5883L, &hi2c1);

		//----------------------画像取得--------------------------
		while (snap_shot(&huart4) != CAMERA_OK);

		//ファイルを開く
		sprintf(path, "/edmm%d/%d.jpg", dir_num, file_num);
		res_fs = f_open(&fil_jpeg, (char*)path, FA_WRITE | FA_CREATE_ALWAYS);

		get_picture(&huart4, &jpeg.jpeg_data, (uint32_t)40000, &data_size);//写真を転送
		res_fs = f_write(&fil_jpeg, &jpeg.jpeg_data, data_size, &testBytes);//SDカードに書き込み

		//ファイルを保存して閉じる
		res_fs = f_close(&fil_jpeg);
		//----------------------画像取得--------------------------

		//目標との距離と角度を求める
		double dx = GOAL_LATITUDE - gps_data.latitude;
		double dy = GOAL_LONGITUDE - gps_data.longitude;
		double arg = atan2(dy, dx);
		double dist = dx * dx + dy * dy;

		const double pi = 3.1415926535;
		double a = HMC5883L.arg + MAGNETIC_DECLINATION - arg;

		while (a > 1.0*pi) a -= 2 * pi;
		while (a < -1.0*pi) a += 2 * pi;


		//指定の方位に向かって進む
		if (mode == 0) {	//ゴールまでの距離が遠い場合 GPS誘導
			if(dist < GOAL_ALEA2){
				mode =1;
			}else{
				if (e == 0) {
					if (a > 1.6) {
						motorL = 0;
						motorR = 1024;
					}
					if (a < 1.60 && a >= 0.18) {
						motorL = 700;
						motorR = 1024;
					}
					if (a < 0.18 && a >= -0.18) {
						motorL = 1024;
						motorR = 1024;
					}
					if (a < -0.18 && a >= -1.60) {
						motorL = 1024;
						motorR = 700;
					}
					if (a <= -1.6) {
						motorL = 1024;
						motorR = 0;
					}
				} else {
					motorL = 800;
					motorR = 1024;
				}
			}
			motor(&htim2, &htim3, motorL, motorR);
		}
		if(mode == 1){	//ゴールまでの距離が近い場合	画像認識
			mode = 1;

			//JPEGデコード

			motor(&htim2, &htim3, 0, 0);

			uint8_t work[3100];
			jpeg.jpeg_data_seek = 0;
			jpeg_res = jd_prepare(&jdec, in_func, work, 3100, &jpeg);
			memset(jpeg.RED_bool, 0, sizeof(jpeg.RED_bool));

			if (jpeg_res == JDR_OK && data_size % 100 != 0) {
				//デコード開始
				jpeg_res = jd_decomp(&jdec, out_func, 0);
				if (jpeg_res == JDR_OK) {

					//重心計算
					//UINT xc = 0, yc = 0, s = 0;
					for (UINT h = 0; h < jdec.height; h++) {
						for (UINT w = 0; w < jdec.width; w++) {
							if ((jpeg.RED_bool[h][w / 8] & (0b10000000 >> (w % 8))) != 0) {
								xc += w;
								yc += h;
								s++;
							}
						}
					}
					if (s > 0 ) {	//カラーコーンを見つけた場合
						xc = xc / s;
						yc = yc / s;
						if (s > SQUARE) {
							//右
							if (xc < 60) {
								xp = 1;
								motorL = 900;
								motorR = 300;
							}
							//やや右
							if (xc >= 60 && xc < 120) {
								xp = 1;
								motorL = 900;
								motorR = 600;
							}
							//中央
							if (xc >= 120 && xc < 200) {
								xp = 0;
								motorL = 900;
								motorR = 900;
							}
							//やや左
							if (xc >= 200 && xc < 260) {
								xp = -1;
								motorL = 600;
								motorR = 900;
							}
							//左
							if (xc >= 260) {
								xp = -1;
								motorL = 300;
								motorR = 900;
							}

							//ゴール判定
							dc[4] = dc[3];
							dc[3] = dc[2];
							dc[2] = dc[1];
							dc[1] = dc[0];
							dc[0] = (xc_p - xc)*(xc_p - xc) + (yc_p - yc)*(yc_p - yc);
							xc_p = xc;
							yc_p = yc;
							if (n > 5) {
								int dc_sum = dc[0] + dc[1] + dc[2] + dc[3] + dc[4];
								if (dc_sum < GOAL)break;
							}
							n++;

							
						} else {
							if(xp == 1){
								motorR = 400;
								motorL = 1000;
							}else{
								motorR = 1000;
								motorL = 400;
							}
						}

					} else {	//無かった場合
						xc = -1;
						yc = -1;
						if (xp == 1) {
							motorR = 400;
							motorL = 700;
						} else {
							motorR = 700;
							motorL = 400;
						}
					}

				}
				//JPEGのデコードに失敗した場合
				else {
					motorL = 0;
					motorR = 0;
				}
			} else {
				motorL = 0;
				motorR = 0;
			}
			motor(&htim2, &htim3, motorL, motorR);
		}

		//motor(&htim2, &htim3, motorL, motorR);

/*
		//いろいろ表示
		//GPSエラーコード
		sprintf(str, "%x  ", e);
		HAL_UART_Transmit(&huart3, &str, strlen(str), 0x00FF);
		//電圧
		sprintf(str, "%.3lfV ", v);
		HAL_UART_Transmit(&huart3, &str, strlen(str), 0x00FF);
		//GPS時刻・座標
		sprintf(str, " %02d:%02d:%02d %9.6f,%10.6f ", gps_data.hh, gps_data.mm, gps_data.ss, gps_data.latitude, gps_data.longitude);
		HAL_UART_Transmit(&huart3, &str, strlen(str), 0x00FF);
		//地磁気・方位角
		sprintf(str, "X:%+5d Y:%+5d  %+f  ", HMC5883L.x, HMC5883L.y, HMC5883L.arg);
		HAL_UART_Transmit(&huart3, &str, strlen(str), 0x00FF);
		//距離・進路
		sprintf(str, "dist %e Darg%+f  GPS_arg %+f ", dist, a, arg);
		HAL_UART_Transmit(&huart3, &str, strlen(str), 0x00FF);
		//画像
		HAL_UART_Transmit(&huart3, &path, strlen(path), 0x00FF);
		sprintf(str, "\n", dist, a, arg);
		HAL_UART_Transmit(&huart3, &str, strlen(str), 0x00FF);
*/
		//sprintf(str, "%.3lf,0,%x,%9.6f,%10.6f,%e,%f,%f,%f\n", v, e, gps_data.latitude, gps_data.longitude, dist, arg, HMC5883L.arg, a);
		//HAL_UART_Transmit(&huart1, &str, strlen(str), 0x00FF);

		sprintf(str, "%d,%d,%02d,%02d,%02d,%9.6f,%10.6f,%x,%.3lf,%+5d,%+5d,%+f,%e,%+f,%+f,%d,%d,,%d,%d,%d,%d\n",
			log_num,
			mode,
			gps_data.hh,
			gps_data.mm,
			gps_data.ss,
			gps_data.latitude,
			gps_data.longitude,
			e,
			v,
			HMC5883L.x,
			HMC5883L.y,
			HMC5883L.arg,
			dist,
			a,
			arg,
			motorL,
			motorR,
			file_num,
			xc,
			yc,
			s);

		HAL_UART_Transmit(&huart3, &str, strlen(str), 0x00FF);
		HAL_UART_Transmit(&huart1, &str, strlen(str), 0x00FF);
		HAL_UART_Transmit(&huart1, "\r", 1, 0x00FF);
		ret = f_puts(str, &fil_log);
		if (ret == EOF) {
			sprintf(str, "f_puts() : error\n\n");
			HAL_UART_Transmit(&huart3, str, strlen(str), 0x00FF);
		}
		f_sync(&fil_log);
		log_num++;
		file_num++;

	}
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	motor(&htim2, &htim3, -1024, -1024);
	HAL_Delay(500);
	motor(&htim2, &htim3, 1024, 1024);
	HAL_Delay(1000);
	motor(&htim2, &htim3, 0, 0);
	HAL_Delay(1000);
	motor(&htim2, &htim3, 1024, 1024);
	HAL_Delay(200);
	motor(&htim2, &htim3, 0, 0);
	HAL_Delay(200);
	motor(&htim2, &htim3, 1024, 1024);
	HAL_Delay(200);
	motor(&htim2, &htim3, 0, 0);
	HAL_Delay(200);
	motor(&htim2, &htim3, 1024, 1024);
	HAL_Delay(200);
	motor(&htim2, &htim3, 0, 0);
	while (1) {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
		HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
		HAL_Delay(500);
		HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
		HAL_Delay(500);

	}
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 240;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 15;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SDIO|RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48CLKSOURCE_PLLQ;
  PeriphClkInitStruct.SdioClockSelection = RCC_SDIOCLKSOURCE_CLK48;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* ADC1 init function */
static void MX_ADC1_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
    */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* I2C1 init function */
static void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* SDIO init function */
static void MX_SDIO_SD_Init(void)
{

  hsd.Instance = SDIO;
  hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
  hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
  hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
  hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
  hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd.Init.ClockDiv = 2;

}

/* TIM2 init function */
static void MX_TIM2_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1023;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_TIM_MspPostInit(&htim2);

}

/* TIM3 init function */
static void MX_TIM3_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 1023;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_TIM_MspPostInit(&htim3);

}

/* UART4 init function */
static void MX_UART4_Init(void)
{

  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART3 init function */
static void MX_USART3_UART_Init(void)
{

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART6 init function */
static void MX_USART6_UART_Init(void)
{

  huart6.Instance = USART6;
  huart6.Init.BaudRate = 9600;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LED1_Pin|LED2_Pin|LED3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(NICHROME_GPIO_Port, NICHROME_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LED1_Pin LED2_Pin LED3_Pin */
  GPIO_InitStruct.Pin = LED1_Pin|LED2_Pin|LED3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : NICHROME_Pin */
  GPIO_InitStruct.Pin = NICHROME_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(NICHROME_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : FLIGHT_PIN_Pin */
  GPIO_InitStruct.Pin = FLIGHT_PIN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(FLIGHT_PIN_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
