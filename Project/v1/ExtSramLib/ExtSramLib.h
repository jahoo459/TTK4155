/*
 * ExtSramLib.h
 *
 * Created: 08.09.2017 09:58:29
 *  Author: janwh
 */ 


#ifndef EXTSRAMLIB_H_
#define EXTSRAMLIB_H_

#include "..\ProjectMain\definitions.h"

void enableXMEM(char releaseJtagPins);
uint8_t readFromAddress(volatile char* ptr);
void saveToAddress(volatile char* ptr, uint8_t data);
void disableXMEM();

#endif /* EXTSRAMLIB_H_ */
