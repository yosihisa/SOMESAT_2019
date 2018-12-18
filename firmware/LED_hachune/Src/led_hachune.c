#include "main.h"
#include "stm32f0xx_hal.h"
#include "led_hachune.h"


//システムクロック 40MHz
void led0_T0(){
	LED0_GPIO_Port->BSRR = (uint32_t)LED0_Pin;
	asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");

	LED0_GPIO_Port->BRR = (uint32_t)LED0_Pin;
	asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");
	asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");
}

void led0_T1(){
	LED0_GPIO_Port->BSRR = (uint32_t)LED0_Pin;
	asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");
	asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");

	LED0_GPIO_Port->BRR = (uint32_t)LED0_Pin;
	asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");
}

void led0_detasend(rgb *data){
	uint32_t data24 = ((data->grean) <<16) + ((data->red) << 8) + ((data->blue));
	uint8_t buff[24] = {0};

	for(uint8_t i =0;i<24;i++){
		buff[i] = (  data24 & (1 << (23-i) )  ) ?  1 : 0 ;
	}

	buff[0] ? led0_T1() : led0_T0();
	buff[1] ? led0_T1() : led0_T0();
	buff[2] ? led0_T1() : led0_T0();
	buff[3] ? led0_T1() : led0_T0();
	buff[4] ? led0_T1() : led0_T0();
	buff[5] ? led0_T1() : led0_T0();
	buff[6] ? led0_T1() : led0_T0();
	buff[7] ? led0_T1() : led0_T0();

	buff[8] ? led0_T1() : led0_T0();
	buff[9] ? led0_T1() : led0_T0();
	buff[10] ? led0_T1() : led0_T0();
	buff[11] ? led0_T1() : led0_T0();
	buff[12] ? led0_T1() : led0_T0();
	buff[13] ? led0_T1() : led0_T0();
	buff[14] ? led0_T1() : led0_T0();
	buff[15] ? led0_T1() : led0_T0();

	buff[16] ? led0_T1() : led0_T0();
	buff[17] ? led0_T1() : led0_T0();
	buff[18] ? led0_T1() : led0_T0();
	buff[19] ? led0_T1() : led0_T0();
	buff[20] ? led0_T1() : led0_T0();
	buff[21] ? led0_T1() : led0_T0();
	buff[22] ? led0_T1() : led0_T0();
	buff[23] ? led0_T1() : led0_T0();

}

void led1_T0(){
	LED0_GPIO_Port->BSRR = (uint32_t)LED1_Pin;
	asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");

	LED0_GPIO_Port->BRR = (uint32_t)LED1_Pin;
	asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");
	asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");
}

void led1_T1(){
	LED0_GPIO_Port->BSRR = (uint32_t)LED1_Pin;
	asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");
	asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");

	LED0_GPIO_Port->BRR = (uint32_t)LED1_Pin;
	asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");
}

void led1_detasend(rgb *data){
	uint32_t data24 = ((data->grean) <<16) + ((data->red) << 8) + ((data->blue));
	uint8_t buff[24] = {0};

	for(uint8_t i =0;i<24;i++){
		buff[i] = (  data24 & (1 << (23-i) )  ) ?  1 : 0 ;
	}

	buff[0] ? led1_T1() : led1_T0();
	buff[1] ? led1_T1() : led1_T0();
	buff[2] ? led1_T1() : led1_T0();
	buff[3] ? led1_T1() : led1_T0();
	buff[4] ? led1_T1() : led1_T0();
	buff[5] ? led1_T1() : led1_T0();
	buff[6] ? led1_T1() : led1_T0();
	buff[7] ? led1_T1() : led1_T0();

	buff[8] ? led1_T1() : led1_T0();
	buff[9] ? led1_T1() : led1_T0();
	buff[10] ? led1_T1() : led1_T0();
	buff[11] ? led1_T1() : led1_T0();
	buff[12] ? led1_T1() : led1_T0();
	buff[13] ? led1_T1() : led1_T0();
	buff[14] ? led1_T1() : led1_T0();
	buff[15] ? led1_T1() : led1_T0();

	buff[16] ? led1_T1() : led1_T0();
	buff[17] ? led1_T1() : led1_T0();
	buff[18] ? led1_T1() : led1_T0();
	buff[19] ? led1_T1() : led1_T0();
	buff[20] ? led1_T1() : led1_T0();
	buff[21] ? led1_T1() : led1_T0();
	buff[22] ? led1_T1() : led1_T0();
	buff[23] ? led1_T1() : led1_T0();

}

void led2_T0(){
	LED0_GPIO_Port->BSRR = (uint32_t)LED2_Pin;
	asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");

	LED0_GPIO_Port->BRR = (uint32_t)LED2_Pin;
	asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");
	asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");
}

void led2_T1(){
	LED0_GPIO_Port->BSRR = (uint32_t)LED2_Pin;
	asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");
	asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");

	LED0_GPIO_Port->BRR = (uint32_t)LED2_Pin;
	asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");
}

