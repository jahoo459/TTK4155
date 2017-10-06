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
void SPI_send(uint8_t cData);
uint8_t SPI_receive(void);
void SPI_activateSlave(SPI_SLAVES slave);
void SPI_deactivateSlave(SPI_SLAVES slave);


#endif /* SPILIB_H_ */