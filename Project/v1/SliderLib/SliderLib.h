/*
 * SliderLib.h
 *
 * Created: 21.09.2017 15:53:00
 *  Author: janwh
 */


#ifndef SLIDERLIB_H_
#define SLIDERLIB_H_

#include "..\ADCLib\ADCLib.h"

void SLI_init();
void SLI_requestCurrentPosition(uint8_t side);
void SLI_updatePosition(uint8_t side);
void SLI_button(uint8_t side);
SLI_position_t SLI_getPosition();
void SLI_printPosAndDir();

#endif /* SLIDERLIB_H_ */
