/*
 * oledLib.c
 *
 * Created: 9/22/2017 1:22:41 PM
 *  Author: Jasiek
 */ 

#include <avr/io.h>
#include <string.h>
#include "..\ProjectMain\definitions.h"
#include "oledLib.h"
#include "..\fonts\fonts.h"
#include <util/delay.h>
#include <math.h>

volatile uint8_t *oled_cmd = (volatile uint8_t*)0x1000;
volatile uint8_t *oled_data = (volatile uint8_t*)0x1200;

// buffer variables
volatile char *oled_buffer = (char *) 0x1c00;
uint16_t oled_buffer_size = 0x400;
volatile char *oled_buffer_position = (char *) 0x1c00;
//printf("oled_buffer_position = %#x\n", *oled_buffer_position);

// OLED Parameters
static int height = 8;		//screen height (8 pages) 
static int width = 128;		//screen width (128 columns)

// arrow Parameters
#define ARROW_WIDTH 5 // Definition of the Arrow Size - allows easy refreshing of the arrow section
static int arrow_position = 0; // arrow Position in menu
static int arrow_step = 0; // relative arrow Position during Animation

// animation Parameters
static int animation_tick = 0; // indicates an advance of the animation

//------------------------------------------------------------------------------
// Default initialization routine of the OLED display.

void OLED_init(void)
{
	OLED_writeByteToOLED(oled_cmd, 0xae); // Display off
	OLED_writeByteToOLED(oled_cmd, 0xa1); // Segment remap
	OLED_writeByteToOLED(oled_cmd, 0xda); // Common pads hardware: alternative
	OLED_writeByteToOLED(oled_cmd, 0x12);
	OLED_writeByteToOLED(oled_cmd, 0xc8); // Common output scan direction:com63~com0
	OLED_writeByteToOLED(oled_cmd, 0xa8); // Multiplex ration mode:63
	OLED_writeByteToOLED(oled_cmd, 0x3f);
	OLED_writeByteToOLED(oled_cmd, 0xd5); // Display divide ratio/osc. freq. mode
	OLED_writeByteToOLED(oled_cmd, 0x80); // default value is 0x80
	OLED_writeByteToOLED(oled_cmd, 0x81); // Contrast control
	OLED_writeByteToOLED(oled_cmd, 0x50); // set contrast
	OLED_writeByteToOLED(oled_cmd, 0xd9); // Set pre-charge period
	OLED_writeByteToOLED(oled_cmd, 0x21); // command to set column address !! two cmd bytes missing !!
	OLED_writeByteToOLED(oled_cmd, 0x20); // Set Memory Addressing Mode
	OLED_writeByteToOLED(oled_cmd, 0x00); // Page addressing mode (02)
	OLED_writeByteToOLED(oled_cmd, 0xdb); // VCOM deselect level mode
	OLED_writeByteToOLED(oled_cmd, 0x30);
	OLED_writeByteToOLED(oled_cmd, 0xad); // Master configuration
	OLED_writeByteToOLED(oled_cmd, 0x00);
	OLED_writeByteToOLED(oled_cmd, 0xa4); // Out follows RAM content
	OLED_writeByteToOLED(oled_cmd, 0xa6); // Set normal display
	OLED_writeByteToOLED(oled_cmd, 0xaf); // Display on
	
	OLED_clear(); // wipe the screen
	OLED_splashScreen();
	//OLED_flyingArrows();
	
	// setup 8-bit counter0 without PWM
	// setting Mode of Operation to CTC and Prescaler to div/1024
	TCCR0 |= (1<<WGM01) | (1<<CS02) | (1<<CS00);
	// Output Compare Interrupt Enable
	TIMSK |= (1<<OCIE0);
	// 17ms Counter cycle (4800Hz -> 80 cycles)
	OCR0 = 80;
	
}


//------------------------------------------------------------------------------
// This function clears the whole screen by setting the whole OLED RAM to zero.

