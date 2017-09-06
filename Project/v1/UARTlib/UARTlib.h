/*
 * UARTlib.h
 *
 * Created: 9/6/2017 12:32:30 PM
 *  Author: Jasiek
 */ 


#ifndef UARTLIB_H_
#define UARTLIB_H_

#define set_bit( reg, bit ) (reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))


int uartSend(const char byte2Send, FILE* file);
unsigned char uartReceive(FILE* file);
void uartInit(uint32_t baud, uint32_t fosc, uint32_t ubrr);


#endif /* UARTLIB_H_ */