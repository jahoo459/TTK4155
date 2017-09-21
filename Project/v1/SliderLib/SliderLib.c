/*
 * SliderLib.c
 *
 * Created: 15.09.2017 17:46:32
 *  Author: janwh
 */

#include <avr/io.h>

#include "SliderLib.h"
#include "..\ADCLib\ADCLib.h"

// channels of sliders for AD conversion
#define L_channel 3
#define R_channel 4

// current position of sliders
SLI_position_t currentSliPosition;
SLI_buttons_t currentSliButtons;

//------------------------------------------------------------------------------
// set initial values for Position and Buttons
void SLI_init()
{
  currentSliPosition.R_abs = 0;
  currentSliPosition.L_abs = 0;
  currentSliPosition.R_per = 0;
  currentSliPosition.L_per = 0;

  currentSliButtons.leftButton = 0;
  currentSliButtons.rightButton = 0;
}

//------------------------------------------------------------------------------
// This function requests an AD conversion for the specified slider.
void SLI_requestCurrentPosition(uint8_t side)
{
  if(side == 'l')
	{
		ADC_request(L_channel, adc_ext_ram);
	}

	else if(side == 'r')
	{
		ADC_request(R_channel, adc_ext_ram);
	}
}

//------------------------------------------------------------------------------
// This function is called after the interrupt for completed conversion was
// received.
void SLI_updatePosition(uint8_t side)
{
  if(side == 'l')
	{
		currentSliPosition.L_abs = ADC_read(adc_ext_ram);
		currentSliPosition.L_per = currentSliPosition.L_abs/256;
	}

	else if(side == 'r')
	{
    currentSliPosition.R_abs = ADC_read(adc_ext_ram);
		currentSliPosition.R_per = currentSliPosition.R_abs/256;
	}
}

//------------------------------------------------------------------------------
// This function is called after the interrupt for pressed button was
// received.
void SLI_button(uint8_t side)
{
  if(side == 'l')
  {
    currentSliButtons.leftButton = 1;
  }

  else if(side == 'r')
  {
    currentSliButtons.rightButton = 1;
  }
}

//------------------------------------------------------------------------------
//
SLI_position_t SLI_getPosition()
{
  return currentSliPosition;
}

void SLI_printPosAndBtn();