void OLED_clear(void)
{
	// clear OLED buffer
	for(uint16_t i = 0; i < oled_buffer_size; i++)
	{
		oled_buffer[i] = 0x00;
	}
	
	// todo: remove later
// 	for(uint8_t count_row = 0; count_row < height; count_row++)
// 	{
// 		// move to first column in row
// 		OLED_goto(count_row, 0);
// 
// 		for(uint8_t count_column = 0; count_column < width; count_column++)
// 		{
// 			OLED_writeByteToOLED(oled_data, 0x00);
// 		}
// 	}
}


//------------------------------------------------------------------------------
void OLED_updateScreen(void)
{
	// set column address
// 	OLED_writeByteToOLED(oled_cmd_ext_ram, 0x21); // command to set column address
// 	OLED_writeByteToOLED(oled_cmd_ext_ram, 0x00); // column start address ~ 0d
// 	OLED_writeByteToOLED(oled_cmd_ext_ram, 0x7f); // column end address ~ 127d
	// set horizontal addressing mode
// 	OLED_writeByteToOLED(oled_cmd, 0x20); // set memory addressing mode
// 	OLED_writeByteToOLED(oled_cmd, 0x00); // set horizontal addressing mode ~ 0d
// 	
	// write OLED buffer to OLED Memory
	OLED_goto(0,0); // todo: start writing in top left corner (maybe not further needed)
	for(uint16_t i = 0; i < oled_buffer_size; i++)
	{
		OLED_writeByteToOLED(oled_data, oled_buffer[i]);
	}
	
	// todo: remove later
	// go back to page adressing mode
// 	OLED_writeByteToOLED(oled_cmd, 0x20); // Set Memory Addressing Mode
// 	OLED_writeByteToOLED(oled_cmd, 0x02); // Page addressing mode
 }


//------------------------------------------------------------------------------
// This Function writes the Byte _data to a specified (*_address) position in 
// the OLED RAM.
void OLED_writeByteToOLED(volatile uint8_t *address, uint8_t data)
{
	*address = data;
}


//------------------------------------------------------------------------------
//
void OLED_writePixelToOLED(uint8_t u, uint8_t v)
{
	// determine relevant row
	uint8_t row = v/8;
	// buffer old column
	OLED_bufferGoto(row, u);
	uint8_t buffer_column = readFromAddress(oled_buffer_position);
	// create new column
	buffer_column |= (1<<(v%8));
	OLED_writeByteToOLED(oled_buffer_position, buffer_column);
}


//------------------------------------------------------------------------------
// This Function allows to set the starting position on the OLED screen.
//
// _row specifies the page start address
//		+ command: 0xB0 ~ 0xB7
//		+ range: 0 - 7
//
// _column specifies the start column address. The column address has to be
// parted into its two nibbles.
//		+ command lower nibble: 0x00 ~ 0x0F
//		+ command higher nibble: 0x10 ~ 0x1F
//		+ range: 0 - 127
//
// In order to obtain the lower nibble, the column address is masked:
//		0000 1111	mask 0x0f
//	&	HNIB LNIB	column address
//	-------------
//	=	0000 LNIB	lower column start address
//
// In order to obtain the higher nibble, the column address is right shifted:
//		HNIB LNIB >> 4	=	HNIB

void OLED_goto(int row, int column)
{
	uint8_t lnib = 0;
	uint8_t hnib = 0;

	// row address
	row = 0xb0 + row; // construction of the page start address

	// column address
	lnib = 0x0f & column; // mask lower nibble from column
	lnib = 0x00 + lnib; // construction of lower column start address

	hnib = column >> 4; // right shift column to get higher nibble
	hnib = 0x10 + hnib; // construction of higher column start address

	// get to desired page by taking out the orders
	OLED_writeByteToOLED(oled_cmd, row);
	OLED_writeByteToOLED(oled_cmd, lnib);
	OLED_writeByteToOLED(oled_cmd, hnib);
}

void OLED_bufferGoto(int row, int column)
{
	// change pointer in OLED SRAM buffer
	oled_buffer_position = oled_buffer + (row * 0x80) + column;
	//printf("oled_buffer_position = %#x\n", oled_buffer_position);
}


//------------------------------------------------------------------------------
// This function accesses the font that was saved to the MCU's flash memory and
// prints out one character.
// To access the desired character in the font table, it has to be converted to
// the characters position in the table.
// The available characters match the order of the ASCII table starting with
// SPACE (' '). As SPACE corresponds to position 32 in the ASCII table, this 
// number has to be subtracted in order to access the right position in the font
// table.

