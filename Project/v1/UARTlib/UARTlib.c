/*
 * UARTlib.c
 *
 * Created: 9/6/2017 12:22:53 PM
 *  Author: Jasiek
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <avr/sleep.h>

#include "UARTlib.h"

void uartSend(const char byte2Send)
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	/* Put data into buffer, sends the data */
	UDR0 = byte2Send;
}

unsigned char uartReceive()
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) );
	/* Get and return received data from buffer */
	unsigned char receivedByte = UDR0;
	
	return receivedByte;
}

void uartInit(uint32_t baud, uint32_t fosc, uint32_t ubrr)
{	
	/*Set BAUD rate*/
	UBRR0H = (unsigned char) (ubrr>>8);
	UBRR0L = (unsigned char) ubrr;
	
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	
	//Enable UART0 interrupts
	set_bit(UCSR0B, RXCIE0);
	
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);
	
	fdevopen(uartSend, uartReceive);
}