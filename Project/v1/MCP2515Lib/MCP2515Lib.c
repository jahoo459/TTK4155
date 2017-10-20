/*
 * MCP2515Lib.c
 *
 * Created: 06.10.2017 13:23:10
 *  Author: lukasah
 */ 

#include <avr/io.h>
#include "MCP2515Lib.h"



uint8_t MCP2515_init()
{
	MCP2515_reset(SS_CAN_CONTROLLER); // Send reset-command
	// Self-test
	uint8_t value;
	value = MCP2515_read(SS_CAN_CONTROLLER, MCP_CANSTAT);
	printf("CANSTAT: %#x\n", value);
	if((value & MODE_MASK) != MODE_CONFIG) 
	{
		//printf("MCP2515 is NOT in configuration mode after reset! Value: %d\n", value);
		return 1;
	}
	//
	//printf("finished MCP2515_init\n");
	return 0;
}

 void MCP2515_reset(SPI_SLAVES slave)
{
	SPI_activateSlave(slave);
	SPI_send(MCP_RESET);
	_delay_ms(1);
	SPI_deactivateSlave(slave);
	_delay_ms(10);
}

uint8_t MCP2515_read(SPI_SLAVES slave, uint8_t address)
{
	SPI_activateSlave(slave);
	SPI_send(MCP_READ);
	_delay_ms(1);
	SPI_send(address);
	
	uint8_t result;
	result = SPI_receive();
	
	SPI_deactivateSlave(slave);
	return result;
}

void MCP2515_write(SPI_SLAVES slave, uint8_t address, uint8_t data)
{
	SPI_activateSlave(slave);
	SPI_send(MCP_WRITE);
	SPI_send(address);
	SPI_send(data);
	SPI_deactivateSlave(slave);
}

void MCP2525_requestToSend(SPI_SLAVES slave, uint8_t sB2, uint8_t sB1, uint8_t sB0)
{
	uint8_t instruction = 0x80;
	
	if(sB2) {instruction = instruction | (1<<2);}
	if(sB1) {instruction = instruction | (1<<1);}
	if(sB0) {instruction = instruction | (1<<0);}
	
	printf("RTS: Instruction: %d\n", instruction);
	
	SPI_activateSlave(slave);
	SPI_send(instruction);
	SPI_deactivateSlave(slave);
}

uint8_t MCP2515_readStatus(SPI_SLAVES slave)
{
	SPI_activateSlave(slave);
	uint8_t result;
	
	SPI_send(MCP_READ_STATUS);
	result = SPI_receive();
	
	SPI_deactivateSlave(slave);
	return result;
}

void MCP2515_bitModify(SPI_SLAVES slave, uint8_t register2change, uint8_t mask, uint8_t newValue)
{
	SPI_activateSlave(slave);
	
	SPI_send(MCP_BITMOD);
	SPI_send(register2change);
	SPI_send(mask);
	SPI_send(newValue);
	
	SPI_deactivateSlave(slave);
}
