/*
 * SliderLib.c
 *
 * Created: 15.09.2017 17:46:32
 *  Author: janwh
 */ 

#include <avr/io.h>
#include "SliderLib.h"
#include "..\ADCLib\ADCLib.h"

// channels of sliders for AD conversion
#define L_channel 3
#define R_channel 4

// current position of sliders
SLI_position_t currentSliPosition;

// startup phase
uint8_t SLI_startup = 1; 

// set initial values for Position and Buttons
void SLI_init()
{
	currentSliPosition.R_abs = 0;
	currentSliPosition.L_abs = 0;
	currentSliPosition.R_per = 0;
	currentSliPosition.L_per = 0;
}

//------------------------------------------------------------------------------
// This function requests an AD conversion for the specified slider.
void SLI_requestCurrentPosition(uint8_t side)
{
	if(side == 'l')
	{
		ADC_request(L_channel, adc_ext_ram);
	}

	else if(side == 'r')
	{
		ADC_request(R_channel, adc_ext_ram);
	}
}

//------------------------------------------------------------------------------
// This function is called after the interrupt for completed conversion was
// received.
void SLI_updatePosition(uint8_t side)
{
	if(side == 'l')
	{
		currentSliPosition.L_abs = ADC_read(adc_ext_ram);
		currentSliPosition.L_per = currentSliPosition.L_abs * 100 / 256;
		//printf("CurrSliPos_L: %d, CurrSliPos_R: %d\n", currentSliPosition.L_per, currentSliPosition.R_per);
	}

	else if(side == 'r')
	{
		currentSliPosition.R_abs = ADC_read(adc_ext_ram);
		currentSliPosition.R_per = currentSliPosition.R_abs * 100 / 256;
		//printf("CurrSliPos_L: %d, CurrSliPos_R: %d\n", currentSliPosition.L_per, currentSliPosition.R_per);
	}
}

//
SLI_position_t SLI_getPosition()
{
	SLI_requestCurrentPosition('r');
	_delay_us(60);
	SLI_updatePosition('r');
	
	return currentSliPosition;
	
}