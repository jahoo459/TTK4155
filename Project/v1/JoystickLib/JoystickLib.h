/*
 * JoystickLib.h
 *
 * Created: 15.09.2017 11:45:06
 *  Author: janwh
 */


#ifndef JOYSTICKLIB_H_
#define JOYSTICKLIB_H_

#include "..\ADCLib\ADCLib.h"

void JOY_init();
void JOY_calibrate();
void JOY_requestCurrentPosition(char axis);
void JOY_updatePosition(char axis);
void JOY_calculateDirection();
JOY_position_t JOY_getPosition();
void JOY_printPosAndDir();

// char JOY_button(int button);
// void JOY_updateDirection();
//
// uint8_t JOY_getPositionX();
// uint8_t JOY_getPositionY();
// void JOY_calibrate();

#endif /* JOYSTICKLIB_H_ */
