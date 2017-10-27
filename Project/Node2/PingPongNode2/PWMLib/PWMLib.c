/*
 * PWMLib.c
 *
 * Created: 27.10.2017 09:27:48
 *  Author: lukasah
 */ 

#include "..\..\..\v1\ProjectMain\definitions.h"
#include "..\PWMLib\PWMLib.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define SERVO_MIN 250
#define SERVO_MAX 500

void PWM_init()
{
	// set Pin B5 to Output Mode
	DDRB |= (1<<DDB5);
	
	// setting Mode of Operation to FAST-PWM and Prescaler to div/64
	TCCR1A |= (1<<COM1A1) | (1<<WGM11);
	TCCR1B |= (1<<WGM13) | (1<<WGM12) | (1<<CS11) | (1<<CS10);
	
	// Overflow Interrupt Enable
	//TIMSK1 |= (1<<OCIE1A);
	TIMSK1 |= (1<<TOIE1);
	
	// 20ms PWM cycle
	ICR1 = 5000;
	
	// setting initial PWM Output to 1.5ms
	OCR1A = 375;
}

void PWM_setLevel(uint8_t level)
{
	if(level <= 100)
	{
		OCR1A = level*(SERVO_MAX-SERVO_MIN)/100 + SERVO_MIN;
	}
	else
	{
		printf("servo level over 100% - set to 100%\n");
		OCR1A = SERVO_MAX;
		
	}
}