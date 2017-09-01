/*
 * UARTLib.c
 *
 * Created: 01.09.2017 14:12:52
 *  Author: janwh
 */ 

#include <avr/io.h>
#include <stdio.h>

void initUart(unsigned int baud, unsigned int fosc)
{
	#define BAUD baud
	#define FOSC fosc
	
	#define MYUBRR FOSC/16/BAUD-1
	
	unsigned int ubrr = MYUBRR; 
	
	/*Set BAUD rate*/
	UBRR0H = (unsigned char) (ubrr>>8);
	UBRR0L = (unsigned char) ubrr; 
	
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);
}

void sendData(unsigned char byte2Send)
{
    
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	/* Put data into buffer, sends the data */
	UDR0 = byte2Send;
}

unsigned char receiveData()
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) )
	;
	/* Get and return received data from buffer */
	unsigned char receivedByte = UDR0;	
	
	return receivedByte;
} 
