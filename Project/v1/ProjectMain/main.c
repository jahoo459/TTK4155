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

//ANIMATION
volatile uint8_t timer0_increment = 0;
volatile uint8_t timer1_increment = 0;

/*
=======================INTERRUPTS=========================
*/

ISR(USART0_RXC_vect)
{
	//interrupt generated after receiving a byte over UART

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

ISR(TIMER0_COMP_vect)
{
	OLED_updateScreen();
	timer0_increment++;
	timer1_increment++;
	if(timer0_increment == 2)
	{
		OLED_setAnimationTick();
		timer0_increment = 0;
	}
}

 /*
 =======================FUNCTION DECLARATION=================
 */

void SRAM_test(void);
void init();

/*
=======================MAIN FUNCTION=========================
*/

int main(void)
{
	
	init();
	
    while(1)
    {
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
		
		if(activeState == GAME)
		{
			Game_init();
			
			Game_play(&SPIreceivedFlag, &updateCmdDispFlag, &uartMouseSteeringMessage, &inputMode);
			activeState = IDLE;
		}
    }
}
  
  
 /*
 =======================FUNCTION DEFINITIONS================
 */

 void SRAM_test(void)
 {
	 volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
	 uint16_t ext_ram_size = 0x800;
	 uint16_t write_errors= 0;
	 uint16_t retrieval_errors= 0;

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
 }
