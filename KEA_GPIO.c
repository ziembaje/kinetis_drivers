/*


 * KEA_GPIO.c

 *
 *  Created on: Mar 29, 2016
 *      Author: Jack Ziemba
 */

#include "SKEAZ1284.h"
#include "KEA_GPIO.h"

void Module_GPIO_InitPin(_Pin Pin_Details) {

	GPIO_MemMapPtr base;
	uint8_t offset = 0;

	switch (Pin_Details.Port) {

	case PTA:
		base = GPIOA_BASE_PTR;
		offset = 0;
		break;

	case PTB:
		base = GPIOA_BASE_PTR;
		offset += 8;
		break;

	case PTC:
		base = GPIOA_BASE_PTR;
		offset += 16;
		break;

	case PTD:
		base = GPIOA_BASE_PTR;
		offset += 24;
		break;

	case PTE:
		base = GPIOB_BASE_PTR;
		offset = 0;
		break;

	case PTF:
		base = GPIOB_BASE_PTR;
		offset += 8;
		break;

	case PTG:
		base = GPIOB_BASE_PTR;
		offset += 16;
		break;

	case PTH:
		base = GPIOB_BASE_PTR;
		offset += 24;
		break;

	case PTI:
		base = GPIOC_BASE_PTR;
		offset = 0;
		break;
	}

	offset += Pin_Details.PortIndex;

	if (Pin_Details.PinType == Digital_Out)
		GPIO_PDDR_REG(base) |= (1 << offset);
	else {
		GPIO_PDDR_REG(base) &= ~(1 << offset);
		GPIO_PIDR_REG(base) &= ~(1 << offset);
	}

}

void Module_GPIO_PinWrite(_Pin Pin_Details, short state) {

	GPIO_MemMapPtr base;
	uint8_t offset = 0;

	switch (Pin_Details.Port) {

	case PTA:
		base = GPIOA_BASE_PTR;
		offset = 0;
		break;

	case PTB:
		base = GPIOA_BASE_PTR;
		offset += 8;
		break;

	case PTC:
		base = GPIOA_BASE_PTR;
		offset += 16;
		break;

	case PTD:
		base = GPIOA_BASE_PTR;
		offset += 24;
		break;

	case PTE:
		base = GPIOB_BASE_PTR;
		offset = 0;
		break;

	case PTF:
		base = GPIOB_BASE_PTR;
		offset += 8;
		break;

	case PTG:
		base = GPIOB_BASE_PTR;
		offset += 16;
		break;

	case PTH:
		base = GPIOB_BASE_PTR;
		offset += 24;
		break;

	case PTI:
		base = GPIOC_BASE_PTR;
		offset = 0;
		break;
	}

	offset += Pin_Details.PortIndex;

	if (state == 0)
		GPIO_PDOR_REG(base) &= ~(1 << offset);
	else
		GPIO_PDOR_REG(base) |= (1 << offset);

}

short Module_GPIO_PinRead(_Pin Pin_Details) {

	GPIO_MemMapPtr base;
	uint8_t offset = 0;

	if (Pin_Details.PinType == Digital_In) //Verify that the Pin is an input
			{

		GPIO_MemMapPtr base;
		uint8_t offset = 0;

		switch (Pin_Details.Port) {

		case PTA:
			base = GPIOA_BASE_PTR;
			offset = 0;
			break;

		case PTB:
			base = GPIOA_BASE_PTR;
			offset += 8;
			break;

		case PTC:
			base = GPIOA_BASE_PTR;
			offset += 16;
			break;

		case PTD:
			base = GPIOA_BASE_PTR;
			offset += 24;
			break;

		case PTE:
			base = GPIOB_BASE_PTR;
			offset = 0;
			break;

		case PTF:
			base = GPIOB_BASE_PTR;
			offset += 8;
			break;

		case PTG:
			base = GPIOB_BASE_PTR;
			offset += 16;
			break;

		case PTH:
			base = GPIOB_BASE_PTR;
			offset += 24;
			break;

		case PTI:
			base = GPIOC_BASE_PTR;
			offset = 0;
			break;
		}

		offset += Pin_Details.PortIndex;

		return (GPIO_PDIR_REG(base) & (1 << offset))
				>> offset;

	}

	return -1;
}

