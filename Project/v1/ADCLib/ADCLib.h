/*
 * ADCLib.h
 *
 * Created: 15.09.2017 11:21:01
 *  Author: janwh
 */ 


#ifndef ADCLIB_H_
#define ADCLIB_H_

#include "..\ExtSramLib\ExtSramLib.h"

#define set_bit( reg, bit ) (reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))

void ADC_init();
char ADC_request(unsigned char channel, volatile char *ADC_ADDRESS);
unsigned char ADC_read(volatile char *ADC_ADDRESS);


#endif /* ADCLIB_H_ */