void OLED_printCharacter(uint8_t character)
{
	character = character-32; // map character to font table
	
	// print 8 columns into OLED Buffer in SRAM
	for(uint8_t i = 0; i < 8; i++)
	{
		saveToAddress(oled_buffer_position, pgm_read_byte(&font8[character][i]));
		oled_buffer_position++;
	}
	
	// print 8 columns into OLED RAM
// 	for(uint8_t i = 0; i < 8; i++)
// 	{
// 		OLED_writeByteToOLED(oled_data, pgm_read_byte(&font8[character][i]));
// 	}
}


//------------------------------------------------------------------------------
// This function allows to print Strings to the OLED display

void OLED_printString(char* msg)
{
	for(uint8_t i = 0; i < strlen(msg); i++)
	{
		OLED_printCharacter(msg[i]);
	}
}


//------------------------------------------------------------------------------
// This function prints an arrow to the OLED display

void OLED_printArrow(void)
{
	// write arrow to oled sram buffer
	saveToAddress(oled_buffer_position, 0b00011000);
	oled_buffer_position++;
	saveToAddress(oled_buffer_position, 0b00011000);
	oled_buffer_position++;
	saveToAddress(oled_buffer_position, 0b01111110);
	oled_buffer_position++;
	saveToAddress(oled_buffer_position, 0b00111100);
	oled_buffer_position++;
	saveToAddress(oled_buffer_position, 0b00011000);
	oled_buffer_position++;
	
// 	OLED_writeByteToOLED(oled_data, 0b00011000);
// 	OLED_writeByteToOLED(oled_data, 0b00011000);
// 	OLED_writeByteToOLED(oled_data, 0b01111110);
// 	OLED_writeByteToOLED(oled_data, 0b00111100);
// 	OLED_writeByteToOLED(oled_data, 0b00011000);
}


//------------------------------------------------------------------------------
// This function clears the first columns in the size of the arrow from the 
// OLED display

void OLED_clearArrow(void)
{
	for(uint8_t count_row = 0; count_row < height; count_row++)
	{
		OLED_bufferGoto(count_row,0); // move to first column in row

		// the inner loop iterates the columns of each row
		for(uint8_t count_column = 0; count_column < ARROW_WIDTH; count_column++)
		{
			saveToAddress(oled_buffer_position, 0x00);
			oled_buffer_position++;
		}
	}
	// the outer loop iterates the display rows
// 	for(uint8_t count_row = 0; count_row < height; count_row++)
// 	{
// 		OLED_goto(count_row,0); // move to first column in row
// 
// 		// the inner loop iterates the columns of each row
// 		for(uint8_t count_column = 0; count_column < arrow_width; count_column++)
// 		{
// 			OLED_writeByteToOLED(oled_data, 0x00);
// 		}
// 	}
}


