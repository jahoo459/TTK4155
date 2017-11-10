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

// PID controller
//#define P_GAIN 0.031
#define P_GAIN 0.031
#define I_GAIN 0.005
#define MAX_ENC_LEFT 8870
#define MAX_ENC_RIGHT 0
#define LIMIT_ENC_LEFT 8300
#define LIMIT_ENC_RIGHT 300

typedef enum {MOTOR_LEFT, MOTOR_RIGHT} MOTOR_DIR_t;

void Motor_init(void);
void Motor_JoySetSpeed(uint8_t joystick_position);
void Motor_setSpeed(int speed);
int Motor_readEncoder();
void Motor_do_PID(int desired_value, int actual_value);
void Motor_setDirection(MOTOR_DIR_t motorDir);

#endif /* MOTORLIB_H_ */