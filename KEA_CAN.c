/*
 * Main_CAN.c

 *
 *  Created on: Mar 13, 2016
 *      Author: jack
 */

//#include "SKEAZ1284.h"
#include "KEA_CAN.h"
#include "stdbool.h"

/// Function :   Module_CAN_Initialization
/***************************************************************************
 *
 * @Brief: Initialise MSCAN driver
 *
 * @param: uint8 identifier_acceptance: array containing 8 acceptance filter values
 * 
 * @param: uint8 identifier_mask: array containing 8 filter masks
 *
 * @param: void receiveHandlerTeamCAN: function pointer for receive handler
 * 
 * @param: void errorHandlerTeamCAN: function pointer for error handler
 *
 * @return: void
 *
 **************************************************************************/

void (*receiveHandler)(_CAN_Data);
void (*errorHandler)(_CAN_Error_Status);

void Module_CAN_Init(uint8_t *identifier_acceptance, uint8_t *identifier_mask,
		void (*receiveHandlerTeamCAN)(_CAN_Data),
		void (*errorHandlerTeamCAN)(_CAN_Error_Status)) {

	receiveHandler = receiveHandlerTeamCAN;
	errorHandler = errorHandlerTeamCAN;
	
	SIM_SCGC |= SIM_SCGC_MSCAN_MASK;

	MSCAN_CANCTL1 |= MSCAN_CANCTL1_CANE_MASK;

	MSCAN_CANCTL1 &= ~MSCAN_CANCTL1_LISTEN_MASK; /* exit listen only mode */
	MSCAN_CANBTR0 |= 0xC0; /* Synchronisation jump width = 4tq cycle, baud rate prescaler = 1*/
	MSCAN_CANBTR1 |= 0x3A; /* one sample per bit, 4 tq cycles, 11 tq cycles */
	MSCAN_CANIDAC |= 0x20; /* four 16-bit acceptance filters */
	MSCAN_CANCTL0 |= MSCAN_CANCTL0_TIME_MASK; /* enable time stamps */

	MSCAN_CANIDAR0 = identifier_acceptance[0]; /* Initialise acceptance filters and masks */
	MSCAN_CANIDMR0 = identifier_mask[0];

	MSCAN_CANIDAR1 = identifier_acceptance[1];
	MSCAN_CANIDMR1 = identifier_mask[1];

	MSCAN_CANIDAR2 = identifier_acceptance[2];
	MSCAN_CANIDMR2 = identifier_mask[2];

	MSCAN_CANIDAR3 = identifier_acceptance[3];
	MSCAN_CANIDMR3 = identifier_mask[3];

	MSCAN_CANIDAR4 = identifier_acceptance[4];
	MSCAN_CANIDMR4 = identifier_mask[4];

	MSCAN_CANIDAR5 = identifier_acceptance[5];
	MSCAN_CANIDMR5 = identifier_mask[5];

	MSCAN_CANIDAR6 = identifier_acceptance[6];
	MSCAN_CANIDMR6 = identifier_mask[6];

	MSCAN_CANIDAR7 = identifier_acceptance[7];
	MSCAN_CANIDMR7 = identifier_mask[7];

	MSCAN_CANCTL0 |= MSCAN_CANCTL0_WUPE_MASK;

	MSCAN_CANCTL0 &= ~(MSCAN_CANCTL0_INITRQ_MASK); /* clear soft reset */
	MSCAN_CANCTL0 &= ~MSCAN_CANCTL0_SLPRQ_MASK; /* exit sleep mode */
	MSCAN_CANCTL1;

	/* EnableInterrupts */

	NVIC_ICPR |= NVIC_ICPR_CLRPEND30_MASK;
	NVIC_ISER |= NVIC_ISER_SETENA30_MASK;
	NVIC_ICPR |= NVIC_ICPR_CLRPEND31_MASK;
	NVIC_ISER |= NVIC_ISER_SETENA31_MASK;

	MSCAN_CANRIER |= 0x20 | 0x08 | MSCAN_CANRIER_CSCIE_MASK
			| MSCAN_CANRIER_OVRIE_MASK | MSCAN_CANRIER_RXFIE_MASK;

	NVIC_IPR7 &= ~(0xC00000); //set Rx interrupt to highest priority

}

