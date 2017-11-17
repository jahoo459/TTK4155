/*
 * GameLib.h
 *
 * Created: 17.11.2017 09:43:20
 *  Author: lukasah
 */ 


#ifndef GAMELIB_H_
#define GAMELIB_H_

void Game_init(void);
void Game_updateLives(void);
void Game_updateScore(void);
void Game_play(uint8_t* SPIreceivedFlag, uint8_t* updateCmdDispFlag, UART_Message_t* uartMouseSteeringMessage, INPUT_MODE* inputMode);

#endif /* GAMELIB_H_ */