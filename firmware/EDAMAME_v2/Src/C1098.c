#include "C1098.h"

int Change_UART_BaudRate(UART_HandleTypeDef *huart,uint32_t baudrate)
{

  huart->Init.BaudRate = baudrate;
  
 // huart->Instance = USART1;
  /*
  *huart->Init.WordLength = UART_WORDLENGTH_8B;
  *huart->Init.StopBits = UART_STOPBITS_1;
  *huart->Init.Parity = UART_PARITY_NONE;
  *huart->Init.Mode = UART_MODE_TX_RX;
  *huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
  *huart->Init.OverSampling = UART_OVERSAMPLING_16;
  */
  if (HAL_UART_Init(huart) != HAL_OK)
  {
    return -1;
  }
  return 0;
}

 /**
  * @brief  同期　通信確認(通常main関数から呼び出されない)
  * @param  huart: カメラが接続されているポートのUARTハンドル
  * @retval CAMELA status
  */
CAMERARESULT camera_sync(UART_HandleTypeDef *huart){
    const char sync[6] ={0xAA,0x0D,0x00,0x00,0x00,0x00};
    const char ack[6] = {0xAA,0x0E,0x00,0x00,0x00,0x00};
    char c_data[12];

    HAL_UART_Transmit( huart, &sync, 6, 6 );
    HAL_UART_Receive(huart, &c_data, 12, 100);
    if(c_data[0]==0xAA && c_data[1]==0x0E && c_data[2]==0x0D  && c_data[6]==0xAA && c_data[7]==0x0D && c_data[8]==0x00){
        HAL_UART_Transmit( huart, &ack, 6, 6 );
        return CAMERA_OK;
    }
    return CAMERA_COM_ERR;
}

 /**
  * @brief  通信確立・初期設定(通常main関数から呼び出されない)
  * @param  huart: カメラが接続されているポートのUARTハンドル
  * @param  size: 撮影する画像の解像度
  * @retval CAMELA status
  */
CAMERARESULT camera_inital(UART_HandleTypeDef *huart ,CAMERARESIZE size){
    char c_data[6];
    char set_package_size[6] = {0xAA,0x06,0x08,0x00,0x01,0x00};  

    const char ack[6] = {0xAA,0x0E,0x00,0x00,0x00,0x00};    
    if(size == QVGA){
        char initial[6] = {0xAA,0x01,0x07,0x07,0x00,0x05};
        HAL_UART_Transmit( huart, &initial, 6, 6 );
    }else{
        char initial[6] = {0xAA,0x01,0x07,0x07,0x00,0x07};    
        HAL_UART_Transmit( huart, &initial, 6, 6 );
    }

    HAL_UART_Receive(huart, &c_data, 6, 100);
    if(c_data[0]==0xAA && c_data[1]==0x0E && c_data[2]==0x01){
        HAL_Delay(60);
        HAL_UART_Transmit( huart, &set_package_size, 6, 6 );
        HAL_UART_Receive(huart, &c_data, 6, 100);
        if(c_data[0]==0xAA && c_data[1]==0x0E && c_data[2]==0x06)return CAMERA_OK;
        else return CAMERA_ERR;
    }
    return CAMERA_ERR;
}

 /**
  * @brief  通信確立・初期設定・パケットサイズの設定
  * @param  huart: カメラが接続されているポートのUARTハンドル
  * @param  size: 撮影する画像の解像度
  * @retval CAMELA status
  */
CAMERARESULT camera_Init(UART_HandleTypeDef *huart ,CAMERARESIZE size){
    for(int i =0 ; i<70 ; i ++ ){
        if(camera_sync(huart) == CAMERA_OK){
            if(camera_inital(huart,size) == CAMERA_OK) return CAMERA_OK;
            else return CAMERA_ERR;
        }
    }
    return CAMERA_COM_ERR;    
}

 /**
  * @brief  写真を1枚撮影する
  * @param  huart: カメラが接続されているポートのUARTハンドル
  * @retval CAMELA status
  */
CAMERARESULT snap_shot(UART_HandleTypeDef *huart){
	char c_data[6];
    const char snap_shot[6] = {0xAA,0x05,0x00,0x00,0x00,0x00};  
    HAL_UART_Transmit(huart, &snap_shot, 6, 6 );
    HAL_UART_Receive(huart, &c_data, 6, 100);
    if(c_data[0]==0xAA && c_data[1]==0x0E && c_data[2]==0x05)return CAMERA_OK;
    else return CAMERA_ERR;    
}


 /**
  * @brief  撮影した写真を転送する
  * @param  huart: カメラが接続されているポートのUARTハンドル
  * @param  buffer: 転送先のバッファの始点
  * @param  Size: バッファの容量
  * @param  Timeout: 撮影した画像の容量
  * @retval CAMELA status
  */
