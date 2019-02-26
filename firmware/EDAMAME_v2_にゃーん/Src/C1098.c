#include "C1098.h"

//�ʐM���x�̕ύX
HAL_StatusTypeDef Change_UART_BaudRate(UART_HandleTypeDef *huart ,uint32_t baudrate){
	huart->Init.BaudRate = baudrate;
	return HAL_UART_Init(huart);
}

//sync�R�}���h�̑��M
CAMERARESULT sync(c1098_handle *handle){
	uint8_t sync[6] ={0xAA,0x0D,0x00,0x00,0x00,0x00};
	uint8_t ack[6] = {0xAA,0x0E,0x00,0x00,0x00,0x00};
	uint8_t c_data[12];

	HAL_UART_Transmit( handle->uart_port, sync, 6, 6 );
	HAL_UART_Receive( handle->uart_port, c_data, 12, 100);
	if (c_data[0]==0xAA && c_data[1]==0x0E && c_data[2]==0x0D  && c_data[6]==0xAA && c_data[7]==0x0D && c_data[8]==0x00){
		HAL_UART_Transmit( handle->uart_port, ack, 6, 6 );
		return CAMERA_OK;
	}
	return CAMERA_COM_ERR;
}

//�J�����ƒʐM�ł���悤�ɂ���
CAMERARESULT camera_sync(c1098_handle *handle){

	//�z�b�g�X�^�[�g
	if (Change_UART_BaudRate(handle->uart_port,handle->baudrate)!=HAL_OK){
		return CAMERA_FAILED_CHANGE_BAUDRATE;
	}
	if (sync(handle) == CAMERA_OK){
		return CAMERA_OK;
	}

	//����������������f�t�H���g�̑��x�ŒʐM
	if (Change_UART_BaudRate(handle->uart_port,14400)!=HAL_OK){
		return CAMERA_FAILED_CHANGE_BAUDRATE;
	}

	//60��sync�𑗏o
	for (int i =0 ; i<60 ; i ++ ){
		if (sync(handle) == CAMERA_OK){
			return CAMERA_OK;
		}
	}
	return CAMERA_COM_ERR;
}

//�J�����̏����ݒ�
CAMERARESULT camera_init(c1098_handle *handle){
	uint8_t initial[6] = {0xAA,0x01,0x07,0x07,0x00,0x00};
	uint8_t set_package_size[6] = {0xAA,0x06,0x08,0x00,0x01,0x00};

	uint8_t c_data[12];

	if (handle->resolution == QVGA)initial[5]=0x05;
	if (handle->resolution ==  VGA)initial[5]=0x07;

	set_package_size[3] = (handle->packet_size) ;
	set_package_size[4] = (handle->packet_size) >>8;

	if (handle->baudrate == 14400)initial[2]=0x07;
	if (handle->baudrate == 28800)initial[2]=0x06;
	if (handle->baudrate == 57600)initial[2]=0x05;
	if (handle->baudrate == 115200)initial[2]=0x04;
	if (handle->baudrate == 230400)initial[2]=0x03;
	if (handle->baudrate == 460800)initial[2]=0x02;

	//sync
	if (camera_sync(handle) != CAMERA_OK){
		return CAMERA_COM_ERR;
	}

	//Cahge BaudRate
	HAL_UART_Transmit( handle->uart_port, initial, 6, 6 );
	HAL_UART_Receive(handle->uart_port, c_data, 6, 100);
	if (c_data[0]==0xAA && c_data[1]==0x0E && c_data[2]==0x01){

		if (Change_UART_BaudRate(handle->uart_port,handle->baudrate) != HAL_OK){
			return CAMERA_FAILED_CHANGE_BAUDRATE;
		}
	}else{
		return CAMERA_ERR;
	}

	HAL_Delay(60);

	//Cange Package Size
	HAL_UART_Transmit(handle->uart_port, set_package_size, 6, 6 );
	HAL_UART_Receive(handle->uart_port, c_data, 6, 100);
	if (c_data[0]==0xAA && c_data[1]==0x0E && c_data[2]==0x06){
		return CAMERA_OK;
	}else{
		return CAMERA_ERR;
	}
}

//�ʐ^���B�e����
CAMERARESULT snap_shot(c1098_handle *handle){
	uint8_t c_data[6];
	uint8_t snap_shot[6] = {0xAA,0x05,0x00,0x00,0x00,0x00};
	HAL_UART_Transmit(handle->uart_port, snap_shot, 6, 6 );
	HAL_UART_Receive(handle->uart_port, c_data, 6, 100);
	if (c_data[0]==0xAA && c_data[1]==0x0E && c_data[2]==0x05){
		return CAMERA_OK;
	}else{
		return CAMERA_ERR;
	}
}

//�B�e�����ʐ^��]������
CAMERARESULT get_picture(c1098_handle *handle,uint8_t *buffer,uint32_t buffer_size , uint32_t *received_size){
	uint8_t get_picture[6] = {0xAA,0x04,0x01,0x00,0x00,0x00};
	uint8_t  c_data[6];
	uint32_t data_length =0;
	uint16_t packet_size =0;
	uint16_t id =0;

	*received_size = 0;

	//Transmit GET PICTURE
	HAL_UART_Transmit(handle->uart_port, get_picture, 6, 6 );

	//Receive ACK
	HAL_UART_Receive(handle->uart_port, c_data, 6, 100);
	if (c_data[0]!=0xAA || c_data[1]!=0x0E || c_data[2]!=0x04){
		return CAMERA_ERR;
	}

	//Receive Data Length
	HAL_UART_Receive(handle->uart_port, c_data, 6, 100);
	if (c_data[0]!=0xAA || c_data[1]!=0x0A || c_data[2]!=0x01){
		return CAMERA_ERR;
	}

	//Data Length
	data_length |= c_data[5]<<16;
	data_length |= c_data[4]<<8;
	data_length |= c_data[3];


	//�o�b�t�@�̗e�ʂ�����Ă��邩�m�F
	if ( data_length >= buffer_size ){
		return CAMERA_NOT_ENOUGH_CORE;
	}

	//��M
	uint8_t ack[6] = {0xAA,0x0E,0x00,0x00,0x00,0x00};
	while(1){
		//ACK with Package ID
		ack[4]= (uint8_t)  id;
		ack[5]= (uint8_t) (id>>8);
		HAL_UART_Transmit(handle->uart_port, ack, 6, 6 );

		//Image Data Package
		HAL_UART_Receive(handle->uart_port, c_data, 4, 100);
		packet_size  = c_data[3]<<8;
		packet_size |= c_data[2];
		HAL_UART_Receive(handle->uart_port, buffer + (handle->packet_size-6)*id, packet_size, 100);
		HAL_UART_Receive(handle->uart_port, c_data, 2, 100);

		*received_size += packet_size;
		if ( *received_size >=  data_length ){
			break;
		}
		id++;
	}
	ack[4] = 0xF0;
	ack[5] = 0xF0;
	HAL_UART_Transmit(handle->uart_port, get_picture, 6, 6 );

	return CAMERA_OK;
}
