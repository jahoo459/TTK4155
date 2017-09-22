/*
 * oledLib.c
 *
 * Created: 9/22/2017 1:22:41 PM
 *  Author: Jasiek
 */ 

#include <avr/io.h>

int width = 128;
int height = 8;

volatile uint8_t *oled_cmd = (volatile uint8_t*)0x1000;
volatile uint8_t *oled_data = (volatile uint8_t*)0x1200;

//void OLED_print_arrow(int row, int col)
//{
	//OLED_pos(row, col);
	//OLED_write_data(0b00011000);
	//OLED_write_data(0b00011000);
	//OLED_write_data(0b01111110);
	//OLED_write_data(0b00111100);
	//OLED_write_data(0b00011000);
//}

void OLED_init()
{
	WriteByteToOLED(oled_cmd, 0xae); // Display off
	WriteByteToOLED(oled_cmd, 0xa1); // Segment remap
	WriteByteToOLED(oled_cmd, 0xda); // Common pads hardware: alternative
	WriteByteToOLED(oled_cmd, 0x12);
	WriteByteToOLED(oled_cmd, 0xc8); // Common output scan direction:com63~com0
	WriteByteToOLED(oled_cmd, 0xa8); // Multiplex ration mode:63
	WriteByteToOLED(oled_cmd, 0x3f);
	WriteByteToOLED(oled_cmd, 0xd5); // Display divide ratio/osc. freq. mode
	WriteByteToOLED(oled_cmd, 0x80);
	WriteByteToOLED(oled_cmd, 0x81); // Contrast control
	WriteByteToOLED(oled_cmd, 0x50);
	WriteByteToOLED(oled_cmd, 0xd9); // Set pre-charge period
	WriteByteToOLED(oled_cmd, 0x21);
	WriteByteToOLED(oled_cmd, 0x20); // Set Memory Addressing Mode
	WriteByteToOLED(oled_cmd, 0x00); // Horizontal addressing
	WriteByteToOLED(oled_cmd, 0xdb); // VCOM deselect level mode
	WriteByteToOLED(oled_cmd, 0x30);
	WriteByteToOLED(oled_cmd, 0xad); // Master configuration
	WriteByteToOLED(oled_cmd, 0x00);
	WriteByteToOLED(oled_cmd, 0xa4); // Out follows RAM content
	WriteByteToOLED(oled_cmd, 0xa6); // Set normal display
	WriteByteToOLED(oled_cmd, 0xaf); // Display on
}

void writeByteToOLED(volatile uint8_t *address, uint8_t data)
{
	*address = data;
}
