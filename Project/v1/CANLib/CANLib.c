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
		{
			// register TXB0CTRL (30h)
			// configuration of flags and buffer priority
			//MCP2515_bitModify(SS_CAN_CONTROLLER, MCP_TXB0CTRL,);
			//MCP2515_bitModify(SS_CAN_CONTROLLER, MCP_TXB0CTRL, );
			
			// write ID to TXB0SIDH and TXB0SIDL
			unsigned int canID;
			canID = msg->id; 
			MCP2515_write(SS_CAN_CONTROLLER, MCP_TXB0SIDH, canID>>3); // write first 7 bits to TXB0SIDH Register
			MCP2515_write(SS_CAN_CONTROLLER, MCP_TXB0SIDL, 0x0003 & canID); // write last 4 bits to TXB0SIDL Register
			
			// write Data length to TXB0DLC Register
			// TODO: only modify the lowest 4 bits
			MCP2515_write(SS_CAN_CONTROLLER, MCP_TXB0DLC, msg->length);
			
			uint8_t dataRegister = MCP_TXB0D0;
			// write Data to TXB0Dm Register			
			for(uint8_t i = 0; i < msg->length; i++)
			{
				dataRegister ++;
				MCP2515_write(SS_CAN_CONTROLLER, dataRegister, msg->data[i]);
			}
			
			// initiating transmission
			MCP2515_bitModify(SS_CAN_CONTROLLER, MCP_TXB0CTRL, 0x08, 0x08);
		break;
		}
	}
	//send ID
	//send length
	//send data
	
}
