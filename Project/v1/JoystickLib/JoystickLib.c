/*
 * JoystickLib.c
 *
 * Created: 15.09.2017 11:44:47
 *  Author: janwh
 */ 

#include <avr/io.h>

#include "JoystickLib.h"
#include "..\ADCLib\ADCLib.h"
#include <util/delay.h>
#include <string.h>

// current position and direction of the joystick
JOY_position_t currentPosition;
JOY_direction_t currentDirection;

#define Y_channel 1
#define X_channel 2

// calibration
#define delay1 200
#define delay2 50
// variables needed for joystick calibration
volatile uint8_t calR = 200;
volatile uint8_t calL = 25;
volatile uint8_t calUp = 200;
volatile uint8_t calDown = 25;
// mean values for vertical and horizontal axis
volatile uint8_t meanVert = 128;
volatile uint8_t meanHor = 128;

//------------------------------------------------------------------------------
// set initial values for Position and Direction
void JOY_init()
{
	currentPosition.X_abs = 0;
	currentPosition.Y_abs = 0;
	currentPosition.X_per = 0;
	currentPosition.Y_per = 0;
	currentDirection = CENTRE;
}

//------------------------------------------------------------------------------
// This function tracks the maximum and minimum value on joystick x and y axis.
// After a predefined period of time the mean value for both axes is calculated.
void JOY_calibrate()
{
	uint8_t temp = 0;
	
	printf("Move JOY to the right\n");
	_delay_ms(delay1);
	
	for(int i = 0; i < 20; i++)
	{
		ADC_request(X_channel, adc_ext_ram);
		_delay_ms(delay2);
		temp = ADC_read(adc_ext_ram);
		
		if(temp > calR) {calR = temp;}
	}
	
	printf("Move JOY to the left\n");
	_delay_ms(delay1);
	
	for(int i = 0; i < 20; i++)
	{
		ADC_request(X_channel, adc_ext_ram);
		_delay_ms(delay2);
		temp = ADC_read(adc_ext_ram);
		
		if(temp < calL) {calL = temp;}
	}
	
	printf("Move JOY up\n");
	_delay_ms(delay1);
	
	for(int i = 0; i < 20; i++)
	{
		ADC_request(Y_channel, adc_ext_ram);
		_delay_ms(delay2);
		temp = ADC_read(adc_ext_ram);
		
		if(temp > calUp) {calUp = temp;}
	}
	
	printf("Move JOY down\n");
	_delay_ms(delay1);
	
	for(int i = 0; i < 20; i++)
	{
		ADC_request(Y_channel, adc_ext_ram);
		_delay_ms(delay2);
		temp = ADC_read(adc_ext_ram);
		
		if(temp < calDown) {calDown = temp;}
	}
	
	
	printf("calR: %d, calL: %d, calUp:%d, calDown:%d\n", calR, calL, calUp, calDown);
	
	meanVert = (calUp - calDown)/2;
	meanHor = (calR - calL)/2;
	
	printf("Calibration done.. new VM: %d HM:%d\n", meanVert, meanHor);
}

//------------------------------------------------------------------------------
// This function requests an AD conversion for the specified axis.
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

//------------------------------------------------------------------------------
// This function is called after the interrupt for completed conversion was
// received.
// 1.) Receive converted value X_abs from ADC and write them to the current
// position.
// 2.) Calculate relative value X_per for position (-100% .. 100%) and write
// them to the current position.
// 3.) Call method to calculate current direction.
void JOY_updatePosition(char axis)
{
	if(axis == 'x')
	{
		currentPosition.X_abs = ADC_read(adc_ext_ram);
		currentPosition.X_per = ((int)currentPosition.X_abs -  (int)meanHor)*100 / (int) meanHor;
	}

	else if(axis == 'y')
	{
		currentPosition.Y_abs = ADC_read(adc_ext_ram);
		currentPosition.Y_per = ((int)currentPosition.Y_abs -  (int)meanVert)*100 / (int) meanVert;
	}
	
	JOY_calculateDirection();
}

//------------------------------------------------------------------------------
// Calculate the current direction based on the relative value X_per.
// A direction is valid after the threshold of 50% was passed.
void JOY_calculateDirection()
{
	if(currentPosition.X_per > 50) {
	currentDirection = RIGHT;}
	else if(currentPosition.X_per < -50) {
	currentDirection = LEFT;}
	else if(currentPosition.Y_per > 50) {
	currentDirection = UP;}
	else if(currentPosition.Y_per < -50) {
	currentDirection = DOWN;}
	else{currentDirection = CENTRE;}
}

//------------------------------------------------------------------------------
//
JOY_position_t JOY_getPosition()
{
	return currentPosition;
}

//------------------------------------------------------------------------------
//
JOY_direction_t JOY_getDirection()
{
	return currentDirection;
}
