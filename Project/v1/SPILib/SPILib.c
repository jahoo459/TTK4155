/*
 * SPILib.c
 *
 * Created: 06.10.2017 11:22:41
 *  Author: lukasah
 */ 

#include <avr/io.h>
#include "..\SPILib\SPILib.h"
#include "..\ProjectMain\definitions.h"

#define SELECT_CAN_CONTROLLER_SLAVE clear_bit(PORTB, SS_CAN_CONTROLLER_PIN)
#define UNSELECT_CAN_CONTROLLER_SLAVE set_bit(PORTB, SS_CAN_CONTROLLER_PIN)


void SPI_init()
{
	printf("Initializing SPI");
	// Set _SS, MOSI and SCK output, all others input
	DDRB = (1<<SS_CAN_CONTROLLER_PIN) | (1<<PB5) | (1<<PB7);
	
	// Enable SPI, Master, set clock rate fck/16
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0);
	
	// set SS_CAN_CONTROLLER high for default (slave not selected)
	set_bit(PORTB, SS_CAN_CONTROLLER_PIN);
}

void SPI_send(uint8_t cData, SPI_SLAVES slave)
{
	activateSlave(slave);
	
	// Start transmission
	SPDR = cData;
	printf("Sendind %d\n", cData);
	// Wait for transmission complete
	while(!(SPSR & (1<<SPIF)));
	
	deactivateSlave(slave);
}

uint8_t SPI_receive(SPI_SLAVES slave)
{	
	activateSlave(slave);
	//To receive byte from the slave a dummy byte should be sent
	SPI_send(0, slave);
	
	// Wait for transmission complete
	while(!(SPSR & (1<<SPIF)));
	
	//read the buffer
	uint8_t data = SPDR;
	
	deactivateSlave(slave);
	return data;
}

void activateSlave(SPI_SLAVES slave)
{
	switch (slave)
	{
		case SS_CAN_CONTROLLER:
		SELECT_CAN_CONTROLLER_SLAVE;
		break;
	}
}

void deactivateSlave(SPI_SLAVES slave)
{
	switch (slave)
	{
		case SS_CAN_CONTROLLER:
		UNSELECT_CAN_CONTROLLER_SLAVE;
		break;
	}
}