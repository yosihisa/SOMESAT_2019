#include "main.h"
#include "stm32f4xx_hal.h"

typedef enum {
    CAMERA_OK = 0,				/* (0) Succeeded */
    CAMERA_ERR,			        /* (1) �G���[ */
    CAMERA_COM_ERR,             /* (2) �ʐM�G���[ */
    CAMERA_NOT_ENOUGH_CORE      /* (3)�������s�� */
    
} CAMERARESULT;

typedef enum {
    QVGA = 0,
    VGA
} CAMERARESIZE;

CAMERARESULT camera_Init(UART_HandleTypeDef *huart ,CAMERARESIZE size);//�ʐM�m���E�������E�p�P�b�g�T�C�Y�w��
CAMERARESULT snap_shot(UART_HandleTypeDef *huart);//�ʐ^���B��
CAMERARESULT get_picture(UART_HandleTypeDef *huart,uint8_t *buffer,uint32_t size , uint32_t *data_size);//�ʐ^��]������


CAMERARESULT camera_sync(UART_HandleTypeDef *huart);
CAMERARESULT camera_inital(UART_HandleTypeDef *huart ,CAMERARESIZE size);
CAMERARESULT camera_inital_HiSpeed(UART_HandleTypeDef *huart ,CAMERARESIZE size,uint32_t baudrate);
            
