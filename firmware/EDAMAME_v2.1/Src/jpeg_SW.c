#include "jpeg_SW.h"

//JPEGデコーダ入力関数
UINT in_func(JDEC* jd, uint8_t* buff, uint32_t nbyte){
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
UINT out_func(JDEC* jd, void* bitmap, JRECT* rect)
{
    IODEV *dev = (IODEV*)jd->device;       
	uint8_t *src;
	uint32_t width, height,bitshift;
	uint8_t r, g, b;
	double h=0, s, v; //H:色相 S:彩度 V:明度

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

