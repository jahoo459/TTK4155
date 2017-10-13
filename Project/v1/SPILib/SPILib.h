/*
 * SPILib.h
 *
 * Created: 06.10.2017 11:23:41
 *  Author: lukasah
 */ 


#ifndef SPILIB_H_
#define SPILIB_H_

#define DDR_SPI DDRB
#define PORT_SPI PORTB				//for slave selection
#define SS_CAN_CONTROLLER_PIN PB4	//for slave selection
#define MOSI_PIN DDB5
#define MISO_PIN DDB6
#define SCK_PIN DDB7


typedef enum {SS_CAN_CONTROLLER} SPI_SLAVES;

void SPI_init(void);
void SPI_send(uint8_t cData);
uint8_t SPI_receive(void);
void SPI_activateSlave(SPI_SLAVES slave);
void SPI_deactivateSlave(SPI_SLAVES slave);


#endif /* SPILIB_H_ */