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
#include "..\JoystickLib\JoystickLib.h"
#include "..\ExtSramLib\ExtSramLib.h"

#include <util/delay.h>
#include <UARTlib.h>




/*
=======================PROGRAM=========================
*/

// GLOBAL VARIABLES

//UART COMMUNICATION
volatile char receivedByte;
volatile uint8_t USARTreceivedFlag = 0;
volatile uint8_t ADCconversionCompletedFlag = 0;
volatile uint8_t JOYcalibFlag = 0;
volatile uint8_t LeftButtonFlag = 0;
volatile uint8_t RightButtonFlag = 0;

//ADC CONVERTER
uint8_t currentChannel = 1;


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
	receivedByte = UDR0;		//received byte
	USARTreceivedFlag = 1;		//set the flag to 1
}

ISR(INT0_vect)
{
	//interrupt generated on pin PD2 after the conversion in the ADC is completed
	ADCconversionCompletedFlag = 1;
}

ISR(INT1_vect)
{
	//interrupt generated on pin PD3 to start the joystick calibration
	JOYcalibFlag = 1;
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
	
	printf("Starting SRAM test...\n");
	
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
	
	printf("SRAM test completed with\n %4d errors in write phase and\n%4d errors in retrieval phase\n\n", write_errors, retrieval_errors);
}


/*
=======================MAIN FUNCTION=========================
*/

//#define ADDRESS 0x1400

int main(void)
{	
	uartInit(BAUDRATE, FOSC, UBRR);
	enableXMEM(1);
	
	//init external interrupt INT0 on falling edge
	set_bit(GICR, INT0);
	set_bit(MCUCR, ISC01);
	clear_bit(MCUCR, ISC00);
	
	//Pull-up on PD3
	clear_bit(DDRD, PD3);
	set_bit(PORTD, PD3);
	
	//init external interrupt INT1 on falling edge
	set_bit(GICR, INT1);
	set_bit(MCUCR, ISC11);
	clear_bit(MCUCR, ISC10);

	////init external interrupt INT2 on falling edge
	//set_bit(GICR, INT2);
	//clear_bit(EMCUCR, ISC2);
	
	//PE2
	clear_bit(DDRE, PE2);
	clear_bit(PORTE, PE2);
	
	//PB0
	clear_bit(DDRB, PB0);
	clear_bit(PORTB, PB0);
	

	
	sei();

	JOY_requestCurrentPosition('x');

    while(1)
    {	
		if((PINB & (1<<PB0)))
		{
			printf("Left button clicked ");
		}
		else if((PINE & (1<<PE2)))
		{
			printf("Right button clicked ");
		}
		
		if(JOYcalibFlag)
		{
			//run joystick calibration
			JOY_calibrate();
			JOYcalibFlag = 0;
		}
		
		
		JOY_printPosAndDir();
		
		if(ADCconversionCompletedFlag)
		{
			switch(currentChannel){
				case 1:	//X axis
					JOY_updatePosition('x');
					JOY_requestCurrentPosition('y');
					currentChannel++;
				break;	
				
				case 2:	//Y_axis
					JOY_updatePosition('y');
					JOY_requestCurrentPosition('x');
					currentChannel = 1;
				break;
				
				case 3: //slider_left
				
				break;
				
				case 4:	//slider_right
				
				break;
			}
		}

				
    }
}