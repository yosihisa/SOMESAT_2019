#include "stm32f4xx_hal.h"

typedef enum {
	CAMERA_OK = 0,				  /* (0) Succeeded */
	CAMERA_ERR,			          /* (1) エラー */
	CAMERA_COM_ERR,               /* (2) 通信エラー */
	CAMERA_NOT_ENOUGH_CORE,       /* (3)メモリ不足 */
	CAMERA_FAILED_CHANGE_BAUDRATE /* (4)ボーレート変更に失敗 */

} CAMERARESULT;

typedef enum {
	QVGA = 0,
	VGA
} CAMERA_RESOLUTION;

typedef struct {
	UART_HandleTypeDef *uart_port;
	uint32_t packet_size;
	uint32_t baudrate;
	CAMERA_RESOLUTION resolution;
} c1098_handle;

CAMERARESULT camera_init(c1098_handle *handle); //初期化
CAMERARESULT snap_shot(c1098_handle *handle);//写真を撮る
CAMERARESULT get_picture(c1098_handle *handle,uint8_t *buffer,uint32_t size , uint32_t *data_size);//写真を転送する