//------------------------------------------------------------------------------
// This function can be used to remove the arrow from its old row and move
// it to a new one.
void OLED_moveArrow(int joy_counter)
{
	// reset animation tick
	animation_tick = 0;
	
	if(joy_counter == arrow_position)
	{
		OLED_clearArrow(); // clear the arrow space
		//OLED_goto(joy_counter,0); // move cursor to specified row
		OLED_bufferGoto(joy_counter, 0);
		OLED_printArrow(); // print arrow
	}
	else if(joy_counter > arrow_position)
	{		
		//printf("%#x %#x %#x %#x %#x \n", readFromAddress(0x1c00), readFromAddress(0x1c01), readFromAddress(0x1c02), readFromAddress(0x1c03), readFromAddress(0x1c04));
		uint8_t buffer_dummy[ARROW_WIDTH] = {0x00};
		uint8_t buffer_current[ARROW_WIDTH] = {0x00};
			
		for(uint8_t step = 0; step < 8; step++)
		{
			for(uint8_t i = 0; i < height; i++)
			{
				for(uint8_t j = 0; j < ARROW_WIDTH; j++)
				{
					OLED_bufferGoto(i, j);
					buffer_current[j] = readFromAddress(oled_buffer_position); // save state of current row
				}
			
				for(uint8_t k = 0; k < ARROW_WIDTH; k++)
				{				
					OLED_bufferGoto(i, k);
					saveToAddress(oled_buffer_position, buffer_current[k]<<1 | buffer_dummy[k]>>7); // move current row 1 down + combine with lowest from above
					buffer_dummy[k] = buffer_current[k]; // current row is reference for next row
				}
			}
			
			while(animation_tick != 1){_delay_us(1);};
			animation_tick = 0;
		}
		arrow_position++;
	}
	else
	{
		uint8_t buffer_dummy[ARROW_WIDTH] = {0x00};
		uint8_t buffer_current[ARROW_WIDTH] = {0x00};
		
		for(uint8_t step = 0; step < 8; step++)
		{
			for(uint8_t i = 0; i < height; i++)
			{
				for(uint8_t j = 0; j < ARROW_WIDTH; j++)
				{
					OLED_bufferGoto(height-1-i, j);
					buffer_current[j] = readFromAddress(oled_buffer_position); // save state of current row
				}
				
				for(uint8_t k = 0; k < ARROW_WIDTH; k++)
				{
					OLED_bufferGoto(height-1-i, k);
					saveToAddress(oled_buffer_position, buffer_current[k]>>1 | buffer_dummy[k]<<7); // move current row 1 up + combine with highest from below
					buffer_dummy[k] = buffer_current[k]; // current row is reference for next row
				}
			}
			
			while(animation_tick != 1){_delay_us(1);};
			animation_tick = 0;
		}
		arrow_position--;
	}
	
}


//------------------------------------------------------------------------------
//
void OLED_resetArrow()
{
	OLED_clearArrow();
	OLED_bufferGoto(0,0);
	OLED_printArrow();
	arrow_position = 0;
}


//------------------------------------------------------------------------------
//
void OLED_setAnimationTick()
{
		animation_tick = 1;
}


//------------------------------------------------------------------------------
//
void OLED_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
	if(x0 == x1)
	{
		for(uint8_t v = y0; v <= y1; v++)
		{
			OLED_writePixelToOLED(x0, v);
		}
	}
	else
	{	
		if(y0 < y1)
		{
 			float m = 0.5;
			float c;
 			
			if(y0 == 14)
			{
				c = -7.5;
			}
			else if(y0 == 39)
			{
				c = 17.5;
			}
			else
			{
				c = -27.5;
			}
						
			for(uint8_t u = x0; u <= x1; u++)
			{
				for(uint8_t v = y0; v <= y1; v++)
				{
					if((v-u*m-c) == 0)
					{
						OLED_writePixelToOLED(u, v);
					}
				}
			}
		}
		else
		{
			float m = -0.5;
			float c;
			
			if(y0 == 14)
			{
				c = 35.5;
			}
			else if(y0 == 24)
			{
				c = 55.5;
			}
			else
			{
				c = 80.5;
			}
						
			for(uint8_t u = x0; u <= x1; u++)
			{
				for(uint8_t v = y1; v <= y0; v++)
				{
					if((v-u*m-c) == 0)
					{
						OLED_writePixelToOLED(u, v);
					}
				}
			}
		}
	}
}


//------------------------------------------------------------------------------
//
void OLED_circle(uint8_t x0, uint8_t y0, uint8_t r)
{
	OLED_writePixelToOLED(x0, y0);
	
	float step = 2*M_PI_4/7;
	uint8_t u, v;
	
	for(float theta = 0; theta < 2*M_PI/4; theta += step)
	{
		u = x0 + (int)(r*cos(theta));
		v = y0 + (int)(r*sin(theta));
		OLED_writePixelToOLED(u, v);
		OLED_writePixelToOLED(u, 2*y0-v);
 		OLED_writePixelToOLED(2*x0-u, v);
 		OLED_writePixelToOLED(2*x0-u, 2*y0-v);
// 		OLED_writePixelToOLED(u, v);
// 		OLED_writePixelToOLED(u, v);
// 		OLED_writePixelToOLED(u, v);
// 		OLED_writePixelToOLED(u, v);
	}
}


