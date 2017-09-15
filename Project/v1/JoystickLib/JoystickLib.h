/*
 * JoystickLib.h
 *
 * Created: 15.09.2017 11:45:06
 *  Author: janwh
 */ 


#ifndef JOYSTICKLIB_H_
#define JOYSTICKLIB_H_

#include "..\ADCLib\ADCLib.h"


typedef struct  {
	int X;
	int Y;
} JOY_position_t;

typedef enum {CENTRE, UP, DOWN, RIGHT, LEFT} JOY_direction_t;

void JOY_init();
void JOY_calibrate();
char JOY_button(int button);
JOY_position_t JOY_getPosition();
JOY_direction_t JOY_getDirection();
void JOY_updatePosition(char axis);
void JOY_updateDirection();
void JOY_requestCurrentPosition(char axis);
unsigned char JOY_getPositionX();
unsigned char JOY_getPositionY();
#endif /* JOYSTICKLIB_H_ */