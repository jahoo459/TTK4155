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
	
	// configure output pins
	// PH6 ~ !RST, PH5 ~ !OE, PH4 ~ EN, PH3 ~ SEL, PH1 ~ DIR
	DDRH |= (1<<DDH6) | (1<<DDH5) | (1<<DDH4) | (1<<DDH3) | (1<<DDH1);
	// set default values
	set_bit(MOT_ENC_REG, MOTOR_DIR_PIN); // 1 as default 
	set_bit(MOT_ENC_REG, _ENC_OE_PIN);
	set_bit(MOT_ENC_REG, _ENC_RESET_PIN);
	set_bit(MOT_ENC_REG, MOTOR_EN_PIN);
	
	//Toggle !RST to reset encoder
	clear_bit(MOT_ENC_REG, _ENC_RESET_PIN);
	_delay_us(20);
	set_bit(MOT_ENC_REG, _ENC_RESET_PIN);
	
}

void Motor_setSpeed(uint8_t joystick_position)
{
	uint8_t speed;
	uint8_t step = 3;
	
	//set the direction
	if(joystick_position >= 50) //right
	{
		set_bit(PORTH, MOTOR_DIR_PIN);
		//step = 255/50 assume 5
		speed = (joystick_position - 50) * step;
	}
	else
	{
		clear_bit(PORTH, MOTOR_DIR_PIN);
		speed = (50 - joystick_position) * step;
	}
	

	uint8_t msgSize = 3;
	unsigned char msg2send[msgSize];
	
	msg2send[0] = TWI_MAX520;
	msg2send[1] = 0x00;
	msg2send[2] = speed;
	
	TWI_Start_Transceiver_With_Data(msg2send, msgSize);
}

int Motor_readEncoder()
{
	//Set !OE low to enable output of encoder
	clear_bit(MOT_ENC_REG, _ENC_OE_PIN);
	//Set SEL low to get high byte
	clear_bit(MOT_ENC_REG, ENC_SEL_PIN);
	//Wait about 20 microseconds
	_delay_us(50);
	//Read MSB
	int value = PINK << 8;
	//Set SEL high to get low byte
	set_bit(MOT_ENC_REG, ENC_SEL_PIN);
	//Wait about 20 microseconds
	_delay_us(50);
	//Read LSB
	value |= PINK;
	//Toggle !RST to reset encoder
	//clear_bit(MOT_ENC_REG, _ENC_RESET_PIN);
	//_delay_us(20);
	//set_bit(MOT_ENC_REG, _ENC_RESET_PIN);
	//Set !OE high to disable output of encoder
	set_bit(MOT_ENC_REG, _ENC_OE_PIN);
	return value;
}