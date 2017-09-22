/*
 * oledLib.c
 *
 * Created: 9/22/2017 1:22:41 PM
 *  Author: Jasiek
 */ 

#include <avr/io.h>
#include "oledLib.h"

volatile uint8_t *oled_cmd = (volatile uint8_t*)0x1000;
volatile uint8_t *oled_data = (volatile uint8_t*)0x1200;

int height = 8; //screen height in 
int width = 128; //screen width in 

void OLED_print_arrow()
{
	//OLED_pos(row, col);
	OLED_writeByteToOLED(oled_data, 0b00011000);
	OLED_writeByteToOLED(oled_data, 0b00011000);
	OLED_writeByteToOLED(oled_data, 0b01111110);
	OLED_writeByteToOLED(oled_data, 0b00111100);
	OLED_writeByteToOLED(oled_data, 0b00011000);
	//OLED_writeByteToOLED(oled_cmd, 0x2f);
}

void OLED_writeByteToOLED(volatile uint8_t *address, uint8_t data)
{
	*address = data;
}

void OLED_init()
{
	OLED_writeByteToOLED(oled_cmd, 0xae); // Display off
	OLED_writeByteToOLED(oled_cmd, 0xa1); // Segment remap
	OLED_writeByteToOLED(oled_cmd, 0xda); // Common pads hardware: alternative
	OLED_writeByteToOLED(oled_cmd, 0x12);
	OLED_writeByteToOLED(oled_cmd, 0xc8); // Common output scan direction:com63~com0
	OLED_writeByteToOLED(oled_cmd, 0xa8); // Multiplex ration mode:63
	OLED_writeByteToOLED(oled_cmd, 0x3f);
	OLED_writeByteToOLED(oled_cmd, 0xd5); // Display divide ratio/osc. freq. mode
	OLED_writeByteToOLED(oled_cmd, 0x80);
	OLED_writeByteToOLED(oled_cmd, 0x81); // Contrast control
	OLED_writeByteToOLED(oled_cmd, 0x50);
	OLED_writeByteToOLED(oled_cmd, 0xd9); // Set pre-charge period
	OLED_writeByteToOLED(oled_cmd, 0x21);
	OLED_writeByteToOLED(oled_cmd, 0x20); // Set Memory Addressing Mode
	OLED_writeByteToOLED(oled_cmd, 0x00); // Horizontal addressing
	OLED_writeByteToOLED(oled_cmd, 0xdb); // VCOM deselect level mode
	OLED_writeByteToOLED(oled_cmd, 0x30);
	OLED_writeByteToOLED(oled_cmd, 0xad); // Master configuration
	OLED_writeByteToOLED(oled_cmd, 0x00);
	OLED_writeByteToOLED(oled_cmd, 0xa4); // Out follows RAM content
	OLED_writeByteToOLED(oled_cmd, 0xa6); // Set normal display
	OLED_writeByteToOLED(oled_cmd, 0xaf); // Display on
}

void OLED_clear()
{
	int bit = 0;
	int row = 0;
		
	for(bit; bit < width*height; bit++)
	{
		OLED_writeByteToOLED(oled_data, 0b00000000);
	}

}

void OLED_goto(int row, int column)
{
	//row should be 0-7
	//column should be 16 - 127
}




