/*
 * MCP2515Lib.h
 *
 * Created: 06.10.2017 13:23:50
 *  Author: lukasah
 */ 


#ifndef MCP2515LIB_H_
#define MCP2515LIB_H_


uint8_t MCP2515_init();
void MCP2515_reset();
uint8_t MCP2515_read(uint8_t address, SPI_SLAVES slave);
void MCP2515_write(uint8_t address, uint8_t data, SPI_SLAVES slave);
void MCP2525_requestToSend(SPI_SLAVES slave, uint8_t sB2, uint8_t sB1, uint8_t sB0);
uint8_t MCP2515_readStatus(SPI_SLAVES slave);
void MCP2515_bitModify(SPI_SLAVES slave, uint8_t register2change, uint8_t bit2change, uint8_t newValue);

#endif /* MCP2515LIB_H_ */