void led2_detasend(rgb *data){
	uint32_t data24 = ((data->grean) <<16) + ((data->red) << 8) + ((data->blue));
	uint8_t buff[24] = {0};

	for(uint8_t i =0;i<24;i++){
		buff[i] = (  data24 & (1 << (23-i) )  ) ?  1 : 0 ;
	}

	buff[0] ? led2_T1() : led2_T0();
	buff[1] ? led2_T1() : led2_T0();
	buff[2] ? led2_T1() : led2_T0();
	buff[3] ? led2_T1() : led2_T0();
	buff[4] ? led2_T1() : led2_T0();
	buff[5] ? led2_T1() : led2_T0();
	buff[6] ? led2_T1() : led2_T0();
	buff[7] ? led2_T1() : led2_T0();

	buff[8] ? led2_T1() : led2_T0();
	buff[9] ? led2_T1() : led2_T0();
	buff[10] ? led2_T1() : led2_T0();
	buff[11] ? led2_T1() : led2_T0();
	buff[12] ? led2_T1() : led2_T0();
	buff[13] ? led2_T1() : led2_T0();
	buff[14] ? led2_T1() : led2_T0();
	buff[15] ? led2_T1() : led2_T0();

	buff[16] ? led2_T1() : led2_T0();
	buff[17] ? led2_T1() : led2_T0();
	buff[18] ? led2_T1() : led2_T0();
	buff[19] ? led2_T1() : led2_T0();
	buff[20] ? led2_T1() : led2_T0();
	buff[21] ? led2_T1() : led2_T0();
	buff[22] ? led2_T1() : led2_T0();
	buff[23] ? led2_T1() : led2_T0();

}

void led3_T0(){
	LED0_GPIO_Port->BSRR = (uint32_t)LED3_Pin;
	asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");

	LED0_GPIO_Port->BRR = (uint32_t)LED3_Pin;
	asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");
	asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");
}

void led3_T1(){
	LED0_GPIO_Port->BSRR = (uint32_t)LED3_Pin;
	asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");
	asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");

	LED0_GPIO_Port->BRR = (uint32_t)LED3_Pin;
	asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");
}

void led3_detasend(rgb *data){
	uint32_t data24 = ((data->grean) <<16) + ((data->red) << 8) + ((data->blue));
	uint8_t buff[24] = {0};

	for(uint8_t i =0;i<24;i++){
		buff[i] = (  data24 & (1 << (23-i) )  ) ?  1 : 0 ;
	}

	buff[0] ? led3_T1() : led3_T0();
	buff[1] ? led3_T1() : led3_T0();
	buff[2] ? led3_T1() : led3_T0();
	buff[3] ? led3_T1() : led3_T0();
	buff[4] ? led3_T1() : led3_T0();
	buff[5] ? led3_T1() : led3_T0();
	buff[6] ? led3_T1() : led3_T0();
	buff[7] ? led3_T1() : led3_T0();

	buff[8] ? led3_T1() : led3_T0();
	buff[9] ? led3_T1() : led3_T0();
	buff[10] ? led3_T1() : led3_T0();
	buff[11] ? led3_T1() : led3_T0();
	buff[12] ? led3_T1() : led3_T0();
	buff[13] ? led3_T1() : led3_T0();
	buff[14] ? led3_T1() : led3_T0();
	buff[15] ? led3_T1() : led3_T0();

	buff[16] ? led3_T1() : led3_T0();
	buff[17] ? led3_T1() : led3_T0();
	buff[18] ? led3_T1() : led3_T0();
	buff[19] ? led3_T1() : led3_T0();
	buff[20] ? led3_T1() : led3_T0();
	buff[21] ? led3_T1() : led3_T0();
	buff[22] ? led3_T1() : led3_T0();
	buff[23] ? led3_T1() : led3_T0();

}

void led_hachune(led_hachune_t *data){
	led0_detasend(&data->led[0]);
	led1_detasend(&data->led[1]);
	led2_detasend(&data->led[2]);
	led3_detasend(&data->led[3]);
}

rgb get_color(const uint32_t data){
	rgb color;
	color.red   = (data & 0xFF0000) >> 16;
	color.grean = (data & 0x00FF00) >> 8;
	color.blue  = (data & 0x0000FF);
	return color;
}


rgb get_hsv(float h ,float s,float v){
	float max = v;
	float min = max - ((s/255)* max);
	float red,grean,blue;
	rgb color;
	if( h>=0 && h<60 ){
		red   = max;
		grean = (h/60) * (max-min) + min;
		blue  = min;
	}
	if( h>=60 && h<120 ){
		red   = ((120-h)/60)*(max-min)+min;
		grean = max;
		blue  = min;
	}
	if( h>=120 && h<180 ){
		red   = min;
		grean = max;
		blue  = ((h-120)/60)*(max-min)+min;
	}
	if( h>=180 && h<240 ){
		red   = min;
		grean = ((240-h)/60) * (max-min) + min;
		blue  = max;
	}
	if( h>=240 && h<300 ){
		red   = ((h-240)/60) * (max-min) + min;
		grean = min;
		blue  = max;
	}
	if( h>=300 && h<360 ){
		red   = max;
		grean = min;
		blue  = ((360-h)/60) * (max-min) + min;
	}

	color.grean = grean;
	color.red = red;
	color.blue = blue;

	return color;
}
