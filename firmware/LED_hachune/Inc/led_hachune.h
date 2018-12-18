/*
 * led_hachune.h
 *
 *  Created on: 2018/12/06
 *      Author: yosihisa
 */

#ifndef LED_HACHUNE_H_
#define LED_HACHUNE_H_

typedef struct {
    uint8_t red,grean,blue;
} rgb;

typedef struct {
	rgb led[4];
} led_hachune_t;

void led_hachune(led_hachune_t *data);

rgb get_color(uint32_t);
rgb get_hsv(float h ,float s,float v);

#endif /* LED_HACHUNE_H_ */
