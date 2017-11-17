/*
 * ADC2Lib.c
 *
 * Created: 27.10.2017 17:35:15
 *  Author: lukasah
 */ 

#include "ADC2Lib.h"
#include <avr/io.h>

static uint8_t oldADCvalues[4];
static uint8_t IRStatus;
static uint8_t threshold = 20;

void ADC2_init()
{
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Set ADC prescaler to 128 - 125KHz sample rate @ 16MHz

	ADMUX |= (1 << REFS0); // Set ADC reference to AVCC
	ADMUX |= (1 << ADLAR); // Left adjust ADC result to allow easy 8 bit reading

	// No MUX values needed to be changed to use ADC0

	ADCSRA |= (1 << ADIE);  // Set ADC to Free-Running Mode
	ADCSRA |= (1 << ADEN);  // Enable ADC
	
	for(uint8_t i = 0; i < 4; i++)
	{
		oldADCvalues[i] = 100;
	}
	
	IRStatus = 100;
}

uint8_t ADC2_updateValue()
{
	uint16_t temp;
	
	oldADCvalues[0] = ADCH;
	
	temp = (oldADCvalues[0] + oldADCvalues[1] + oldADCvalues[2] + oldADCvalues[3])/4;
	IRStatus = temp;
	
	for(uint8_t i = 0; i < 3; i++)
	{
		oldADCvalues[3-i] = oldADCvalues[2-i];
	}
	
	if(IRStatus < threshold)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}