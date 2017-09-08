/*
 * ExtSramLib.h
 *
 * Created: 08.09.2017 09:58:29
 *  Author: janwh
 */ 


#ifndef EXTSRAMLIB_H_
#define EXTSRAMLIB_H_

void enableXMEM(char releaseJtagPins);
unsigned char readFromAddress(char* address);
void saveToAddress(char* address, unsigned char data);
void disableXMEM();



#endif /* EXTSRAMLIB_H_ */