CAMERARESULT get_picture(UART_HandleTypeDef *huart,uint8_t *buffer,uint32_t size , uint32_t *data_size){
    const char get_picture[6] = {0xAA,0x04,0x01,0x00,0x00,0x00};  
    char c_data[6];
    *data_size =0 ;
    uint16_t image_data_size =0 ,id =0;

    //送信 GET PICTURE
    HAL_UART_Transmit(huart, &get_picture, 6, 6 );

    //受信 ACK
    HAL_UART_Receive(huart, &c_data, 6, 100);
    if(c_data[0]!=0xAA || c_data[1]!=0x0E || c_data[2]!=0x04)return CAMERA_ERR;

    //受信 Data Length
    HAL_UART_Receive(huart, &c_data, 6, 1000);
    if(c_data[0]!=0xAA || c_data[1]!=0x0A || c_data[2]!=0x01)return CAMERA_ERR;
    *data_size |= c_data[5]<<16;
    *data_size |= c_data[4]<<8;
    *data_size |= c_data[3];

    //バッファの容量が足りているか確認
    if(*data_size >= size) return CAMERA_NOT_ENOUGH_CORE;

    //受信
    char ack[6] = {0xAA,0x0E,0x00,0x00,0x00,0x00};    
    while(1){
        ack[4]= (char) id;
        ack[5]= (char) id >>8;
        
        HAL_UART_Transmit(huart, &ack, 6, 6 );
        HAL_UART_Receive(huart, &c_data, 4, 1000);
        image_data_size  = c_data[3]<<8;
        image_data_size |= c_data[2];
        HAL_UART_Receive(huart, buffer + 250*id, image_data_size, image_data_size + 1000);

        HAL_UART_Receive(huart, &c_data, 2, 1000);
        
        if(image_data_size < 250)break;

        id ++;
    }
    ack[4] = 0xF0;
    ack[5] = 0xF0;
    HAL_UART_Transmit(huart, &get_picture, 6, 6 );
    
    return CAMERA_OK;
    
}


CAMERARESULT camera_sync_hispeed(UART_HandleTypeDef *huart ,uint32_t baudrate, uint16_t c){
    int e = Change_UART_BaudRate(huart,14400);    
    for(int i =0 ; i<=c ; i ++ ){
        if(camera_sync(huart) == CAMERA_OK)return CAMERA_OK;
        if(i == c){
            Change_UART_BaudRate(huart,baudrate);
            if(e != 0)return 12;
            for(int i =0 ; i<=c ; i ++ ){
                if(camera_sync(huart) == CAMERA_OK)return CAMERA_OK;
                if(i == c)return CAMERA_COM_ERR; 
            }
        }
    }
}
 /**
  * @brief  通信確立・初期設定(通常main関数から呼び出されない)
  * @param  huart: カメラが接続されているポートのUARTハンドル
  * @param  size: 撮影する画像の解像度
  * @param  baudrate: 通信速度
  * @retval CAMELA status
  */
  CAMERARESULT camera_inital_HiSpeed(UART_HandleTypeDef *huart ,CAMERARESIZE size,uint32_t baudrate){
    int e =0;;


    const char sync[6] ={0xAA,0x0D,0x00,0x00,0x00,0x00};
    const char ack[6] = {0xAA,0x0E,0x00,0x00,0x00,0x00};
    char initial[6] = {0xAA,0x01,0x07,0x07,0x00,0x00};    
    char set_package_size[6] = {0xAA,0x06,0x08,0x00,0x01,0x00};  
    
    char c_data[12];
/*
    int e = Change_UART_BaudRate(huart,14400);    
    for(int i =0 ; i<70 ; i ++ ){
        if(camera_sync(huart) == CAMERA_OK)break;
        if(i == 69){
            Change_UART_BaudRate(huart,baudrate);
            if(e != 0)return 12;
            for(int i =0 ; i<70 ; i ++ ){
                if(camera_sync(huart) == CAMERA_OK)break;                
                if(i == 69)return CAMERA_COM_ERR; 
            }
        }
    }*/
    if(camera_sync_hispeed(huart,baudrate,10) != CAMERA_OK){
        camera_sync_hispeed(huart,baudrate,70);
    }

    //if(camera_sync(huart) == CAMERA_OK)

    if(size == QVGA){
        initial[5]=0x05;
    }else{
        initial[5]=0x07;        
    }
    if(baudrate == 14400)initial[2]=0x07;    
    if(baudrate == 28800)initial[2]=0x06;
    if(baudrate == 57600)initial[2]=0x05;
    if(baudrate == 115200)initial[2]=0x04;
    if(baudrate == 230400)initial[2]=0x03;
    if(baudrate == 460800)initial[2]=0x02;
    
    
    HAL_UART_Transmit( huart, &initial, 6, 6 );
    HAL_UART_Receive(huart, &c_data, 6, 100);
    if(c_data[0]==0xAA && c_data[1]==0x0E && c_data[2]==0x01){

        HAL_Delay(60);

        e = Change_UART_BaudRate(huart,baudrate);
        if(e != 0)return 13;

        HAL_Delay(20);
        
        HAL_UART_Transmit( huart, &set_package_size, 6, 6 );
        HAL_UART_Receive(huart, &c_data, 6, 100);
        if(c_data[0]==0xAA && c_data[1]==0x0E && c_data[2]==0x06)return CAMERA_OK;
        else return CAMERA_ERR;
    }else{
        return CAMERA_ERR;
    }

   // if(camera_inital(huart,size) == CAMERA_OK) return CAMERA_OK;
  //  else return CAMERA_ERR;
    
}