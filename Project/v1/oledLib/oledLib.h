/*
 * oledLib.h
 *
 * Created: 9/22/2017 1:23:02 PM
 *  Author: Jasiek
 */ 


#ifndef OLEDLIB_H_
#define OLEDLIB_H_

void OLED_init(void);
void OLED_clear(void);

void OLED_writeByteToOLED(volatile uint8_t *address, uint8_t data);
void OLED_goto(int row, int column);

void OLED_printCharacter(uint8_t character);
void OLED_printString(char* msg);

void OLED_printArrow(void);
void OLED_clearArrow(void);
void OLED_moveArrow(int joy_counter);

//void OLED_reset();
//void OLED_home();
//void OLED_goto_line(int line);
//void OLED_clear_line(int line);
//void OLED_pos(int row, int column);
//void OLED_print(char* text);
//void OLED_set_brightness(int lvl);

#endif /* OLEDLIB_H_ */