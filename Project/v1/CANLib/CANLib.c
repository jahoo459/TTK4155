/*
 * CANLib.c
 *
 * Created: 06.10.2017 17:42:15
 *  Author: lukasah
 */ 

#include <avr/io.h>
#include "CANLib.h"

void CAN_init()
{
	// activate Rx Buffer Interrupts
    MCP2515_write(SS_CAN_CONTROLLER, MCP_CANINTE, (1<<MCP_RX1IE)|(1<<MCP_RX0IE) );
   
	// SETUP FILTERS    
    // set both buffers to accept all messages
    MCP2515_write(SS_CAN_CONTROLLER, MCP_RXB0CTRL, (1<<MCP_RXM1)|(1<<MCP_RXM0) ); // buffer 0
   
    // delete all bits from reception mask -> accept all messages
    //mcp2515_write_register( RXM0SIDH, 0 );
    //mcp2515_write_register( RXM0SIDL, 0 );
    //mcp2515_write_register( RXM0EID8, 0 );
    //mcp2515_write_register( RXM0EID0, 0 );
   
    // switch to loopback mode
    MCP2515_bitModify(SS_CAN_CONTROLLER, MCP_CANCTRL, 0xc0, 0x40);
}

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
			printf("canID: %#x\n", canID);
			MCP2515_write(SS_CAN_CONTROLLER, MCP_TXB0SIDL, (0x0007 & canID)<<5); // write last 4 bits to TXB0SIDL Register
			printf("SIDL: %#x\n", 0x0007 & canID);
			MCP2515_write(SS_CAN_CONTROLLER, MCP_TXB0SIDH, canID>>3); // write first 7 bits to TXB0SIDH Register		
			printf("SIDH: %#x\n", canID>>3);
			
			// write Data length to TXB0DLC Register
			// TODO: only modify the lowest 4 bits
			MCP2515_write(SS_CAN_CONTROLLER, MCP_TXB0DLC, msg->length);
			
			uint8_t dataRegister = MCP_TXB0D0;
			// write Data to TXB0Dm Register			
			for(uint8_t i = 0; i < msg->length; i++)
			{
				MCP2515_write(SS_CAN_CONTROLLER, dataRegister, msg->data[i]);
				dataRegister ++;
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
