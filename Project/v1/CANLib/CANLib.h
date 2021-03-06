/*
 * CANLib.h
 *
 * Created: 06.10.2017 17:42:47
 *  Author: lukasah
 */ 


#ifndef CANLIB_H_
#define CANLIB_H_

#include "..\MCP2515Lib\MCP2515Lib.h"

void CAN_init(void);
void CAN_sendMessage(can_message_t* msg, uint8_t transmitBufferNumber);
can_message_t CAN_receiveMessage(uint8_t receiveBufferStatus);
void CAN_printMessage(can_message_t* msg);

#endif /* CANLIB_H_ */