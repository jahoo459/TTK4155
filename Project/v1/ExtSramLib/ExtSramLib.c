/*
 * ExtSramLib.c
 *
 * Created: 08.09.2017 08:57:08
 *  Author: janwh
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "ExtSramLib.h"

//BIT operations
#define set_bit( reg, bit ) (reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))


void enableXMEM(char releaseJtagPins)
{
	//enable XMEM
	set_bit(MCUCR, SRE);
	
	//set waiting to one cycle
	set_bit(EMCUCR, SRW01);
	
	if(releaseJtagPins != 0)
	{
		//disable 4 bits in port D from XMEM - needed for JTAG interface To release ports PC7 - PC4 -> XMM2 - 1, XMM1 - 0, XMM0 - 0
		set_bit(SFIOR, XMM2);
	}
}

unsigned char readFromAddress(volatile char* ptr)
{
	unsigned char val = *ptr;
	return val;
}

void saveToAddress(volatile char* ptr, unsigned char data)
{
	*ptr = data;
}


void disableXMEM()
{
	clear_bit(MCUCR, SRE);
}