/// Function :   Module_CAN_Transmit
/***************************************************************************
 *
 * @Brief: Transmit a message 
 *
 * @param: _CAN_Data: struct containing CAN message data
 *
 * @return: bool; true if transmit successful, otherwise false
 *
 **************************************************************************/

bool Module_CAN_Transmit(_CAN_Data outData) {

	/* message buffers are 16-bytes */

	uint8_t selected_buffer = 0;

	if (MSCAN_CANTFLG == 0x00) // transmit buffers are all full
		return false;

	else {

		MSCAN_CANTBSEL = MSCAN_CANTFLG;
		selected_buffer = MSCAN_CANTBSEL;

		MSCAN_TEIDR0 = 0;
		MSCAN_TEIDR1 = 0;
		MSCAN_TEIDR2 = 0;
		MSCAN_TEIDR3 = 0;

		MSCAN_TSIDR0 = outData.msgID >> 3; /* load identifier register */
		MSCAN_TSIDR1 = outData.msgID << 5;

		MSCAN_TDLR = outData.msgLength;

		for (int i = 0; i < outData.msgLength; i++) /* load message into data segment registers */
			MSCAN_TEDSR_REG(MSCAN_BASE_PTR,i) = outData.msg_data[i];

		MSCAN_TBPR = 0;

		MSCAN_CANTFLG = selected_buffer;

		return true;
	}

}

void MSCAN_Rx_IRQHandler() {

	_CAN_Data outData;

	outData.msgID = 0;
	outData.msgID |= MSCAN_RSIDR0 << 3;
	outData.msgID |= MSCAN_RSIDR1 >> 5;
	outData.msgLength = MSCAN_RDLR;
	outData.msgTimestamp = 0;
	outData.msgTimestamp |= MSCAN_RTSRH << 3;
	outData.msgTimestamp |= MSCAN_RTSRL >> 5;

	for (int i = 0; i < outData.msgLength; i++)
		outData.msg_data[i] = MSCAN_REDSR_REG(MSCAN_BASE_PTR,i) ;

	receiveHandler(outData);

	MSCAN_CANRFLG |= MSCAN_CANRFLG_RXF_MASK;

}

void MSCAN_Tx_IRQHandler() {

	_CAN_Error_Status canData;

	if ((MSCAN_CANRFLG & MSCAN_CANRFLG_RSTAT_MASK) == 0x00) {
		canData.rx_warn = 0;
		canData.rx_err = 0;
	} else if ((MSCAN_CANRFLG & MSCAN_CANRFLG_RSTAT_MASK) == 0x10) {
		canData.rx_warn = 1;
		canData.rx_err = 0;
	} else if ((MSCAN_CANRFLG & MSCAN_CANRFLG_RSTAT_MASK) == 0x20) {
		canData.rx_warn = 0;
		canData.rx_err = 1;
	}

	if ((MSCAN_CANRFLG & MSCAN_CANRFLG_TSTAT_MASK) == 0x00) {
		canData.tx_warn = 0;
		canData.tx_err = 0;
	} else if ((MSCAN_CANRFLG & MSCAN_CANRFLG_TSTAT_MASK) == 0x04) {
		canData.tx_warn = 1;
		canData.tx_err = 0;
	} else if ((MSCAN_CANRFLG & MSCAN_CANRFLG_TSTAT_MASK) == 0x08) {
		canData.tx_warn = 0;
		canData.tx_err = 1;
	}

	if (((MSCAN_CANRFLG & MSCAN_CANRFLG_RSTAT_MASK) == 0x30)
			|| ((MSCAN_CANRFLG & MSCAN_CANRFLG_TSTAT_MASK) == 0x0C))
		canData.bus_off = 1;
	else
		canData.bus_off = 0;

	if ((MSCAN_CANRFLG & MSCAN_CANRFLG_OVRIF_MASK) == 0x02)
		canData.overflow = 1;
	else
		canData.overflow = 0;

	errorHandler(canData);

	MSCAN_CANRFLG |= MSCAN_CANRFLG_CSCIF_MASK;
	MSCAN_CANRFLG |= MSCAN_CANRFLG_OVRIF_MASK;

}

