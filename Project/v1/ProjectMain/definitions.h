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


//BIT operations
#define set_bit( reg, bit ) (reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))

// basic stuff..
#define BAUDRATE 9600
#define FOSC  4915200
#define  F_CPU FOSC
#define UBRR FOSC/16/BAUDRATE-1

#define uint16_t unsigned int


#endif /* DEFINITIONS_H_ */