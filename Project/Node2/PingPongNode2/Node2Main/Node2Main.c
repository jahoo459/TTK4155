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
#include "..\SolenoidLib\SolenoidLib.h"

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
volatile uint8_t prevRightButtonState = 0;

//waiting mode counter 3s
volatile uint16_t waitingModeTimer = WAITING_TIME;
volatile uint8_t lifeCounter = 3;

//MOTOR CONTROLLER
uint8_t doPID_Flag = 0;

// Time interval counter (160ms) and flag
uint8_t timeIntCounter = 0;
uint8_t timeIntFlag = 0;

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

ISR(TIMER1_OVF_vect) //20ms timer, used for PWM, IR ball detection and PID
{
	ServoFlag = 1;
	doPID_Flag = 1;
		
	//handle the waiting mode timer
	if(waitingMode)
	{
		waitingModeTimer -= 1;
	}
	
	if(waitingModeTimer == 0 && IR_detection_status == 0)
	{
		waitingMode = 0;
		waitingModeTimer = WAITING_TIME;
		
		// sending message to resume playing
		struct can_message resumePlaying;
		resumePlaying.id = 13;
		resumePlaying.length = 1;
		resumePlaying.data[0] = 1;
		CAN_sendMessage(&resumePlaying, 0);
	}
	else
	{
		if(waitingModeTimer == 0)
		{
			waitingModeTimer = WAITING_TIME;	
		}
	}
	
	// set the time interval flag 20*8=160
	if(timeIntCounter == 8)
	{
		timeIntFlag = 1;
		timeIntCounter = 0;
	}
	
	timeIntCounter += 1;	
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
						
			// send that a live was lost
			struct can_message message2sendLifeCounter;
			message2sendLifeCounter.id = 21;
			message2sendLifeCounter.length = 1;
			message2sendLifeCounter.data[0] = IR_detection_status;
			CAN_sendMessage(&message2sendLifeCounter, 0);
		}
	}

}

ISR(TIMER0_OVF_vect)
{
	
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
	Solenoid_init();
	
	sei();
	set_bit(ADCSRA, ADSC); // start first ADC conversion
}

int main(void)
{
	init();
	//JOY_direction_t currJoyDir;
	uint8_t JoyPos = 0;
	uint8_t SliPos = 100;
	uint8_t ButtonRight = 0;
	uint8_t Motor = 100;
	uint8_t Servo = 0;
	uint8_t Solenoid = 0;
	uint8_t inputMode = 0;
	
	uint16_t ADCresult = 0;
		
	
    while(1)
    {
		//printf("Encoder val: %d \n", Motor_readEncoder());
		if(doPID_Flag)
		{
			//printf("Encoder read: %d \t", Motor_readEncoder());
			if (timeIntFlag == 1)
			{
				Motor_do_PID(SliPos, Motor_readEncoder());
				doPID_Flag = 0;
				timeIntFlag = 0;
			}
						
		}
		if(SPIreceivedFlag)
		{
			uint8_t receiveBufferStatus;
			// check for message reception
			if(receiveBufferStatus = (0x03 & MCP2515_read(SS_CAN_CONTROLLER, MCP_CANINTF)))
			{
				struct can_message receivedMessage;
				receivedMessage = CAN_receiveMessage(receiveBufferStatus);
				//printf("%d\n", receivedMessage.data[0]);
				if(receivedMessage.id == 23)
				{
					JoyPos = receivedMessage.data[0]*100/255;
 					PWM_setLevel(JoyPos);
					SliPos = receivedMessage.data[1];
					ButtonRight = receivedMessage.data[2];	
					Motor = receivedMessage.data[3];
					Servo = receivedMessage.data[4];
					Solenoid = receivedMessage.data[5];
					inputMode = receivedMessage.data[6];
				
					// use solenoid
					if(ButtonRight != prevRightButtonState)
					{
						if(ButtonRight == 1)
						{
							Solenoid_TurnOn();
						}
						else
						{
							Solenoid_TurnOff();
						}
					}
				
					printf("%d \t %d \t %d \t %d \t %d \t %d \t %d\n", JoyPos, SliPos, ButtonRight, Motor, Servo, Solenoid, inputMode);
				
		
					prevRightButtonState = ButtonRight;
				}
// 				if(receivedMessage.id == 2)
// 				{
// 					printf("%d\n", receivedMessage.data[0]);
// 				}
				
				SPIreceivedFlag = 0;
				
			}
		}
    }
}