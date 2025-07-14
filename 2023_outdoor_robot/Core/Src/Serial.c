/*
 * Serial.c
 *
 *  Created on: Aug 27, 2018
 *      Author: sadeesh
 */

#include "Serial.h"
#ifdef USE_UART5
SERIAL_HandleTypeDef __uart5_serial_handler = {

		.buffer_Tx = &UART5_Buffer_TX,
		.buffer_Rx = &UART5_Buffer_RX,
		.hal_reg_Tx = &UART5_HAL_Reg_Tx,
		.hal_reg_Rx = &UART5_HAL_Reg_Rx,
		.application_reg_Rx = &UART5_Application_Reg_Tx,
		.WriteLock = SERIAL_WRITE_UNLOCKED,
		.ReadLock = SERIAL_READ_UNLOCKED
};
#endif

#ifdef USE_USART4
SERIAL_HandleTypeDef __uart4_serial_handler = {

		.buffer_Tx = &USART4_Buffer_TX,
		.buffer_Rx = &USART4_Buffer_RX,
		.hal_reg_Tx = &USART4_HAL_Reg_Tx,
		.hal_reg_Rx = &USART4_HAL_Reg_Rx,
		.application_reg_Rx = &USART4_Application_Reg_Tx,
		.WriteLock = SERIAL_WRITE_UNLOCKED,
		.ReadLock = SERIAL_READ_UNLOCKED
};
#endif

//SERIAL_HandleTypeDef*
SERIAL_HandleTypeDef* serial_init(UART_HandleTypeDef *huartx) {

	SERIAL_HandleTypeDef *serial_handler = NULL;

#ifdef USE_UART5
		if(huartx->Instance == UART5){

			UART5_Serial_Handler = &(__uart5_serial_handler);

			UART5_Serial_Handler->huartx = huartx;
			ring_buffer_init(UART5_Serial_Handler->buffer_Rx);
			ring_buffer_init(UART5_Serial_Handler->buffer_Tx);
			serial_handler = UART5_Serial_Handler;
			HAL_UART_Receive_DMA(UART5_Serial_Handler->huartx, UART5_Serial_Handler->buffer_Rx, PRIMARY_REG_SIZE);


		}

#endif
#ifdef USE_USART4
		if(huartx->Instance == USART4){

			USART3_Serial_Handler = &(__uart4_serial_handler);

			USART3_Serial_Handler->huartx = huartx;
			ring_buffer_init(USART4_Serial_Handler->buffer_Rx);
			ring_buffer_init(USART4_Serial_Handler->buffer_Tx);
			serial_handler = USART4_Serial_Handler;
			HAL_UART_Receive_DMA(USART4_Serial_Handler->huartx, USART3_Serial_Handler->hal_reg_Rx, PRIMARY_REG_SIZE);
		}
#endif

	return serial_handler;

}
//void HAL_DMA_STATE_CHANGE(UART_HandleTypeDef *huart)
//{
//  huart->gState = HAL_UART_STATE_READY;
//}

void serial_write(SERIAL_HandleTypeDef* hserial, uint8_t *pData, uint16_t len) {

	if (ring_buffer_is_empty(hserial->buffer_Tx)) {
			if (HAL_UART_Transmit_DMA(hserial->huartx, pData, len) != HAL_OK) {
				ring_buffer_queue_arr(hserial->buffer_Tx, pData, len);
			}
	} else {

				ring_buffer_queue_arr(hserial->buffer_Tx, pData, len);

				HAL_UART_Transmit(&huart5,pData,len,HAL_MAX_DELAY);

					}

				}



uint8_t serial_available(SERIAL_HandleTypeDef* hserial) {

	return !ring_buffer_is_empty(hserial->buffer_Rx);
}

uint8_t serial_read(SERIAL_HandleTypeDef* hserial) {

		if (!ring_buffer_is_empty(hserial->buffer_Rx)) {
			ring_buffer_dequeue(hserial->buffer_Rx, hserial->application_reg_Rx);
			return *(hserial->application_reg_Rx);
		}

		return '\0';
}

//void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
//
//	SERIAL_HandleTypeDef* serial_handler = get_serial_handler(huart);
//
//	if (!ring_buffer_is_empty(serial_handler->buffer_Tx)) {
//
//		  ring_buffer_dequeue(serial_handler->buffer_Tx, serial_handler->hal_reg_Tx);
//		  HAL_UART_Transmit_DMA(huart, serial_handler->hal_reg_Tx, PRIMARY_REG_SIZE);
//	}
//}

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
//
//	SERIAL_HandleTypeDef* serial_handler = get_serial_handler(huart);
//
//	ring_buffer_queue_arr(serial_handler->buffer_Rx, serial_handler->hal_reg_Rx, PRIMARY_REG_SIZE);
//	HAL_UART_Receive_DMA(huart, serial_handler->hal_reg_Rx, PRIMARY_REG_SIZE);
//}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart5) {
	if (huart5->ErrorCode == HAL_UART_ERROR_ORE) {

	}
}

