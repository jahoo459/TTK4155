/*
 * ADCLib.h
 *
 * Created: 15.09.2017 11:21:01
 *  Author: janwh
 */ 


#ifndef ADCLIB_H_
#define ADCLIB_H_

#include "..\ExtSramLib\ExtSramLib.h"

void ADC_init();
void ADC_request(uint8_t channel, volatile char *ADC_ADDRESS);
uint8_t ADC_read(volatile char *ADC_ADDRESS);


#endif /* ADCLIB_H_ */