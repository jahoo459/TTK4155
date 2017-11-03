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

//PINOUT
#define _ENC_OE_PIN PH5
#define _ENC_RESET_PIN PH6
#define MOTOR_EN_PIN PH4
#define ENC_SEL_PIN PH3
#define MOTOR_DIR_PIN PH1

#define MOT_ENC_REG PORTH

void Motor_init(void);
void Motor_setSpeed(uint8_t joystick_position);
int Motor_readEncoder();

#endif /* MOTORLIB_H_ */