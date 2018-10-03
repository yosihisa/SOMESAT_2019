#include "edamame.h"

//電池電圧を返すやつ
int get_battery(ADC_HandleTypeDef* hadc){
	float vf;
	int v;
	HAL_ADC_Start(hadc);
	HAL_ADC_PollForConversion(hadc, 100);
	v = HAL_ADC_GetValue(hadc);
	HAL_ADC_Stop(hadc);
	v = v*10000 / 3101;
	return v;
}

//GPS受信するやつ
int get_gps(struct gps *gps_data, UART_HandleTypeDef *huart, int TIMEOUT) {
	gps_data -> latitude = 0.000;
	gps_data ->  longitude = 0.000;
	gps_data ->  hh = 0;
	gps_data -> mm = 0;
	gps_data -> ss = 0;
	int timeout =0;
	while(1){
		HAL_StatusTypeDef stat;
		uint8_t c;
		uint8_t gps_msg[50];
		uint8_t GPS_stat =0;
		int dd ,mm,mmmm,ddd,dddd;
		
		//GPSから時刻と座標を取得
		GPS_stat =0;
		while(HAL_UART_Receive(huart, &c, 1, 1) != HAL_OK){
			timeout++;
			if(timeout > TIMEOUT)return -1;
		}
		
		//GPSのセンテンスの始まりを探してデータタイプの種類を代入
		if(c == '$'){			
			for(int i =0 ; i<6 ; i++){
				while(HAL_UART_Receive(huart, &c, 1, 1) != HAL_OK){
					timeout++;
					if(timeout > TIMEOUT)return -1;
				}
				gps_msg[i] = c;
			}

			//時刻・座標の取得計算
			if(strncmp(gps_msg,"GPGGA",5) == 0){
				for(int i =0 ; c !='\n' ; i++){
					while(HAL_UART_Receive(huart, &c, 1, 1) != HAL_OK){
						timeout++;
						if(timeout > TIMEOUT)return -1;
					}
					gps_msg[i]=c;
					//if(c < 0x20)i--;					
					if (stat == HAL_OK) {

						//1行取得し終えたら
						if(c =='\n'){
							gps_data->hh = 10*(gps_msg[0]-48) +(gps_msg[1]-48);
							gps_data->mm = 10*(gps_msg[2]-48) +(gps_msg[3]-48);
							gps_data->ss = 10*(gps_msg[4]-48) +(gps_msg[5]-48);
							if(gps_msg[6] != '.')GPS_stat |= 0b0000001;
							if(gps_data->hh < 0 || gps_data->hh >24)GPS_stat = 0b0000001;                
							
							//測位できていたら
							if(i >40){
								int n;
								dd=0;
								for(n =5;n < i;n++){
									if(gps_msg[n] ==','){
										dd  = 10*(gps_msg[n+1]-48) +(gps_msg[n+2]-48);
										mm  = 10*(gps_msg[n+3]-48) +(gps_msg[n+4]-48);
										mmmm= 1000*(gps_msg[n+6]-48) +100*(gps_msg[n+7]-48)+10*(gps_msg[n+8]-48)+(gps_msg[n+9]-48);
										
										gps_data->latitude = dd + (((float)mm + (float)mmmm/10000)/60 );
										if(gps_msg[n+10] != ',' || gps_msg[n+11] != 'N')GPS_stat |= 0b0000010;
										break;
									}
								}
								for(n=n+1 ;n < i;n++){
									if(gps_msg[n] =='N'){
									break;
									}
								}
								for(n = n+1 ;n < i;n++){
									if(gps_msg[n] ==','){
										ddd  =100*(gps_msg[n+1]-48)+ 10*(gps_msg[n+2]-48) +(gps_msg[n+3]-48);
										mm  = 10*(gps_msg[n+4]-48) +(gps_msg[n+5]-48);
										mmmm= 1000*(gps_msg[n+7]-48) +100*(gps_msg[n+8]-48)+10*(gps_msg[n+9]-48)+(gps_msg[n+10]-48);
										gps_data->longitude = ddd + (((float)mm + (float)mmmm/10000)/60 );
										if(gps_msg[n+11] != ',' || gps_msg[n+12] != 'E')GPS_stat |= 0b0000100;
										break;
									}
								}
								for(n=n+1 ;n < i;n++){
									if(gps_msg[n] =='E'){
										break;
									}
								}
							}
							//測位できていなかったら
							else   GPS_stat |= 0b0010000;
							return GPS_stat ;
						}
					}
				}
			}
		}
    }
}

//地磁気キャリブレーションするやつ
void compass_calibrate(struct compass *Compass, I2C_HandleTypeDef *hi2c, TIM_HandleTypeDef *htimL, TIM_HandleTypeDef *htimR, UART_HandleTypeDef *huart, ADC_HandleTypeDef* hadc, int cycle) {
	int16_t x,y,x_max,x_min,y_max,y_min;
	uint8_t Data[14];  	
	motor(htimL,htimR,512,1024);

	//最小値・最大値を最初の測定値で初期化
	HAL_I2C_Mem_Read(hi2c	,HMC5883L_READ	,0x03	,1	,&Data,6,100);
	x = Data[0]<<8 ;x |= Data[1];
	y = Data[4]<<8 ;y |= Data[5];
	x_max = x ; x_min = x ;
	y_max = y ; y_min = y ;
	for (int i = 0; i < cycle; i++) {
		uint8_t Data[14];  
    	HAL_I2C_Mem_Read(hi2c	,HMC5883L_READ	,0x03	,1	,&Data,6,100);
		x = Data[0]<<8 ;x |= Data[1];
    	y = Data[4]<<8 ;y |= Data[5];
		if(x > x_max)x_max =x;
		if(x < x_min)x_min =x;
		if(y > y_max)y_max =y;
		if(y < y_min)y_min =y;
		HAL_Delay(10);

	}
	Compass -> x_offset = (x_max + x_min)/2;
	Compass -> y_offset = (y_max + y_min)/2;
	motor(htimL,htimR,0,0);
	return;
}

