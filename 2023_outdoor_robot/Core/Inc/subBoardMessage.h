/*
 * subBoardMessage.h
 *
 *  Created on: Oct 9, 2023
 *      Author: sj
 */

#ifndef INC_SUBBOARDMESSAGE_H_
#define INC_SUBBOARDMESSAGE_H_

#include <stdio.h>
#include <string.h>
#include <usart.h>
#include "stdint.h"

#define SubMESSAGE (&huart6)


void subBoardData_Callback();

#endif /* INC_SUBBOARDMESSAGE_H_ */
