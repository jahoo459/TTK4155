/*
 * PWMLib.h
 *
 * Created: 27.10.2017 09:28:30
 *  Author: lukasah
 */ 


#ifndef PWMLIB_H_
#define PWMLIB_H_

void PWM_init(void);
void PWM_setLevel(uint8_t level);
void Solenoid_on(void);

#endif /* PWMLIB_H_ */