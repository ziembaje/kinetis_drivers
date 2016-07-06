/*
 * KEA_GPIO.h
 *
 * Author: Jack Ziemba
 *
 * Description:
 *
 * This Module is used to initialize and interface with the registers
 * that support GPIO on the SKEAZ1284 Device.
 *
 *
 * Instructions:
 *
 * To Setup a Pin:
 * 		1) Instantiate a _Pin struct based on your specific Pin Details:
 * 			a) PinType, either Digital_In or Digital_Out
 * 			b) Port, PTx ex, PTA, PTB, PTC ... PTI
 * 			c) PortIndex, 0 to 7. ex PTA5, PortIndex = 5
 * 		2) Call the Module_GPIO_InitPin(_Pin Pin_Details) function with the _Pin struct
 * 		   that was just created.
 */

#ifndef LIB_KEA_GPIO_H_
#define LIB_KEA_GPIO_H_

typedef enum _PinConfigTypes {

	Digital_In, Digital_Out

} _PinConfigTypes;

typedef enum _PinRegisterBank {

	PTA, PTB, PTC, PTD, PTE, PTF, PTG, PTH, PTI

} _PinRegisterBank;

typedef struct _Pin {

	_PinConfigTypes PinType;		// Pin type, from above enumeration
	_PinRegisterBank Port;
	uint16_t PortIndex;   			// Register Index

} _Pin;

/* Description: Initializes the pin according to values in Pin_Details structure
 * Inputs: _Pin Structure.
 * */

void Module_GPIO_InitPin(_Pin Pin_Details);

/* Description: Writes variable 'state' to the pin contained by pin structure.
 * Inputs: _Pin Structure, integer state to write ( high = 1 , low = 0).
 * */
void Module_GPIO_PinWrite(_Pin Pin_Details, short state);

/* Description: Read the state of a pin.
 * Inputs: Pin Structure.
 * Returns: Value representing the state of the Pin. 1 = high , 0 = low.
 * */
short Module_GPIO_PinRead(_Pin Pin_Details);

#endif /* LIB_KEA_GPIO_H_ */