//------------------------------------------------------------------------------
//
void OLED_logo()
{
	// define cube parameters
// 	uint8_t ver_padding = 10;
// 	uint8_t hor_padding = 32;
// 	uint8_t ver_axis = width/2;
// 	uint8_t Ay = ver_padding+ver_axis-hor_padding;
	
	// draw cube top
	// vertical lines
	OLED_line(43, 14, 43, 39);
	OLED_line(83, 14, 83, 39);
	OLED_line(63, 24, 63, 49);
	// diagonal lines
	// left half
 	OLED_line(43, 14, 63, 4);
 	OLED_line(43, 14, 63, 24);
	OLED_line(43, 39, 63, 49);
	// right half
	OLED_line(63, 4, 83, 14);
	OLED_line(63, 4, 83, 14);
	OLED_line(63, 24, 83, 14);
	OLED_line(63, 49, 83, 39);
	
	// letters
	OLED_bufferGoto(1, 59);
	OLED_printCharacter('G');
	OLED_bufferGoto(4, 49);
	OLED_printCharacter('4');
	OLED_bufferGoto(4, 69);
	OLED_printCharacter('6');
}


//------------------------------------------------------------------------------
//
void OLED_table()
{
	// horizontal lines
	OLED_line(30, 34, 96, 34); // top
	OLED_line(30, 40, 96, 40); // bottom
	// vertical lines
 	OLED_line(63, 28, 63, 40); // net
 	OLED_line(35, 40, 35, 48); // left leg
 	OLED_line(91, 40, 91, 48); // right leg
	OLED_line(30, 34, 30, 40); // left edge
	OLED_line(96, 34, 96, 40); // right edge
}

//------------------------------------------------------------------------------
//
void OLED_splashScreen()
{
	printf("run splash screen...\n");
	OLED_logo();
 	OLED_bufferGoto(7,12);
 	OLED_printString("Group 46 Soft");
	//OLED_table();
	//OLED_bufferGoto(7,28);
	//OLED_printString("Ping-Pong");
}

