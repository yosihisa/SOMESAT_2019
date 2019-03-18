#include "stm32f4xx_hal.h"
#include "tjpgd.h"


//�F���͈̔�
#define H_MIN_1 0	//�Œ�
#define H_MAX_1 10

#define H_MIN_2 330
#define H_MAX_2 360 //�Œ�

//�ʓx�͈̔�
#define S_MIN 45
#define S_MAX 100

//���x�͈̔�
#define V_MIN 30
#define V_MAX 100

#define HEIGHT 480
#define WIDTH  640


uint8_t RED_bool[HEIGHT][WIDTH/8];

#define MAX_SIZE 40*1024
typedef struct {
	uint8_t jpeg_data[MAX_SIZE];
	uint32_t jpeg_data_seek ;
    uint32_t jpeg_data_size ;
    uint8_t RED_bool[HEIGHT][WIDTH / 8];
} IODEV;

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

UINT in_func(JDEC* jd, uint8_t* buff, uint32_t nbyte);
UINT out_func(JDEC* jd, void* bitmap, JRECT* rect);
