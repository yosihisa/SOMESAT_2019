#include "main.h"
#include "stm32f4xx_hal.h"

typedef enum {
    CAMERA_OK = 0,				/* (0) Succeeded */
    CAMERA_ERR,			        /* (1) エラー */
    CAMERA_COM_ERR,             /* (2) 通信エラー */
    CAMERA_NOT_ENOUGH_CORE      /* (3)メモリ不足 */
    
} CAMERARESULT;

typedef enum {
    QVGA = 0,
    VGA
} CAMERARESIZE;

CAMERARESULT camera_Init(UART_HandleTypeDef *huart ,CAMERARESIZE size);//通信確立・初期化・パケットサイズ指定
CAMERARESULT snap_shot(UART_HandleTypeDef *huart);//写真を撮る
CAMERARESULT get_picture(UART_HandleTypeDef *huart,uint8_t *buffer,uint32_t size , uint32_t *data_size);//写真を転送する


CAMERARESULT camera_sync(UART_HandleTypeDef *huart);
CAMERARESULT camera_inital(UART_HandleTypeDef *huart ,CAMERARESIZE size);
CAMERARESULT camera_inital_HiSpeed(UART_HandleTypeDef *huart ,CAMERARESIZE size,uint32_t baudrate);
            
