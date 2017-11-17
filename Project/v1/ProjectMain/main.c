/*
 * uartTester.c
 *
 * Created: 01.09.2017 15:04:31
 *  Author: janwh
 */

/*
=======================INCLUDES=========================
*/
#include <avr/interrupt.h>
//#include "definitions.h"
// #include "..\JoystickLib\JoystickLib.h"
// #include "..\SliderLib\SliderLib.h"
#include "..\ExtSramLib\ExtSramLib.h"
#include "..\oledLib\oledLib.h"
#include "..\menuLib\menuLib.h"
#include "..\SPILib\SPILib.h"
#include "..\MCP2515Lib\MCP2515Lib.h"
#include "..\CANLib\CANLib.h"
#include "..\GameLib\GameLib.h"
#include <UARTlib.h>




/*
=======================PROGRAM=========================
*/

// GLOBAL VARIABLES

//UART COMMUNICATION
volatile char UART_ReceivedByte;
volatile uint8_t USARTreceivedFlag = 0;
volatile uint8_t waitForNextMessageFlag = 0;
volatile uint8_t pcCmdType = 0;

//PC Input
UART_Message_t uartMouseSteeringMessage;
uint8_t uartMsg;
static char str[3];
uint8_t updateCmdDispFlag = 0;

//INPUT MULTIFANCTION BOARD
volatile uint8_t ADCconversionCompletedFlag = 0;
volatile uint8_t JOYcalibFlag = 0;
volatile uint8_t LeftButtonFlag = 0;
volatile uint8_t RightButtonFlag = 0;

//MENU
volatile uint8_t activateMenuFlag = 0;
volatile OP_STATE activeState = IDLE;
volatile INPUT_MODE inputMode = SLIDER;
volatile menuAlreadyBuiltFlag = 0;

//SPI COMMUNICATION
volatile uint8_t SPI_ReceivedByte;
volatile uint8_t SPIreceivedFlag = 0;

//INITIALIZATION
uint16_t ext_ram_size = (uint16_t)EXT_RAM_SIZE;

volatile char *oled_cmd_ext_ram = (char*) OLED_CMD_EXT_RAM;
volatile char *oled_data_ext_ram = (char*) OLED_DATA_EXT_RAM;
volatile char *sram_ext = (char*) SRAM_EXT;
volatile char *adc_ext_ram = (char*) ADC_EXT_RAM;



/*
=======================INTERRUPTS=========================
*/

ISR(USART0_RXC_vect)
{
	//interrupt generated after receiving a byte over UART
	//UART_ReceivedByte = UDR0;	
	
	uartMsg = uartReceive();	//received byte

	if(waitForNextMessageFlag)
	{
		switch(pcCmdType)
		{
			case 0: //motor
				uartMouseSteeringMessage.Motor = uartMsg;
				waitForNextMessageFlag = 0;
				updateCmdDispFlag = 1;
				break;
			
			case 1: //servo
				uartMouseSteeringMessage.Servo = uartMsg;
				waitForNextMessageFlag = 0;
				updateCmdDispFlag = 1;
				break;
			
			case 2: //solenoid
				uartMouseSteeringMessage.Button = uartMsg;
				waitForNextMessageFlag = 0;
				updateCmdDispFlag = 1;
				break;
		}
	}
			
	if(uartMsg == 0xff) //Motor steering command received
	{
		waitForNextMessageFlag = 1; //wait for 3 steering command bytes	
		pcCmdType = 0;
	}
	
	if(uartMsg == 0xfe) //Servo steering command received
	{
		waitForNextMessageFlag = 1; //wait for 3 steering command bytes
		pcCmdType = 1;
	}
	
	if(uartMsg == 0xfd) //Solenoid steering command received
	{
		waitForNextMessageFlag = 1; //wait for 3 steering command bytes
		pcCmdType = 2;
	}
	
	USARTreceivedFlag = 1;		//set the flag to 1
}

ISR(INT0_vect)
{
	//interrupt generated on pin PD2 after the conversion in the ADC is completed
	ADCconversionCompletedFlag = 1;
}

