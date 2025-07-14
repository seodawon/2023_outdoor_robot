/*
 * bluetooth.c
 *
 *  Created on: Oct 3, 2023
 *      Author: sj
 */
#include "bluetooth.h"

uint8_t cnt = 0;
//앱인벤터에서 보낸 데이터를 받는 변수
uint8_t rx3_data;
//앱인벤터에서 보낸 데이터를 받아서 파싱하기위해 담는 변수
char rx3_data1[25];
//앱인벤터에서 보낸 값 넣는 변수 형식 => 각도,거리#
float GPS_angle;
float GPS_distance;
uint8_t point;

void APP_VALUE_PARSE_CallBack(){
	//앱인벤터에서 보낸 각도와 거리를 받아서 파싱하는 함수
	//rx3_data1의 문자열에 rx3_data(앱인벤터에서 보낸 값)을 넣는데 1바이트씩 각 인덱스 방에 넣기 시작
	rx3_data1[cnt++] = (char) rx3_data;
	//앱인벤터에서 보내는 하나의 패킷이 '#'으로 끝나게 만들었기 때문에 마지막 바이트가 '#'으로 들어오면
	//cnt=0으로 만들어주어서 다시 값을 첨부터 받을 준비하기 => 다음에 값을 받을떄 덮어씌우기 형
	if(rx3_data == '#') cnt = 0;
	//sscanf는 내가 들어오는 문자열 형식을 알떄 각각의 변수에 할당시킬 수 있음
	sscanf(rx3_data1, "%f,%f,%d#", &GPS_angle, &GPS_distance,&point);
	//BLUETOOTH_USART=>&huart1을 뜻
	HAL_UART_Receive_IT(BLUETOOTH_USART, &rx3_data, sizeof(rx3_data));
}

void AppValue_SendData1(){
	/* Send data via UART */
	// 앱인벤터에서 보낸 각도와 거리를 받아서 usb to ttl과 연결된 huart3로 보내는 함수
	char buffer[80];
	sprintf(buffer, "Number %f,%f,%d\r\n", GPS_angle, GPS_distance,point);
	HAL_UART_Transmit(&huart6, (uint8_t*) buffer, strlen(buffer), HAL_MAX_DELAY);
}
