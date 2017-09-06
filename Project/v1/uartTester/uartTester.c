/*
 * uartTester.c
 *
 * Created: 01.09.2017 15:04:31
 *  Author: janwh
 */ 

/*
=======================INCLUDES=========================
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <avr/sleep.h>

#define set_bit( reg, bit ) (reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))


#define BAUDRATE 9600
#define FOSC  4915200
#define  F_CPU FOSC
#define UBRR FOSC/16/BAUDRATE-1
#include <util/delay.h>


#include <UARTlib.h>


/*
=======================PROGRAM=========================
*/

volatile char receivedByte;
volatile unsigned char receivedFlag = 0;

ISR(USART0_RXC_vect)
{
	//interrupt generated after receiving a byte
	receivedByte = UDR0;		//received byte
	receivedFlag = 1;		//set the flag to 1
}



int main(void)
{	
	set_bit(DDRB, PB0);
	clear_bit(PORTB, PB0);
	
	uartInit(BAUDRATE, FOSC, UBRR);
	
	set_sleep_mode(SLEEP_MODE_PWR_SAVE);
	sei();

    while(1)
    {		
		if(receivedFlag == 1)
		{
			receivedFlag = 0;
			uartSend(receivedByte, NULL);
			printf(receivedByte);
		}

    }
}