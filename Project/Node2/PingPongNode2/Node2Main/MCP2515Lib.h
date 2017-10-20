/*
 * MCP2515Lib.h
 *
 * Created: 06.10.2017 13:23:50
 *  Author: lukasah
 */ 


#ifndef MCP2515LIB_H_
#define MCP2515LIB_H_

#include "..\ProjectMain\definitions.h"
#include "MCP2515.h"
#include "..\SPILib\SPILib.h"

uint8_t MCP2515_init();
void MCP2515_reset(SPI_SLAVES slave);
uint8_t MCP2515_read(SPI_SLAVES slave, uint8_t address);
void MCP2515_write(SPI_SLAVES slave, uint8_t address, uint8_t data);
void MCP2525_requestToSend(SPI_SLAVES slave, uint8_t sB2, uint8_t sB1, uint8_t sB0);
uint8_t MCP2515_readStatus(SPI_SLAVES slave);
void MCP2515_bitModify(SPI_SLAVES slave, uint8_t register2change, uint8_t bit2change, uint8_t newValue);

#endif /* MCP2515LIB_H_ */