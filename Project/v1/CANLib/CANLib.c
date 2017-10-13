/*
 * CANLib.c
 *
 * Created: 06.10.2017 17:42:15
 *  Author: lukasah
 */ 

#include <avr/io.h>
#include "CANLib.h"
#include "..\MCP2515Lib\MCP2515Lib.h"

void CAN_sendMessage(can_message_t* msg, uint8_t transmitBufferNumber)
{
	// fill (one of three) transmit buffers
	switch(transmitBufferNumber)
	{
		// TXB0
		case 0:
			// register TXB0CTRL (30h)
			// configuration of flags and buffer priority
			MCP2515_bitModify(SS_CAN_CONTROLLER, MCP_TXB0CTRL, MCP_TXP1, 1);
			MCP2515_bitModify(SS_CAN_CONTROLLER, MCP_TXB0CTRL, MCP_TXP0, 1);
			
			// write ID to TXB0SIDH and TXB0SIDL
			uint16_t canID = 0b11111111111 & msg->id; // map id to 11 bits
			MCP2515_write(SS_CAN_CONTROLLER, MCP_TXB0SIDH, canID>>4); // write first 7 bits to TXB0SIDH Register
			MCP2515_write(SS_CAN_CONTROLLER, MCP_TXB0SIDL, 0b00000001111 & canID); // write last 4 bits to TXB0SIDL Register
			
			// write Data length to TXB0DLC Register
			uint8_t canDataLength = 0b1111 & msg->length;
			MCP2515_bitModify(SS_CAN_CONTROLLER, MCP_TXB0DLC, MCP_DLC3, 0b1000 & canDataLength);	
			MCP2515_bitModify(SS_CAN_CONTROLLER, MCP_TXB0DLC, MCP_DLC2, 0b0100 & canDataLength);
			MCP2515_bitModify(SS_CAN_CONTROLLER, MCP_TXB0DLC, MCP_DLC1, 0b0010 & canDataLength);
			MCP2515_bitModify(SS_CAN_CONTROLLER, MCP_TXB0DLC, MCP_DLC0, 0b0001 & canDataLength);
			
			// write Data to TXB0Dm Register
			int i = msg->length;
			
			for(i; i > 0; i++);
			{
				//MCP2515_write();
			}
		break;
		
	}
	//send ID
	//send length
	//send data
	
}
