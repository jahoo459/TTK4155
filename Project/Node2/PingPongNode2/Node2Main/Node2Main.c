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
#include "..\MotorLib\MotorLib.h"

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

//Game Flags
uint8_t IR_detection_status = 0;
volatile uint8_t waitingMode = 0;
//waiting mode counter 3s
volatile uint16_t waitingModeTimer = WAITING_TIME;
volatile uint8_t lifeCounter = 3;


/*
=======================INTERRUPTS=========================
*/

ISR(USART0_RX_vect)
{
	//interrupt generated after receiving a byte over UART
	UART_ReceivedByte = UDR0;		//received byte
	USARTreceivedFlag = 1;		//set the flag to 1
}

ISR(MCP2515_INT) //CAN message received
{
	SPIreceivedFlag = 1;
}

ISR(TIMER1_OVF_vect) //PWM
{
	ServoFlag = 1;
	
	//handle the waiting mode timer
	if(waitingMode)
	{
		waitingModeTimer -= 1;
	}
	
	if(waitingModeTimer == 0 && IR_detection_status == 0)
	{
		waitingMode = 0;
		waitingModeTimer = WAITING_TIME;
		printf("Continue PLEASE! \n");
	}
	else
	{
		if(waitingModeTimer == 0)
		{
			waitingModeTimer = WAITING_TIME;	
		}
	}
}

ISR(ADC_vect) //IR Diodes - counting points
{
	IR_detection_status = ADC2_updateValue();
	set_bit(ADCSRA, ADSC);
	
	if(waitingMode == 0)
	{
		if(IR_detection_status == 1) //ball detected, start the waiting mode
		{
			waitingMode = 1;
			lifeCounter -= 1;
			printf("Life left: %d\n", lifeCounter);
			
			if(lifeCounter == 0)
			{
				printf("GAME OVER! \n");
			}
		}
	}

}

void init()
{
	uartInit(BAUDRATE, FOSC, UBRR); printf("\n====================STARTUP=======================\n");
	SPI_init();
	MCP2515_init();
	CAN_init();
	PWM_init();
	ADC2_init();
	Motor_init();
	
	sei();
	set_bit(ADCSRA, ADSC); // start first ADC conversion
}

int main(void)
{
	init();
	JOY_direction_t currJoyDir;
	int currJoyPos = 0;
	
	uint16_t ADCresult = 0;
		
	
    while(1)
    {
		
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
				//printf("%d\n", currJoyPos);
				PWM_setLevel(currJoyPos);
				Motor_setSpeed(currJoyPos);
					
				SPIreceivedFlag = 0;
				
				printf("Encoder val: %d \n", Motor_readEncoder());
			}
		}
    }
}