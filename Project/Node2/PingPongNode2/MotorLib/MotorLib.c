/*
 * MotorLib.c
 *
 * Created: 03.11.2017 10:21:28
 *  Author: lukasah
 */ 


#include "MotorLib.h"


void Motor_init()
{
	TWI_Master_Initialise();
	// configure the DIR pin
	
	set_bit(DDRH, DDH1);
	set_bit(PORTH, PH1); // 1 as default 
	
}

void Motor_setSpeed(uint8_t joystick_position)
{
	uint8_t speed;
	uint8_t step = 3;
	
	//set the direction
	if(joystick_position >= 50) //right
	{
		set_bit(PORTH, PH1);
		//step = 255/50 assume 5
		speed = (joystick_position - 50) * step;
	}
	else
	{
		clear_bit(PORTH, PH1);
		speed = (50 - joystick_position) * step;
	}
	

	uint8_t msgSize = 3;
	unsigned char msg2send[msgSize];
	
	msg2send[0] = TWI_MAX520;
	msg2send[1] = 0x00;
	msg2send[2] = speed;
	
	TWI_Start_Transceiver_With_Data(msg2send, msgSize);
}