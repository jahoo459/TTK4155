/*
 * JoystickLib.c
 *
 * Created: 15.09.2017 11:44:47
 *  Author: janwh
 */ 

#include <avr/io.h>

#include "JoystickLib.h"
#include "..\ADCLib\ADCLib.h"
//#include <util/delay.h>
#include <string.h>

// current position and direction of the joystick
static JOY_position_t currentPosition;
static JOY_direction_t currentDirection;

// channels for Joy axes used by ADC
#define Y_channel 1
#define X_channel 2

// delay in us after which the AD conversion should be finished
// todo: handle conversion with interrupts
#define delayConversion 60

// variables needed for joystick calibration
// delay for sampling in ms
#define delay1 200
#define delay2 75
// thresholds for directions
static volatile uint8_t calR = 200;
static volatile uint8_t calL = 25;
static volatile uint8_t calUp = 200;
static volatile uint8_t calDown = 25;
// default mean values for vertical and horizontal axis
static volatile uint8_t meanVert = 128;
static volatile uint8_t meanHor = 128;

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
	
	OLED_clear();
	OLED_bufferGoto(0,0);
	OLED_printString("JOY RIGHT");
	//printf("Move JOY to the right\n");
	_delay_ms(delay1);
	
	for(int i = 0; i < 20; i++)
	{
		ADC_request(X_channel, adc_ext_ram);
		_delay_ms(delay2);
		temp = ADC_read(adc_ext_ram);
		
		if(temp > calR) {calR = temp;}
	}
	
	OLED_bufferGoto(1,0);
	OLED_printString("JOY LEFT");
	//printf("Move JOY to the left\n");
	_delay_ms(delay1);
	
	for(int i = 0; i < 20; i++)
	{
		ADC_request(X_channel, adc_ext_ram);
		_delay_ms(delay2);
		temp = ADC_read(adc_ext_ram);
		
		if(temp < calL) {calL = temp;}
	}
	
	OLED_bufferGoto(2,0);
	OLED_printString("JOY UP");
	//printf("Move JOY up\n");
	_delay_ms(delay1);
	
	for(int i = 0; i < 20; i++)
	{
		ADC_request(Y_channel, adc_ext_ram);
		_delay_ms(delay2);
		temp = ADC_read(adc_ext_ram);
		
		if(temp > calUp) {calUp = temp;}
	}
	
	OLED_bufferGoto(3,0);
	OLED_printString("JOY DOWN");
	//printf("Move JOY down\n");
	_delay_ms(delay1);
	
	for(int i = 0; i < 20; i++)
	{
		ADC_request(Y_channel, adc_ext_ram);
		_delay_ms(delay2);
		temp = ADC_read(adc_ext_ram);
		
		if(temp < calDown) {calDown = temp;}
	}
	
	
	//printf("calR: %d, calL: %d, calUp:%d, calDown:%d\n", calR, calL, calUp, calDown);
	
	meanVert = (calUp - calDown)/2;
	meanHor = (calR - calL)/2;
	
	OLED_bufferGoto(4,0);
	OLED_printString("CALIB DONE");
	//printf("Calibration done.. new VM: %d HM:%d\n", meanVert, meanHor);
	_delay_ms(1000);
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
}

//------------------------------------------------------------------------------
// Calculate the current direction based on the relative value X_per.
// A direction is valid after the threshold of 50% was passed.
void JOY_calculateDirection()
{
	if(currentPosition.X_per > 90) {
	currentDirection = RIGHT;}
	else if(currentPosition.X_per < -90) {
	currentDirection = LEFT;}
	else if(currentPosition.Y_per > 90) {
	currentDirection = UP;}
	else if(currentPosition.Y_per < -90) {
	currentDirection = DOWN;}
	else{currentDirection = CENTRE;}
}

//------------------------------------------------------------------------------
// This function returns the current position of the Joystick.
// It is a struct with the four parameters X_abs, Y_abs, X_per and Y_per
JOY_position_t JOY_getPosition()
{
	JOY_requestCurrentPosition('x');
	_delay_us(delayConversion);
	JOY_updatePosition('x');
	JOY_requestCurrentPosition('y');
	_delay_us(delayConversion);
	JOY_updatePosition('y');
	return currentPosition;
}

//------------------------------------------------------------------------------
// This function returns the current Direction of the Joystick in the form of
// text.
// Before the value is returned, the values of position in x and y direction are 
// requested from the ADC and written to the currentPosition and 
// currentDirection variables.
JOY_direction_t JOY_getDirection()
{
	JOY_requestCurrentPosition('x');
	_delay_us(delayConversion);
	JOY_updatePosition('x');
	JOY_requestCurrentPosition('y');
	_delay_us(delayConversion);
	JOY_updatePosition('y');
	JOY_calculateDirection();
	
	return currentDirection;
}

// old AD conversion handling in the main method
/*
if(ADCconversionCompletedFlag)
{
	switch(currentChannel){
		case 1:	//X axis
			JOY_updatePosition('x');
			JOY_requestCurrentPosition('y');
			currentChannel++;
		break;	
				
		case 2:	//Y_axis
			JOY_updatePosition('y');
			SLI_requestCurrentPosition('l');
			currentChannel++;
		break;
				
		case 3: //slider_left
			SLI_updatePosition('l');
			SLI_requestCurrentPosition('r');
			currentChannel++;
		break;
				
		case 4:	//slider_right
			SLI_updatePosition('r');
			JOY_requestCurrentPosition('x');
			currentChannel = 1;
		break;
	}
}
*/