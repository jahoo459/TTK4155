/*
 * Node2Main.c
 *
 * Created: 20.10.2017 08:46:42
 *  Author: olgakas
 */ 

#define NODE2

/*
=======================INCLUDES=========================
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include "..\..\..\v1\ProjectMain\definitions.h"
#include "..\..\..\v1\UARTlib\UARTlib.h"
#include "..\..\..\v1\SPILib\SPILib.h"
#include "..\..\..\v1\CANLib\CANLib.h"

/*
=======================PROGRAM=========================
*/

// GLOBAL VARIABLES

//UART COMMUNICATION
volatile char UART_ReceivedByte;
volatile uint8_t USARTreceivedFlag = 0;

//SPI COMMUNICATION
volatile uint8_t SPI_ReceivedByte;
volatile uint8_t SPIreceivedFlag = 0;



/*
=======================INTERRUPTS=========================
*/

ISR(USART0_RX_vect)
{
	//interrupt generated after receiving a byte over UART
	UART_ReceivedByte = UDR0;		//received byte
	USARTreceivedFlag = 1;		//set the flag to 1
}

ISR(MCP2515_INT)
{
	SPIreceivedFlag = 1;
}

void init()
{
	uartInit(BAUDRATE, FOSC, UBRR); printf("\n======================STARTUP==========================\n");
	SPI_init();
	MCP2515_init();
	CAN_init();
	
	sei();
}

int main(void)
{
	init();
	
	struct can_message message2send;
	message2send.id = 23;
	message2send.length = 8;
	message2send.data[0] = '@';
	message2send.data[1] = '~';
	message2send.data[2] = 'H';
	message2send.data[3] = 'l';
	message2send.data[4] = '1';
	message2send.data[5] = '.';
	message2send.data[6] = '/';
	message2send.data[7] = '5';
	CAN_sendMessage(&message2send, 0);
	_delay_ms(100);
	
    while(1)
    {
        //printf("Test node FOSC %ld\n", FOSC);
		
		if(SPIreceivedFlag)
		{
			uint8_t receiveBufferStatus;
			// check for message reception
			if(receiveBufferStatus = 0x03 & MCP2515_read(SS_CAN_CONTROLLER, MCP_CANINTF))
			{
				struct can_message receivedMessage;
				receivedMessage = CAN_receiveMessage(receiveBufferStatus);

				CAN_printMessage(&receivedMessage);
				
				SPIreceivedFlag = 0;
				
				
			}
		}
    }
}