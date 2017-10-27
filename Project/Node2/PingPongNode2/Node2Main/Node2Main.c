/*
 * Node2Main.c
 *
 * Created: 20.10.2017 08:46:42
 *  Author: olgakas
 */ 

//#define NODE2

/*
=======================INCLUDES=========================
*/

#include <avr/interrupt.h>
#include "..\PWMLib\PWMLib.h"
#include "..\ADC2Lib\ADC2Lib.h"
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

//PWM
volatile uint8_t ServoFlag = 0;

//ADC
volatile uint8_t ADC2Flag = 0;

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

ISR(TIMER1_OVF_vect)
{
	ServoFlag = 1;

}

ISR(ADC_vect)
{
	ADC2_updateValue();
	printf("IRStatus: %d\n", ADC2_updateValue());
	set_bit(ADCSRA, ADSC);
}

void init()
{
	uartInit(BAUDRATE, FOSC, UBRR); printf("\n======================STARTUP=========================\n");
	SPI_init();
	MCP2515_init();
	CAN_init();
	PWM_init();
	ADC2_init();
	
	
	sei();
	set_bit(ADCSRA, ADSC); // start first ADC conversion
}

int main(void)
{
	init();
	
// 	struct can_message message2send;
// 	message2send.id = 23;
// 	message2send.length = 8;
// 	message2send.data[0] = '@';
// 	message2send.data[1] = '~';
// 	message2send.data[2] = 'H';
// 	message2send.data[3] = 'l';
// 	message2send.data[4] = '1';
// 	message2send.data[5] = '.';
// 	message2send.data[6] = '/';
// 	message2send.data[7] = '5';
// 	CAN_sendMessage(&message2send, 0);
// 	_delay_ms(100);
	JOY_direction_t currJoyDir;
	int currJoyPos = 0;
	
// 	uint8_t PWMcounter = 0;
// 	uint8_t PWMlevel = 50;
// 	uint8_t PWMdirection = 'r';
	
	uint16_t ADCresult = 0;
		
	
    while(1)
    {
		
		//if(ADC2Flag == 1)
		//{
			//ADCresult = ADCH;
			////ADCresult |= (ADCH<<2);
			//
			//printf("ADC result: %d\n", ADCresult);
			//
			//ADC2Flag = 0;
		//}
		//PWM_setLevel(50);
		//_delay_ms(1500);
		//PWM_setLevel(100);
		//_delay_ms(1500);
		//PWM_setLevel(50);
		//_delay_ms(1500);
		//PWM_setLevel(0);
		//_delay_ms(1500);
		//PWMcounter++;
		//if(PWMcounter > 1000)
		//{
			//printf("boing!\n");
			//PWMcounter = 0;
			//if(PWMdirection == 'r')
			//{
				//PWM_setLevel(PWMlevel++);
				//
				//if(PWMlevel == 100)
				//{
					//PWMdirection = 'l';
				//}
			//}
			//else if (PWMdirection == 'l')
			//{
				//PWM_setLevel(PWMlevel--);
				//
				//if(PWMlevel == 0)
				//{
					//PWMdirection = 'r';
				//}
			//}
			//else
			//{
				//printf("unknown value for 'PWMdirection'\n");
			//}
		//}
		
		if(SPIreceivedFlag)
		{
			uint8_t receiveBufferStatus;
			// check for message reception
			if(receiveBufferStatus = (0x03 & MCP2515_read(SS_CAN_CONTROLLER, MCP_CANINTF)))
			{
				struct can_message receivedMessage;
				receivedMessage = CAN_receiveMessage(receiveBufferStatus);
				//printf("%d\n", receivedMessage.data[0]);
				
				currJoyPos = receivedMessage.data[0]*100/255;
				printf("%d\n", currJoyPos);
				PWM_setLevel(currJoyPos);
				
// 				switch (currJoyDir)
// 				{
// 					case CENTRE:
// 					PWM_setLevel(50);
// 					break;
// 					
// 					case LEFT:
// 					PWM_setLevel(100);
// 					break;
// 					
// 					case RIGHT:
// 					PWM_setLevel(0);
// 					break;
// 				}
				
				
	
				

				//CAN_printMessage(&receivedMessage);
				
				SPIreceivedFlag = 0;
				
				
			}
		}
    }
}