ISR(INT1_vect)
{
	//interrupt generated on pin PD3 to start menu
	activateMenuFlag = 1;
}

ISR(MCP2515_INT)
{
	SPIreceivedFlag = 1;
}


/*
=======================FUNCTION DEFINITIONS=========================
*/

void SRAM_test(void)
{
	volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
	uint16_t ext_ram_size = 0x800;
	uint16_t write_errors= 0;
	uint16_t retrieval_errors= 0;

	//printf("Starting SRAM test...\n");

	// rand() stores some internal state, so calling this function in a loop will
	// yield different seeds each time (unless srand() is called before thisfunction)

	uint16_t seed = rand();

	// Write phase: Immediately check that the correct value was stored
	srand(seed);

	for (uint16_t i = 0; i < ext_ram_size; i++)
	{
		uint8_t some_value = rand();
		ext_ram[i] = some_value;
		uint8_t retreived_value = ext_ram[i];

		if (retreived_value != some_value)
		{
			//printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
			write_errors++;
		}

	}

	// Retrieval phase: Check that no values were changed during or after the write phase
	srand(seed);

	// reset the PRNG to the state it had before the write phase
	for (uint16_t i = 0; i < ext_ram_size; i++)
	{
		uint8_t some_value = rand();
		uint8_t retreived_value = ext_ram[i];

		if (retreived_value != some_value)
		{
			//printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
			retrieval_errors++;
		}
	}

	//printf("SRAM test completed with\n %4d errors in write phase and\n%4d errors in retrieval phase\n\n", write_errors, retrieval_errors);
}

void init()
{
	// call initialization subroutines
	uartInit(BAUDRATE, FOSC, UBRR); printf("\n======================STARTUP==========================\n");
	enableXMEM(1);
	SLI_init();
	JOY_init();
	OLED_init();
	SPI_init();
	MCP2515_init();
	CAN_init();
	
	// setup interrupts
	// init external interrupt INT0 on falling edge
	set_bit(GICR, INT0);
	set_bit(MCUCR, ISC01);
	clear_bit(MCUCR, ISC00);
	
	// Pull-up on PD3
	clear_bit(DDRD, PD3);
	set_bit(PORTD, PD3);
	
	// init external interrupt INT1 on falling edge
	set_bit(GICR, INT1);
	set_bit(MCUCR, ISC11);
	clear_bit(MCUCR, ISC10);
	
		
	// PE2
	clear_bit(DDRE, PE2);
	clear_bit(PORTE, PE2);
	// PB0
	clear_bit(DDRB, PB0);
	clear_bit(PORTB, PB0);

	// activate interrupts
	sei();

	// call SRAM Test
	//SRAM_test();
}

// print status variables of Multifunction Board
//void statusMultifunctionBoard(){
	//JOY_position_t currentJoyPosition;
	//currentJoyPosition = JOY_getPosition();
//
	//JOY_direction_t currentJoyDirection;
	//currentJoyDirection = JOY_getDirection();
//
	//SLI_position_t currentSliPosition;
	//currentSliPosition = SLI_getPosition();
//
	//uint8_t leftButton = 0;
	//uint8_t rightButton = 0;
//
	////char directions[] = {'C', 'U', 'D', 'R', 'L'};
	//char* dir;
//
	//if((PINB & (1<<PB0)))
		//{
			////printf("Left button clicked ");
			//leftButton = 1;
		//}
		//else if((PINE & (1<<PE2)))
		//{
			////printf("Right button clicked ");
			//rightButton = 1;
		//}
//
	//switch(currentJoyDirection)
	//{
		//case 0:
//
		//dir = "CENTER";
		//break;
//
		//case 1:
		//dir = "UP";
		//break;
//
		//case 2:
		//dir = "DOWN";
		//break;
//
		//case 3:
		//dir = "RIGHT";
		//break;
//
		//case 4:
		//dir = "LEFT";
		//break;
	//}
