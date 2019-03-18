#include "stm32f4xx_hal.h"
#include "tjpgd.h"


//F‘Š‚Ì”ÍˆÍ
#define H_MIN_1 0	//ŒÅ’è
#define H_MAX_1 10

#define H_MIN_2 330
#define H_MAX_2 360 //ŒÅ’è

//Ê“x‚Ì”ÍˆÍ
#define S_MIN 45
#define S_MAX 100

//–¾“x‚Ì”ÍˆÍ
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
