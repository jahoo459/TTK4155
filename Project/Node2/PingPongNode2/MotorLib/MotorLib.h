/*
 * MotorLib.h
 *
 * Created: 03.11.2017 10:23:11
 *  Author: lukasah
 */ 


#ifndef MOTORLIB_H_
#define MOTORLIB_H_

#include "TWI_Master.h"
#include "../Node2Main/definitions.h"

void Motor_init(void);
void Motor_setSpeed(uint8_t joystick_position);

#endif /* MOTORLIB_H_ */