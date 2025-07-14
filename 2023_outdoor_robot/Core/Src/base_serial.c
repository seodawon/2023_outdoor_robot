/*
 * base_serial.c
 *
 *  Created on: Aug 30, 2018
 *      Author: sadeesh
 */

#include "base_serial.h"

SERIAL_HandleTypeDef* get_serial_handler(UART_HandleTypeDef *huartx){

	SERIAL_HandleTypeDef* ret_serial_handler = NULL;

#ifdef USE_UART5
	if(huartx->Instance == UART5){
		ret_serial_handler = UART5_Serial_Handler;
	}
#endif
	return ret_serial_handler;
}

