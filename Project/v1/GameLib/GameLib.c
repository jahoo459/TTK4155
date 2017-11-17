/*
 * GameLib.c
 *
 * Created: 17.11.2017 09:42:59
 *  Author: lukasah
 */ 

#include <avr/io.h>
#include "..\JoystickLib\JoystickLib.h"
#include "..\SliderLib\SliderLib.h"
#include "..\CANLib\CANLib.h"

#define LIVES_POS 36
#define SCORE_POS 75

static char str[3];
static uint16_t score;
static uint8_t lives;

struct can_message message2send;
static uint8_t JoyPos;
static uint8_t SliPos;
static uint8_t ButtonRight;

void Game_init()
{
	// declare score and lives
	score = 0;
	lives = 3;
	
	// write Title to Screen
	OLED_clear();
	OLED_goto(0,4);
	OLED_printString("** Ping-Pong **");
		
	// write to Screen
	OLED_goto(3,LIVES_POS-16);
	OLED_printString("Lives");
	OLED_goto(3,SCORE_POS-16);
	OLED_printString("Score");
		
	// write Numbers
	OLED_goto(4, LIVES_POS);
	sprintf(str, "%d", lives);
	OLED_printString(str);
	OLED_goto(4, SCORE_POS);
	sprintf(str, "%d", score);
	OLED_printString(str);	
}

void Game_updateLives()
{
	lives--;
	OLED_goto(4, LIVES_POS);
	sprintf(str, "%d", lives);
	OLED_printString(str);
	
	OLED_goto(7, 30);
	OLED_printString("Wait");
}

void Game_play(uint8_t* SPIreceivedFlag)
{
	while(lives > 0)
	{
		// call for Joystick and Slider positions
		JoyPos = JOY_getPosition().X_abs;
		SliPos = SLI_getPosition().R_per;
		
		// Check for pushed Button
		if((PINE & (1<<PE2)))
		{
			ButtonRight = 1;
		}
		else
		{
			ButtonRight = 0;
		}
		
		// send Positions to Node 2
		message2send.id = 23;
		message2send.length = 3;
		message2send.data[0] = JoyPos;
		message2send.data[1] = SliPos;
		message2send.data[2] = ButtonRight;
		
		CAN_sendMessage(&message2send, 0);
		_delay_ms(50);
		
		// CAN reception
		if(*SPIreceivedFlag)
		{
		
		 	uint8_t receiveBufferStatus;
		 	// check for message reception
		 	if(receiveBufferStatus = 0x03 & MCP2515_read(SS_CAN_CONTROLLER, MCP_CANINTF))
		 	{
		 		struct can_message receivedMessage;
		 		receivedMessage = CAN_receiveMessage(receiveBufferStatus);
		
		 		CAN_printMessage(&receivedMessage);
		
		 		if(receivedMessage.id == 21)
		 		{
		 			Game_updateLives();
		 		}
				 
				if(receivedMessage.id == 13)
				{
					OLED_goto(7, 30);
					OLED_printString("    ");
				}
		
		 		*SPIreceivedFlag = 0;
		 	}
		}
	}
	
	OLED_clear();
	OLED_goto(3,5);
	OLED_printString("GAME OVER");
	OLED_goto(5,5);
	sprintf(str, "%d", score);
	OLED_printString(str);
	
	_delay_ms(2000);
}