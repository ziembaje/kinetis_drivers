/*
 * KEA_ADC.c

 *
 *  Created on: Jun 23, 2016
 *      Author: jark
 */

#include "KEA_ADC.h"

bool Module_ADC_Reading() {

	if ((ADC_SC2 & ADC_SC2_ADACT_MASK) == ADC_SC2_ADACT_MASK) {
		return true;
	} else {
		return false;
	}
}

void Module_ADC_Init() {

	SIM_SCGC |= SIM_SCGC_ADC_MASK; // enable bus clock
	ADC_SC3 |= ADC_SC3_ADICLK(0b00); // select bus speed
	ADC_SC2 |= 0x00; // software trigger, disable compare
	ADC_SC1 = 0; // enable ADC 0-15
	ADC_SC1 |= ADC_SC1_ADCO_MASK; // continious mode operation

}

void Module_ADC_EnableChannel(_ADC_Pin pin) {

	ADC_APCTL1 |= ADC_APCTL1_ADPC(1<<pin.channel);
	ADC_SC3 |= ADC_SC3_MODE(pin.mode); // 8, 10, or 12 bit mode
}

uint16_t Module_ADC_Read(_ADC_Pin pin) {

	ADC_SC1 |= ADC_SC1_ADCH(pin.channel); // select channel to read
	while (!(ADC_SC1 & ADC_SC1_COCO_MASK))
		; // wait until conversion has completed
	pin.result = ADC_R;
	return ADC_R ;

}

