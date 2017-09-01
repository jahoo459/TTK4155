#define F_CPU 1000000L

#include "compiler.h"
#include <avr/interrupt.h>
#include "conf_example.h"
#include <util/delay.h>


#define set_bit( reg, bit ) (reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))


#define LED1 PB0


void main(void)
{
	set_bit(DDRB, PB0);
	set_bit(PORTB, PB0);
	
	
	while(1)
	{
		if(test_bit(PORTB, PB0) == 1)
		{
			clear_bit(PORTB, PB0);
		}
		else
		{
			set_bit(PORTB, PB0);
		}
		
		_delay_ms(10);
	}
}