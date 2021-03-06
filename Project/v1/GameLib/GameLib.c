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

#define LIVES_POS 35
#define SCORE_POS 84

static char str[3];
static uint16_t score;
static uint8_t lives;
static uint8_t updateScoreFlag = 1;

struct can_message message2send;
struct can_message message3send;
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
	OLED_bufferGoto(0,4);
	OLED_printString("** Ping-Pong **");
		
	// write to Screen
	OLED_bufferGoto(3,LIVES_POS-16);
	OLED_printString("Lives");
	OLED_bufferGoto(3,SCORE_POS-8);
	OLED_printString("Score");
		
	// write Numbers
	OLED_bufferGoto(5, LIVES_POS);
	sprintf(str, "%d", lives);
	OLED_printString(str);
	OLED_bufferGoto(5, SCORE_POS);
	sprintf(str, "%d", score);
	OLED_printString(str);	
}

void Game_updateLives()
{
	lives--;
	OLED_bufferGoto(5, LIVES_POS);
	sprintf(str, "%d", lives);
	OLED_printString(str);
	
	OLED_bufferGoto(7, 48);
	OLED_printString("Wait");
	updateScoreFlag = 0;
}

void Game_updateScore()
{
	score++;
	OLED_bufferGoto(5, SCORE_POS);
	sprintf(str, "%d", score);
	OLED_printString(str);
}

void Game_play(uint8_t* SPIreceivedFlag, uint8_t* updateCmdDispFlag, UART_Message_t* uartMouseSteeringMessage, INPUT_MODE* inputMode)
{
	while(lives > 0)
	{
		if(updateScoreFlag)
		{
			Game_updateScore();
		}
				
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
		message2send.length = 7;
		message2send.data[0] = JoyPos;
		message2send.data[1] = SliPos;
		message2send.data[2] = ButtonRight;
		message2send.data[3] = uartMouseSteeringMessage->Motor;
		message2send.data[4] = uartMouseSteeringMessage->Servo;
		message2send.data[5] = uartMouseSteeringMessage->Button;
		message2send.data[6] = *inputMode;

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
					OLED_bufferGoto(7, 48);
					OLED_printString("    ");
					updateScoreFlag = 1;
				}
		
		 		*SPIreceivedFlag = 0;
		 	}
		}
	}
	
	OLED_clear();
	OLED_bufferGoto(2,28);
	OLED_printString("GAME OVER");
	OLED_bufferGoto(4,44);
	OLED_printString("SCORE");
	OLED_bufferGoto(5,52);
	sprintf(str, "%d", score);
	OLED_printString(str);
	
	_delay_ms(2000);
}