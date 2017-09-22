/*
 * oledLib.h
 *
 * Created: 9/22/2017 1:23:02 PM
 *  Author: Jasiek
 */ 


#ifndef OLEDLIB_H_
#define OLEDLIB_H_

void OLED_init();
//void OLED_reset();
//void OLED_home();
//void OLED_goto_line(int line);
//void OLED_clear_line(int line);
//void OLED_pos(int row, int column);
//void OLED_print(char* text);
//void OLED_set_brightness(int lvl);
//
//void OLED_print_arrow(int row, int col);

void writeByteToOLED(volatile uint8_t *address, uint8_t data);



#endif /* OLEDLIB_H_ */