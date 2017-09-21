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

volatile char *adc_ext_ram = (char*) ADC_EXT_RAM;

//current position and direction of the joystick
JOY_position_t currentJoyPosition;
JOY_direction_t currentJoyDirection;

#define Y_channel 1
#define X_channel 2

//calibration
#define delay1 200
#define delay2 50
//variables needed for joystick calibration
volatile uint8_t calR = 200;
volatile uint8_t calL = 25;
volatile uint8_t calUp =200;
volatile uint8_t calDown = 25;

volatile uint8_t meanVert = 128;
volatile uint8_t meanHor = 128;

//------------------------------------------------------------------------------
// set initial values for Position and Direction
void JOY_init()
{
	currentJoyPosition.X_abs = 0;
	currentJoyPosition.Y_abs = 0;
	currentJoyPosition.X_per = 0;
	currentJoyPosition.Y_per = 0;
	currentJoyDirection = CENTRE;
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
void JOY_requestcurrentPosition(char axis)
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
		currentJoyPosition.X_abs = ADC_read(adc_ext_ram);
		currentJoyPosition.X_per = ((int)currentJoyPosition.X_abs -  (int)meanHor)*100 / (int) meanHor;
	}

	else if(axis == 'y')
	{
		currentJoyPosition.Y_abs = ADC_read(adc_ext_ram);
		currentJoyPosition.Y_per = ((int)currentJoyPosition.Y_abs -  (int)meanHor)*100 / (int) meanHor;
	}

	// JOY_getPosition();
	JOY_calculateDirection();
}

//------------------------------------------------------------------------------
// Calculate the current direction based on the relative value X_per.
// A direction is valid after the threshold of 50% was passed.
void JOY_calculateDirection()
{
	//calculate current direction, first update position
	// JOY_getPosition();
	if(currentJoyPosition.X_per > 50) {
		currentJoyDirection = RIGHT;}
	else if(currentJoyPosition.X_per < -50) {
		currentJoyDirection = LEFT;}
	else if(currentJoyPosition.Y_per > 50) {
		currentJoyDirection = UP;}
	else if(currentJoyPosition.Y_per < -50) {
		currentJoyDirection = DOWN;}
	else{currentJoyDirection = CENTRE;}
}

//------------------------------------------------------------------------------
//
JOY_position_t JOY_getPosition()
{
	return currentJoyPosition;
}

//------------------------------------------------------------------------------
//
JOY_direction_t JOY_getDirection()
{
	return currentJoyDirection;
}

//------------------------------------------------------------------------------
// Output of the current position and direction of the joystick.
void JOY_printPosAndDir()
{
	// JOY_getPosition();
	// JOY_calculateDirection();

	char directions[] = {'C', 'U', 'D', 'R', 'L'};
	char* dir;

	switch(currentJoyDirection)
	{
		case 0:

		dir = "CENTER";
		break;

		case 1:
		dir = "UP";
		break;

		case 2:
		dir = "DOWN";
		break;

		case 3:
		dir = "RIGHT";
		break;

		case 4:
		dir = "LEFT";
		break;
	}

	printf("JOY: %s, X:%d, Y: %d\n", dir, currentJoyPosition.X_per, currentJoyPosition.Y_per);
}

// char JOY_button(int button)
// {
//
// }

// uint8_t JOY_getPositionX()
// {
// 	return currentJoyPosition.X_abs;
// }

// uint8_t JOY_getPositionY()
// {
// 	return currentJoyPosition.Y_abs;
// }


// void JOY_updateDirection()
// {
//
// }
