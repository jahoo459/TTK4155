/*
 * ExtSramLib.h
 *
 * Created: 08.09.2017 09:58:29
 *  Author: janwh
 */ 


#ifndef EXTSRAMLIB_H_
#define EXTSRAMLIB_H_

void enableXMEM(char releaseJtagPins);
uint8_t readFromAddress(unsigned int address);
void saveToAddress(unsigned int address, uint8_t data);
void disableXMEM();



#endif /* EXTSRAMLIB_H_ */
