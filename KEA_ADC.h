/*
 * KEA_ADC.h


 *
 *  Created on: Jun 23, 2016
 *      Author: jark
 */

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 * 
 * Call Module_ADC_Init first to intialize module
 * Enable desire channels with Module_ADC_EnableChannel giving it the resolution 8,10 or 12 bit and the channel number 0-15
 * call Module_ADC_Read to read from channel
 * 
 * 
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "derivative.h"
#include <stdbool.h>

#ifndef KEA_ADC_H_
#define KEA_ADC_H_

#define EIGHT_BIT 	0
#define TEN_BIT 	1
#define TWELVE_BIT 	2

typedef struct _ADC_Pin {

	uint8_t mode;
	uint8_t channel;
	uint16_t result;

}_ADC_Pin;

//Status
bool Module_ADC_Reading();

//Setup
void Module_ADC_Init();
void Module_ADC_EnableChannel(_ADC_Pin pin);

//Process
uint16_t Module_ADC_Read(_ADC_Pin pin);

#endif /* KEA_ADC_H_ */
