/*
 * ADCLib.c
 *
 * Created: 15.09.2017 11:20:00
 *  Author: janwh
 */

 //-----------------------------------------------------------------------------
 // this driver implements the communication between Joystick Library and the
 // AD Converter

#include <avr/io.h>

#include "ADCLib.h"

//------------------------------------------------------------------------------
// definition of the channel addresses
//
// CH NO		BINARY		HEX			DESCRIPTION
//
// ch1			0b0100		0x04		y-axis
// ch2			0b0101		0x05		x-axis
// ch3			0b0110		0x06		left slider
// ch4			0b0111		0x07		right slider

#define ch1	0x04
#define ch2 0x05
#define ch3 0x06
#define ch4 0x07

//------------------------------------------------------------------------------
// what has to be initialized?
void ADC_init()
{
	//TBD
}

//------------------------------------------------------------------------------
// this function requests a AD conversion on the specified channel
void ADC_request(uint8_t channel, volatile char *ADC_ADDRESS){

	switch(channel){
		case 1:
			saveToAddress(ADC_ADDRESS, (uint8_t)ch1);
			break;

		case 2:
			saveToAddress(ADC_ADDRESS, (uint8_t)ch2);
			break;

		case 3:
			saveToAddress(ADC_ADDRESS, (uint8_t)ch3);
			break;

		case 4:
			saveToAddress(ADC_ADDRESS, (uint8_t)ch4);
			break;

		default:
			printf("Problems with ADC...");
			break;

	}
}

//------------------------------------------------------------------------------
// this function reads the result from the ADC after a completed conversion
uint8_t ADC_read(volatile char *ADC_ADDRESS)
{
	return (uint8_t)readFromAddress(ADC_ADDRESS);
}
