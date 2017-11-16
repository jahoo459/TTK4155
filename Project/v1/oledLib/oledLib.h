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

void OLED_updateScreen(void);

void OLED_writeByteToOLED(volatile uint8_t *address, uint8_t data);
void OLED_writePixelToOLED(uint8_t u, uint8_t v);
void OLED_goto(int row, int column);
void OLED_bufferGoto(int row, int column);

void OLED_printCharacter(uint8_t character);
void OLED_printString(char* msg);

void OLED_printArrow(void);
void OLED_clearArrow(void);
void OLED_moveArrow(int joy_counter);
void OLED_resetArrow(void);

void OLED_setAnimationTick(void);

void OLED_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void OLED_circle(uint8_t x0, uint8_t y0, uint8_t r);

void OLED_logo(void);
void OLED_table(void);
void OLED_splashScreen(void);
// void OLED_flyingArrows(void);

//void OLED_reset();
//void OLED_home();
//void OLED_goto_line(int line);
//void OLED_clear_line(int line);
//void OLED_pos(int row, int column);
//void OLED_print(char* text);
//void OLED_set_brightness(int lvl);

#endif /* OLEDLIB_H_ */