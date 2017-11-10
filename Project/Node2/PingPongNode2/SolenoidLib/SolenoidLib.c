/*
 * SolenoidLib.c
 *
 * Created: 10.11.2017 10:12:34
 *  Author: lukasah
 */ 

#include <avr/io.h>
#include "SolenoidLib.h"

void Solenoid_init()
{
	// set Pin PA1 to Output Mode
	set_bit(DDRA, DDA1);
}

void Solenoid_TurnOn()
{
	clear_bit(PORTA, PA1);
}

void Solenoid_TurnOff()
{
	set_bit(PORTA, PA1);
}