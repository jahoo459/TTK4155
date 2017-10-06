/*
 * definitions.h
 *
 * Created: 15.09.2017 12:53:59
 *  Author: janwh
 */ 


#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#include <avr/io.h>
#include <stdio.h>

//XMEM addresses
#define EXT_RAM_SIZE 0x800

#define OLED_CMD_EXT_RAM 0x1000
#define OLED_DATA_EXT_RAM 0x1200
#define ADC_EXT_RAM 0x1400
#define SRAM_EXT 0x1800

extern volatile char *oled_cmd_ext_ram;
extern volatile char *oled_data_ext_ram;
extern volatile char *sram_ext;
extern volatile char *adc_ext_ram;

//BIT operations
#define set_bit( reg, bit ) (reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))

// basic stuff..
#define BAUDRATE 9600
#define FOSC  4915200
#define F_CPU FOSC
#define UBRR FOSC/16/BAUDRATE-1

#define uint16_t unsigned int

#include <util/delay.h>

// joystick position and direction
typedef struct  {
	int X_abs;
	int Y_abs;
	int X_per;
	int Y_per;
} JOY_position_t;

typedef enum {CENTRE, UP, DOWN, RIGHT, LEFT} JOY_direction_t;

// slider positions and buttons
typedef struct {
	uint8_t R_abs;
	uint8_t L_abs;
	uint8_t R_per;
	uint8_t L_per;
} SLI_position_t;

//CAN MESSAGE
typedef struct can_message{
	unsigned int id;
	uint8_t length;
	uint8_t data[8];
} can_message_t;

#endif /* DEFINITIONS_H_ */