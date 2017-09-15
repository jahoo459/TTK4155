/*
 * JoystickLib.c
 *
 * Created: 15.09.2017 11:44:47
 *  Author: janwh
 */ 

#include <avr/io.h>

#include "JoystickLib.h"
#include "..\ADCLib\ADCLib.h"

volatile char *adc_ext_ram = (char*) ADC_EXT_RAM;

//current position and direction of the joystick
JOY_position_t currentPosition;
JOY_direction_t currentDirection;

#define Y_channel 1
#define X_channel 2

void JOY_init()
{
	// set initial values for Position and Direction
	currentPosition.X = 0;
	currentPosition.Y = 0;
	currentDirection = CENTRE;
}

void JOY_calibrate()
{
	
}

char JOY_button(int button)
{
	
}

JOY_position_t JOY_getPosition()
{
	return currentPosition;
}

unsigned char JOY_getPositionX()
{
	return currentPosition.X;
}

unsigned char JOY_getPositionY()
{
	return currentPosition.Y;
}

JOY_direction_t JOY_getDirection()
{
	return currentDirection;
}

void JOY_requestCurrentPosition(char axis)
{
	if(axis == 'x')
	{
		ADC_request(X_channel, adc_ext_ram);
	}
	
	else if(axis == 'y')
	{
		ADC_request(Y_channel, adc_ext_ram);
	}
}

void JOY_updatePosition(char axis)
{
	if(axis == 'x')
	{
		currentPosition.X = ADC_read(adc_ext_ram);
	}

	else if(axis == 'y')
	{
		currentPosition.Y = ADC_read(adc_ext_ram);
	}
}

void JOY_updateDirection()
{
	
}