// void OLED_setContrast(uint8_t contrast)
// {
// 	// activate contrast control
// 	OLED_writeByteToOLED(oled_cmd, 0x81);
// 	// set contrast to provided value
// 	OLED_writeByteToOLED(oled_cmd, contrast);
// }
// 
// void OLED_fadeIn(void)
// {
// 	// gradually increase contrast
// 	for(uint8_t i = 0; i < 255; i++)
// 	{
// 		OLED_setContrast(i);
// 		_delay_ms(4);
// 	}
// }
// 
// void OLED_fadeOut(void)
// {
// 	// gradually decrease contrast
// 	for(uint8_t i = 0; i < 255; i++)
// 	{
// 		OLED_setContrast(255-i);
// 		_delay_ms(4);
// 	}
// }
// 
// void OLED_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t thickness)
// {
// 	uint8_t lineSegment = 0;
// 	if(y0 == y1)
// 	{
// 		lineSegment = (1<<y0/8);
// 		OLED_goto(y0/8,x0);
// 		for(uint8_t i = x0; i <= x1; i++)
// 		{
// 			OLED_writeByteToOLED(oled_data, lineSegment);
// 		}
// 	}
// 	
// 	// page addressing is wrong - should use all possible lines
// 	// memory buffer should be used!
// 	if(x0 == x1)
// 	{
// 		lineSegment = 0xff;
// 		
// 		for(uint8_t i = 0; i<8; i++)
// 		{
// 			OLED_goto(i,x0);
// 			OLED_writeByteToOLED(oled_data, lineSegment);
// 		}
// 	}
// }
// 
// void OLED_splashScreen(void)
// {
// 	printf("run splash screen...\n");
// 	
// 	OLED_setContrast(0);
// 	
// 	_delay_ms(500);
// 	OLED_goto(2,4);
// 	OLED_printString("Group 46 Soft");
// 	OLED_fadeIn();
// 	_delay_ms(500);
// 	
//  	OLED_clear();
//  	OLED_setContrast(0);
//  	
//  	_delay_ms(200);
//  	OLED_goto(3,28);
//  	OLED_printString("proudly");
//  	OLED_goto(4,24);	
//  	OLED_printString("presents");
//  	OLED_fadeIn();
//  	_delay_ms(500);
//  	
//  	OLED_clear();
//  	OLED_setContrast(0);
//  	
//  	_delay_ms(200);
//  	OLED_goto(3, 28);
//  	OLED_printString("FIFA 18");
//  	OLED_fadeIn();
//  	_delay_ms(1000);
// 	
// 	// clean up
// 	OLED_clear();
// 	OLED_setContrast(0x50); // back to standard contrast
// 	
// 	printf("splash screen done...\n");
// }
// 
// void OLED_flyingArrows()
// {
// 	printf("let the arrows fly...\n");
// 	
// 	// setup fosc
// 	OLED_writeByteToOLED(oled_cmd, 0xd5); // Display divide ratio/osc. freq. mode
// 	OLED_writeByteToOLED(oled_cmd, 0xf0); // default value is 0x80
// 	
// 	// setup scrolling
// 	OLED_writeByteToOLED(oled_cmd, 0x26); // set right horizontal scroll
// 	OLED_writeByteToOLED(oled_cmd, 0x00); // dummy byte 0x00
// 	OLED_writeByteToOLED(oled_cmd, 0x00); // start page address
// 	OLED_writeByteToOLED(oled_cmd, 0x07); // time interval between scroll steps (2 frames)
// 	OLED_writeByteToOLED(oled_cmd, 0x07); // end page address
// 	OLED_writeByteToOLED(oled_cmd, 0x00); // dummy byte 0x00
// 	OLED_writeByteToOLED(oled_cmd, 0xff); // dummy byte 0xff
// 	
// 	for(uint8_t i = 0; i < 9; i++)
// 	{
// 		for(uint8_t count_row = 0; count_row < height; count_row++)
// 		{
// 			OLED_goto(count_row,117); // move to first column in row
// 			
// 			// the inner loop iterates the columns of each row
// 			for(uint8_t count_column = 117; count_column < width; count_column++)
// 			{
// 				OLED_writeByteToOLED(oled_data, 0x00);
// 			}
// 		}
// 		
// 		OLED_goto(0,0);
// 		OLED_printArrow();
// 		OLED_goto(2,0);
// 		OLED_printArrow();
// 		OLED_goto(4,0);
// 		OLED_printArrow();
// 		OLED_goto(6,0);
// 		OLED_printArrow();
// 	
//  		OLED_writeByteToOLED(oled_cmd, 0x2f); // activate scrolling 
//  		_delay_ms(75);
//  		OLED_writeByteToOLED(oled_cmd, 0x2e); // deactivate scrolling 
//  	
//  		OLED_goto(1,0);
//  		OLED_printArrow();
//  		OLED_goto(3,0);
//  		OLED_printArrow();
//  		OLED_goto(5,0);
//  		OLED_printArrow();
//  		OLED_goto(7,0);
//  		OLED_printArrow();
//  	
//  		OLED_writeByteToOLED(oled_cmd, 0x2f); // activate scrolling
//  		_delay_ms(75);
//  		OLED_writeByteToOLED(oled_cmd, 0x2e); // deactivate scrolling
// 	}
// 	
//   	for(uint8_t i = 0; i < 18; i++)
//   	{
//   		for(uint8_t count_row = 0; count_row < height; count_row++)
//   		{
//   			OLED_goto(count_row,122); // move to first column in row
//   
//   			// the inner loop iterates the columns of each row
//   			for(uint8_t count_column = 122; count_column < width; count_column++)
//   			{
//   				OLED_writeByteToOLED(oled_data, 0x00);
//   			}
//   		}
//   		OLED_writeByteToOLED(oled_cmd, 0x2f); // activate scrolling
//   		_delay_ms(75);
//   		OLED_writeByteToOLED(oled_cmd, 0x2e); // deactivate scrolling
//   	}
// 
// 	printf("arrows are done...\n");
// 	OLED_clear();
// 	
// 	// reset fosc
// 	OLED_writeByteToOLED(oled_cmd, 0xd5); // Display divide ratio/osc. freq. mode
// 	OLED_writeByteToOLED(oled_cmd, 0x80); // default value is 0x80
// }