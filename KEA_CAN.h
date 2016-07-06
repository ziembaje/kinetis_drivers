/*
 * Main_CAN.h


 *
 *  Created on: Mar 13, 2016
 *      Author: jack
 */

#ifndef LIB_MAIN_CAN_H_
#define LIB_MAIN_CAN_H_

//#include "Main_CAN_Registers.h"
#include "SKEAZ1284.h"
#include <stdbool.h>

typedef struct _CAN_Data {
	uint8_t msgLength;
	uint16_t msgTimestamp;
	uint16_t msgID;
	uint8_t msg_data[8];

} _CAN_Data;

typedef struct {
	uint8_t tx_err_count;
	uint8_t rx_err_count;
	bool bus_off;
	bool tx_warn;
	bool rx_warn;
	bool tx_err;
	bool rx_err;
	bool overflow;
} _CAN_Error_Status;

void Module_CAN_Init(uint16_t *identifier_acceptance,
		uint16_t *identifier_masks, void (*receiveHandlerTeamCAN)(_CAN_Data),
		void (*errorHandlerTeamCAN)(_CAN_Error_Status));

bool Module_CAN_Transmit(_CAN_Data outData);

#endif /* LIB_MAIN_CAN_H_ */

