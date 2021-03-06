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
	//init external interrupt INT2 on falling edge
	#if defined(__AVR_ATmega162__)
		set_bit(GICR, INT2);
		clear_bit(EMCUCR, ISC2);
		
		// Set _SS, MOSI and SCK output, all others input
		DDR_SPI |= (1<<SS_CAN_CONTROLLER_PIN_MODE) | (1<<MOSI_PIN) | (1<<SCK_PIN);
	#endif
	
	#if defined(__AVR_ATmega2560__)
		set_bit(EIMSK, INT4);
		set_bit(EICRB, ISC41);
		clear_bit(EICRB, ISC40);
		
		// Set _SS, PB7 (real SS), MOSI and SCK output, all others input
		DDR_SPI |= (1<<DDB0) | (1<<SS_CAN_CONTROLLER_PIN_MODE) | (1<<MOSI_PIN) | (1<<SCK_PIN);
	#endif
	
	// Enable SPI, Master, set clock rate fck/4
	SPCR = (1<<SPE) | (1<<MSTR);
	
	// clock rate fck/2
	SPSR = (1<<SPI2X);

	// set SS_CAN_CONTROLLER high for default (slave not selected)
	set_bit(PORT_SPI, SS_CAN_CONTROLLER_PIN);
}

void SPI_send(uint8_t cData)
{
	// Start transmission
	SPDR = cData;
	// Wait for transmission complete
	while(!(SPSR & (1<<SPIF)));
}

uint8_t SPI_receive()
{
	//To receive byte from the slave a dummy byte should be sent
	SPI_send((uint8_t)0xff);

	// Wait for transmission complete
	while(!(SPSR & (1<<SPIF)));

	//read the buffer
	uint8_t data = SPDR;

	return data;
}

void SPI_activateSlave(SPI_SLAVES slave)
{
	switch (slave)
	{
		case SS_CAN_CONTROLLER:
		SELECT_CAN_CONTROLLER_SLAVE;
		break;
	}
}

void SPI_deactivateSlave(SPI_SLAVES slave)
{
	switch (slave)
	{
		case SS_CAN_CONTROLLER:
		UNSELECT_CAN_CONTROLLER_SLAVE;
	}
}
