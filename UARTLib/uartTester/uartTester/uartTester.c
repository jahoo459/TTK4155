/*
 * uartTester.c
 *
 * Created: 01.09.2017 15:04:31
 *  Author: janwh
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <avr/sleep.h>

#define BAUDRATE 9600
#define FOSC  4915200
#define  F_CPU FOSC
#define UBRR FOSC/16/BAUDRATE-1
#include <avr/delay.h>

volatile unsigned char receivedByte;
volatile unsigned char receivedFlag = 0;

volatile unsigned int bufferIndex;


#define set_bit( reg, bit ) (reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))




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
	while ( !(UCSR0A & (1<<RXC0)) );
	/* Get and return received data from buffer */
	unsigned char receivedByte = UDR0;
	
	return receivedByte;
}


void initUart(unsigned int baud, unsigned int fosc)
{
	uint32_t ubrr = UBRR;
	
	/*Set BAUD rate*/
	UBRR0H = (unsigned char) (ubrr>>8);
	UBRR0L = (unsigned char) ubrr;
	
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	
	//Enable UART0 interrupts
	set_bit(UCSR0B, RXCIE0);
	
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);
	fdevopen(sendData, receiveData);
}

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
	
	initUart(BAUDRATE, FOSC);
	
	set_sleep_mode(SLEEP_MODE_PWR_SAVE);
	sei();
	
	printf("Welcome to Trondheim");

    while(1)
    {		
		if(receivedFlag == 1)
		{
			receivedFlag = 0;
			sendData(receivedByte);
		}

    }
}

	