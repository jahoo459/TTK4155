/*
 * uartTester.c
 *
 * Created: 01.09.2017 15:04:31
 *  Author: janwh
 */ 

/*
=======================INCLUDES=========================
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
//#include <avr/sleep.h>


#define set_bit( reg, bit ) (reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))


#define BAUDRATE 9600
#define FOSC  4915200
#define  F_CPU FOSC
#define UBRR FOSC/16/BAUDRATE-1

#define uint16_t unsigned int

#include <util/delay.h>
#include <UARTlib.h>
#include <ExtSramLib.h>


/*
=======================PROGRAM=========================
*/

// GLOBAL VARIABLES

//UART COMMUNICATION
volatile char receivedByte;
volatile unsigned char receivedFlag = 0;

//XMEM addresses 
uint16_t ext_ram_size = 0x800;

volatile char *oled_cmd_ext_ram = (char*) 0x1000;
volatile char *oled_data_ext_ram = (char*) 0x1200;
volatile char *adc_ext_ram = (char*) 0x1400;
volatile char *sram_ext = (char*) 0x1800;



/*
=======================INTERRUPTS=========================
*/

ISR(USART0_RXC_vect)		
{
	//interrupt generated after receiving a byte over UART 
	receivedByte = UDR0;		//received byte
	receivedFlag = 1;		//set the flag to 1
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

#define ADDRESS 0x1000

int main(void)
{	
	uartInit(BAUDRATE, FOSC, UBRR);
	enableXMEM(1);
	
	sei();
	
	volatile unsigned char* pointer = (volatile unsigned char*)ADDRESS;

    while(1)
    {			
		//SRAM_test();
		
		*pointer = 1;
				
		if(receivedFlag == 1)
		{
			receivedFlag = 0;
			//SRAM_test();

			//*pointer = 1;
			
			//saveToAddress(pointer, (unsigned char)'b');
			//char test = readFromAddress(address);
			
		}

    }
}