/*
 * oledLib.c
 *
 * Created: 9/22/2017 1:22:41 PM
 *  Author: Jasiek
 */ 

#include <avr/io.h>
#include <string.h>

#include "oledLib.h"
#include "..\fonts\fonts.h"

volatile uint8_t *oled_cmd = (volatile uint8_t*)0x1000;
volatile uint8_t *oled_data = (volatile uint8_t*)0x1200;

int height = 8; //screen height in 
int width = 128; //screen width in 

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
	OLED_writeByteToOLED(oled_cmd, 0x02); // Page addressing mode
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
	int count_row = 0;
	int count_column = 0;

	for(count_row; count_row < height; count_row++)
	{
		// move to first column in row
		OLED_goto(count_row, 0);

		for(count_column; count_column < width; count_column++)
		{
			OLED_writeByteToOLED(oled_data, 0xff);
		}
	}
}

void OLED_writeByteToOLED(volatile uint8_t *address, uint8_t data)
{
	*address = data;
}

void OLED_goto(int row, int column)
{
	//row should be 0-7
	//column should be 16 - 127
	uint8_t lnib;
	uint8_t hnib;

	// row:
	row = 0xb0 + row;

	// column:
	// lower nibble
	lnib = 0x0f & column;
	lnib = 0x00 + lnib;

	// higher nibble
	hnib = column >> 4;
	hnib = 0x10 + hnib;

	printf("row: %d,\thnib: %#x,\tlnib: %#x", row, hnib, lnib);

	// get to page
	OLED_writeByteToOLED(oled_cmd, row);
	OLED_writeByteToOLED(oled_cmd, lnib);
	OLED_writeByteToOLED(oled_cmd, hnib);
}

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

void OLED_print_character(uint8_t character)
{
	// subtract 32 from character ASCII-code to match SPACE to the first entry of the font array
	character = character-32;
	
	// print 8 columns into OLED RAM
	for(uint8_t i = 0; i < 8; i++)
	{
		OLED_writeByteToOLED(oled_data, pgm_read_byte(&font8[character][i]));
	}
}

void OLED_print_string(char* msg)
{
	for(uint8_t i = 0; i < strlen(msg); i++)
	{
		OLED_print_character(msg[i]);
	}
}
