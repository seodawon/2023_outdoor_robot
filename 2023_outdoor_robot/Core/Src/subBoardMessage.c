/*
 * subBoardMessage.c
 *
 *  Created on: Oct 9, 2023
 *      Author: sj
 */

#include "subBoardMessage.h"

uint8_t cnt1=0;
uint8_t rx6_data;
uint8_t send_data;

char board_data[50];
uint8_t  sensor_name[25];
float sensor_value;
float Lidar_data;
uint16_t srf08_data;
uint8_t LCD_data;

void subBoardData_Callback(){

	board_data[cnt1++] = rx6_data;

	if(rx6_data== '#') cnt1 =0;
	sscanf(board_data,"%s:%f#",&sensor_name,&sensor_value);
	if(sensor_name =="LCD"){
		LCD_data= (uint8_t) sensor_value;
		//시작 버튼을 눌렀을떄 받는 값과 종료 버튼을 눌렀을떄 받는 값 처리해야함
	}else if(sensor_name=="SRF"){
		//srf08 왼쪽과 오른쪽을 나눈다. 왼쪽일때 속도조절 오른쪽일떄 속도 조절
		srf08_data= (uint16_t) sensor_value;
		if(srf08_data ==1){
			send_data=1;
			HAL_UART_Transmit(SubMESSAGE,&send_data,1,HAL_MAX_DELAY);
		}else if(srf08_data ==2){
			send_data=2;
			HAL_UART_Transmit(SubMESSAGE,&send_data,1,HAL_MAX_DELAY);
		}
		//0도가 아닐떄 값받기로 함
	}else if(sensor_name=="LIDAR"){
		Lidar_data= sensor_value;
	}


	HAL_UART_Receive_IT(SubMESSAGE,&rx6_data,sizeof(rx6_data));
}




