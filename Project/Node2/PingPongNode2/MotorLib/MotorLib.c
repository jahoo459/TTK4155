/*
 * MotorLib.c
 *
 * Created: 03.11.2017 10:21:28
 *  Author: lukasah
 */ 


#include "MotorLib.h"

// global sum of the errors
int sumError = 0;

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

void Motor_JoySetSpeed(uint8_t joystick_position)
{
	uint8_t speed;
	uint8_t step = 3;
	
	//set the direction
	if(joystick_position >= 50) //right
	{
		Motor_setDirection(MOTOR_RIGHT);
		speed = (joystick_position - 50) * step;
	}
	else
	{
		Motor_setDirection(MOTOR_LEFT);
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
	//Set !OE high to disable output of encoder
	set_bit(MOT_ENC_REG, _ENC_OE_PIN);
	return value;
}

void Motor_do_PID(int desired_value, int actual_value)
{
	//first map the slider position (0-100%) to MAX encoder values
	desired_value = (LIMIT_ENC_RIGHT - LIMIT_ENC_LEFT)/100 * desired_value + LIMIT_ENC_LEFT;
	int error = desired_value - actual_value;
	if(abs(error) > 10)
	{
		sumError += error;
	}
	
	int u = P_GAIN * error + I_GAIN * sumError;

	Motor_setSpeed(u);
}

void Motor_setSpeed(int speed)
{
	if(speed < 0)
	{
		Motor_setDirection(MOTOR_RIGHT);
		speed = abs(speed);
	}
	else
	{
		Motor_setDirection(MOTOR_LEFT);
	}
	
	if(speed > 120)
	{
		speed = 120;
	}
	
	uint8_t msgSize = 3;
	unsigned char msg2send[msgSize];
	
	msg2send[0] = TWI_MAX520;
	msg2send[1] = 0x00;
	msg2send[2] = speed;
	
	TWI_Start_Transceiver_With_Data(msg2send, msgSize);
}

void Motor_setDirection(MOTOR_DIR_t motorDir)
{
	if(motorDir == MOTOR_LEFT)
	{
		clear_bit(MOT_ENC_REG, MOTOR_DIR_PIN);
	}
	else
	{
		set_bit(MOT_ENC_REG, MOTOR_DIR_PIN);
	}
}
