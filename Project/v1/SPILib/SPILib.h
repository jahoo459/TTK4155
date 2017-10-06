/*
 * SPILib.h
 *
 * Created: 06.10.2017 11:23:41
 *  Author: lukasah
 */ 


#ifndef SPILIB_H_
#define SPILIB_H_

#define SS_CAN_CONTROLLER_PIN PB4

typedef enum {SS_CAN_CONTROLLER} SPI_SLAVES;

void SPI_init(void);
void SPI_send(uint8_t cData, SPI_SLAVES slave);
uint8_t SPI_receive(SPI_SLAVES slave);
void activateSlave(SPI_SLAVES slave);
void deactivateSlave(SPI_SLAVES slave);


#endif /* SPILIB_H_ */