//
	////printf("JOY: %s, X:%d, Y: %d \t\t SLI_l:%d, SLI_r:%d \t l_Btn: %d, r_Btn: %d\n", dir, currentJoyPosition.X_per, currentJoyPosition.Y_per, currentSliPosition.L_per, currentSliPosition.R_per, leftButton, rightButton);
	////printf("JOY: %s, X:%d, Y: %d\n", dir, currentJoyPosition.X_per, currentJoyPosition.Y_per);
//}



/*
=======================MAIN FUNCTION=========================
*/

int main(void)
{
	
	init();

	
	//for(uint8_t i = 10; i < 64; i+=8)
	//{
		//OLED_line(0,i,127,i,1);
	//}
	//
	//OLED_line(0,1,0,7,1);
	
// 	for(uint8_t i = 0; i<128; i+=8)
// 	{
// 		if(i<=73)
// 		{
// 			OLED_line(i,1,i,7,1);
// 		}
// 		else
// 		{
// 			OLED_line(i,0,i,7,1);
// 		}
// 	}
	
// 	struct can_message message2send;
// 
// 	
// 	static uint8_t JoyPos;	
// 	static uint8_t SliPos;
// 	static uint8_t ButtonRight;
	
	//activateMenuFlag = 1; // display the main menu
		
	
	

	uartMsg = 0;
	uartMouseSteeringMessage.Motor = 0;
	uartMouseSteeringMessage.Servo = 0;
	uartMouseSteeringMessage.Button = 0;
	
    while(1)
    {

// 		JoyPos = JOY_getPosition().X_abs;
// 		SliPos = SLI_getPosition().R_per;
// 		if((PINE & (1<<PE2)))
// 		{
// 			ButtonRight = 1;
// 		}
// 		else
// 		{
// 			ButtonRight = 0;
// 		}
		//printf("%d\n", SliPos);
		
// 		message2send.id = 23;
// 		message2send.length = 3;
// 		message2send.data[0] = JoyPos;
// 		message2send.data[1] = SliPos;
// 		message2send.data[2] = ButtonRight;
// 		
// 		CAN_sendMessage(&message2send, 0);
// 		_delay_ms(50);

		if(activateMenuFlag && menuAlreadyBuiltFlag == 0)
		{
			activeState = MENU;
			MENU_activate(&activeState, &inputMode);

			menuAlreadyBuiltFlag = 1;
			activateMenuFlag = 0;
		}

		if(activateMenuFlag && menuAlreadyBuiltFlag == 1)
		{
			activeState = MENU;
			MENU_reactivate();	
			activateMenuFlag = 0;
		}
		
// 		if(SPIreceivedFlag)
// 		{
// 			
// 			uint8_t receiveBufferStatus;
// 			// check for message reception
// 			if(receiveBufferStatus = 0x03 & MCP2515_read(SS_CAN_CONTROLLER, MCP_CANINTF))
// 			{
// 				struct can_message receivedMessage;
// 				receivedMessage = CAN_receiveMessage(receiveBufferStatus);
// 
// 				CAN_printMessage(&receivedMessage);
// 				
// 				if(receivedMessage.id == 21)
// 				{
// 					Game_updateLives();
// 				}
// 				
// 			
// 				SPIreceivedFlag = 0;
// 			
// 			}
// 		}
		
// 		if(updateCmdDispFlag)
// 		{
// 			OLED_clear();
// 			sprintf(str, "%d", uartMouseSteeringMessage.Motor);
// 			OLED_goto(0,0);
// 			OLED_printString(str);
// 			
// 			sprintf(str, "%d", uartMouseSteeringMessage.Servo);
// 			OLED_goto(1,0);
// 			OLED_printString(str);
// 			
// 			sprintf(str, "%d", uartMouseSteeringMessage.Button);
// 			OLED_goto(2,0);
// 			OLED_printString(str);
// 				
// 			updateCmdDispFlag = 0;
// 		}
		
		if(activeState == GAME)
		{
			Game_init();
			Game_play(&SPIreceivedFlag, &updateCmdDispFlag, &uartMouseSteeringMessage, &inputMode);
			activeState = IDLE;
		}
    }
}
  