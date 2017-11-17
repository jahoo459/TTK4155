/*
 * ADC2Lib.h
 *
 * Created: 27.10.2017 17:35:28
 *  Author: lukasah
 */ 


#ifndef ADC2LIB_H_
#define ADC2LIB_H_

#include <avr/io.h>

void ADC2_init(void);
uint8_t ADC2_updateValue(void);

#endif /* ADC2LIB_H_ */