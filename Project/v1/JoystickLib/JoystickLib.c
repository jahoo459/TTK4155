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
JOY_position_t currentPosition;
JOY_direction_t currentDirection;

#define Y_channel 1
#define X_channel 2

//calibration
#define delay1 200
#define delay2 50
//variables needed for joystick calibration
volatile uint8_t calR = 200;
volatile  uint8_t calL = 25;
volatile  uint8_t calUp =200;
volatile  uint8_t calDown = 25;


volatile uint8_t meanVert = 128;
volatile uint8_t meanHor = 128;

void JOY_init()
{
	// set initial values for Position and Direction
	currentPosition.X_abs = 0;
	currentPosition.Y_abs = 0;
	currentPosition.X_per = 0;
	currentPosition.Y_per = 0;
	currentDirection = CENTRE;
}


char JOY_button(int button)
{
	
}

JOY_position_t JOY_getPosition()
{
	//calculate current position [in %]
	// 0 % corresponds to mean vert / hor value

	currentPosition.X_per = ((int)currentPosition.X_abs -  (int)meanHor)*100 / (int) meanHor;
	currentPosition.Y_per = ((int)currentPosition.Y_abs -  (int)meanHor)*100 / (int) meanHor;
	
	//printf("currentPosition.X = %d(per)\t currentPosition.Y = %d(per)\n", currentPosition.X_per, currentPosition.Y_per);
	
	return currentPosition;
}

uint8_t JOY_getPositionX()
{
	return currentPosition.X_abs;
}

uint8_t JOY_getPositionY()
{
	return currentPosition.Y_abs;
}

void JOY_calculateDirection()
{
	//calculate current direction, first update position
	JOY_getPosition();
	
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
		currentPosition.X_abs = ADC_read(adc_ext_ram);
	}

	else if(axis == 'y')
	{
		currentPosition.Y_abs = ADC_read(adc_ext_ram);
	}
	
	JOY_getPosition();
	JOY_calculateDirection();
}

void JOY_updateDirection()
{
	
}

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

void JOY_printPosAndDir()
{
	JOY_getPosition();
	JOY_calculateDirection();
	
	char directions[] = {'C', 'U', 'D', 'R', 'L'};
	char* dir;
	
	switch(currentDirection)
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
	
	printf("JOY: %s, X:%d, Y: %d\n", dir, currentPosition.X_per, currentPosition.Y_per);
}
