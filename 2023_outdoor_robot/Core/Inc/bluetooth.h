/*
 * bluetooth.h
 *
 *  Created on: Oct 3, 2023
 *      Author: sj
 */

#ifndef INC_BLUETOOTH_H_
#define INC_BLUETOOTH_H_

#include <stdio.h>
#include <string.h>
#include <usart.h>
#include "stdint.h"

#define	BLUETOOTH_USART	 (&huart3)

void APP_VALUE_PARSE_CallBack();
void AppValue_SendData1();
void AppValue_SendData1();

#endif /* INC_BLUETOOTH_H_ */
