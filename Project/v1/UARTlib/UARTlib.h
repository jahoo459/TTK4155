/*
 * UARTlib.h
 *
 * Created: 9/6/2017 12:32:30 PM
 *  Author: Jasiek
 */ 


#ifndef UARTLIB_H_
#define UARTLIB_H_

#include "..\ProjectMain\definitions.h"

void uartSend(const char byte2Send);
uint8_t uartReceive();
void uartInit(uint32_t baud, uint32_t fosc, uint32_t ubrr);

#endif /* UARTLIB_H_ */