//地磁気の値を読む
void get_compass(struct compass *Compass, I2C_HandleTypeDef *hi2c){
	int16_t x,y;
	uint8_t Data[14];  	
	HAL_I2C_Mem_Read(hi2c	,HMC5883L_READ	,0x03	,1	,&Data,6,100);
	x = Data[0]<<8 ;x |= Data[1];
	y = Data[4]<<8 ;y |= Data[5];
	Compass -> x = (int)x - Compass->x_offset;
	Compass -> y = (int)y - Compass->y_offset;
	Compass -> arg = -1.0 * atan2((double)Compass -> y,(double)Compass -> x);
	return;
}
//モーター速度変更
void motor(TIM_HandleTypeDef *htimL ,TIM_HandleTypeDef *htimR,int L ,int R){
	if(L>0){
		__HAL_TIM_SetCompare(htimL,TIM_CHANNEL_1,L);
		__HAL_TIM_SetCompare(htimL,TIM_CHANNEL_2,0);
	}
	else{
		__HAL_TIM_SetCompare(htimL,TIM_CHANNEL_1,0);
		__HAL_TIM_SetCompare(htimL,TIM_CHANNEL_2,-L);
	}

	if(R>0){
		__HAL_TIM_SetCompare(htimR,TIM_CHANNEL_1,0);
		__HAL_TIM_SetCompare(htimR,TIM_CHANNEL_2,R);
	}
	else{
		__HAL_TIM_SetCompare(htimR,TIM_CHANNEL_1,-R);
		__HAL_TIM_SetCompare(htimR,TIM_CHANNEL_2,0);
	}
	return;
}


//JPEGデコーダ入力関数
uint32_t in_func(JDEC* jd, uint8_t* buff, uint32_t nbyte){
	IODEV *dev = (IODEV*)jd->device;   
	if (buff) {
		uint32_t e;
		if (dev->jpeg_data_seek+ nbyte <= dev->jpeg_data_size) {
			e = nbyte;
		} else {
			e = dev->jpeg_data_seek+ nbyte - dev->jpeg_data_size;
		}
		for (int i = 0; i < e; i++) {
			*(buff + i) = dev->jpeg_data[dev->jpeg_data_seek + i];
		}
		dev->jpeg_data_seek += e;
		
		return e;

	} else {

		if (dev->jpeg_data_seek + nbyte <= dev->jpeg_data_size) {
			dev->jpeg_data_seek += nbyte;
			return nbyte;

		} else {
			return 0;
		}
	}
}


//JPEGデコーダ出力関数
uint32_t out_func(JDEC* jd, void* bitmap, JRECT* rect)
{
    IODEV *dev = (IODEV*)jd->device;       
	uint8_t *src;
	uint32_t width, height,bitshift;
	uint8_t r, g, b;
	double h, s, v; //H:色相 S:彩度 V:明度

	src = (BYTE*)bitmap;
	for (height = 0; height < (rect->bottom - rect->top + 1); height++) {
		for (width = 0; width < (rect->right - rect->left + 1); width++) {

			//バッファのRGB88から各色を抽出
			r = (BYTE)*(src + 3 * (height*(rect->right - rect->left + 1) + width));
			g = (BYTE)*(src + 3 * (height*(rect->right - rect->left + 1) + width) + 1);
			b = (BYTE)*(src + 3 * (height*(rect->right - rect->left + 1) + width) + 2);

			//HSV変換
			double MAX = max((max(r, g)), b);
			double MIN = min((min(r, g)), b);
			v = MAX / 256 * 100;

			if (MAX == MIN) {
				h = 0;
				s = 0;
			} else {
				if (MAX == r) h = 60.0*(g - b) / (MAX - MIN) + 0;
				else if (MAX == g) h = 60.0*(b - r) / (MAX - MIN) + 120.0;
				else if (MAX == b) h = 60.0*(r - g) / (MAX - MIN) + 240.0;

				if (h > 360.0) h = h - 360.0;
				else if (h < 0) h = h + 360.0;
				s = (MAX - MIN) / MAX * 100.0;
			}

			if (h > 360.0)h -= 360;

			if ((h >= H_MIN_1 && h <= H_MAX_1)|| (h >= H_MIN_2 && h <= H_MAX_2)) {
				if ((s >= S_MIN && s <= S_MAX) && (v >= V_MIN && v <= V_MAX)) {
					bitshift = (rect->left + width) % 8;
					dev->RED_bool[rect->top + height][(rect->left + width) / 8] |= (0b10000000 >> bitshift);

				}
			}
		}
	}
	return 1;
}

