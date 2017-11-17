/*
 * GameLib.c
 *
 * Created: 17.11.2017 09:42:59
 *  Author: lukasah
 */ 

#include <avr/io.h>

static char str[3];

void Game_runGame()
{
	uint16_t score = 0;
	uint8_t lives = 3;
	
	while(lives > 0)
	{
		// write Title to Screen
		OLED_goto(0,4);
		OLED_printString("** Ping-Pong **");
		
		// write to Screen
		OLED_goto(3,24);
		OLED_printString("Lives");
		OLED_goto(3,63);
		OLED_printString("Score");
		
		// write Numbers
		OLED_goto(4, 40);
		sprintf(str, "%d", lives);
		OLED_printString(str);
		OLED_goto(4, 79);
		sprintf(str, "%d", score);
		OLED_printString(str);	
	}
}

//void Game_updateScreen()
