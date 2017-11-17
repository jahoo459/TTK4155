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
	// enable Receive Buffer Full Interrupts
    MCP2515_write(SS_CAN_CONTROLLER, MCP_CANINTE, (1<<MCP_RX1IE)|(1<<MCP_RX0IE) );
   
	// SETUP FILTERS    
    // set both buffers to accept all messages
    MCP2515_write(SS_CAN_CONTROLLER, MCP_RXB0CTRL, (1<<MCP_RXM1)|(1<<MCP_RXM0) );
	
	// switch to normal mode
	MCP2515_bitModify(SS_CAN_CONTROLLER, MCP_CANCTRL, 0x80, 0x00);
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
			
			// write ID to TXB0SIDH and TXB0SIDL
			unsigned int canID;
			canID = msg->id; 
			MCP2515_write(SS_CAN_CONTROLLER, MCP_TXB0SIDL, (0x0007 & canID)<<5); // write last 3 bits to first 3 bits in TXB0SIDL Register
			MCP2515_write(SS_CAN_CONTROLLER, MCP_TXB0SIDH, canID>>3); // write first 8 bits to TXB0SIDH Register		
			
			// write Data length to TXB0DLC Register
			MCP2515_write(SS_CAN_CONTROLLER, MCP_TXB0DLC, msg->length);
			
			uint8_t dataRegister = MCP_TXB0D0;
			// write Data to TXB0Dm Register			
			for(uint8_t i = 0; i < msg->length; i++)
			{
				MCP2515_write(SS_CAN_CONTROLLER, dataRegister, msg->data[i]);
				dataRegister ++;
			}
			
			// initiating transmission
			MCP2515_bitModify(SS_CAN_CONTROLLER, MCP_TXB0CTRL, (1<<MCP_TXREQ), 0xff);
		break;
		}
	}
}
	
	
can_message_t CAN_receiveMessage(uint8_t receiveBufferStatus)
{
	// auxiliary variables for the following switch case
	struct can_message receivedMessage;
	uint8_t dataRegister;
	
	// read receive buffer depending on where the received message was saved
	switch(receiveBufferStatus)
	{
		// prioritize RXB0
		case 1:
		case 3:
			// read higher bits from RXBnSIDH
			receivedMessage.id = MCP2515_read(SS_CAN_CONTROLLER, MCP_RXB0SIDH);
			// read lower bits from RXBnSIDL
			receivedMessage.id = receivedMessage.id<<3 | (MCP2515_read(SS_CAN_CONTROLLER, MCP_RXB0SIDL)>>5);

			// read message length from RXBnDLC
			receivedMessage.length = MCP2515_read(SS_CAN_CONTROLLER, MCP_RXB0DLC);

			// start reading message data from lowest register
			dataRegister = MCP_RXB0D0;
			// increment data register depending on message length
			for(uint8_t i = 0; i < receivedMessage.length; i++)
			{
				// read message data from data register
				receivedMessage.data[i] = MCP2515_read(SS_CAN_CONTROLLER, dataRegister);
				dataRegister++;
			}
	
			// clear receive flag of corresponding buffer
			MCP2515_bitModify(SS_CAN_CONTROLLER, MCP_CANINTF, MCP_RX0IF, 0x00);
	
			return receivedMessage;
			
		case 2:
			// read higher bits from RXBnSIDH
			receivedMessage.id = MCP2515_read(SS_CAN_CONTROLLER, MCP_RXB1SIDH);
			// read lower bits from RXBnSIDL
			receivedMessage.id = receivedMessage.id<<3 | (MCP2515_read(SS_CAN_CONTROLLER, MCP_RXB1SIDL)>>5);

			// read message length from RXBnDLC
			receivedMessage.length = MCP2515_read(SS_CAN_CONTROLLER, MCP_RXB1DLC);

			// start reading message data from lowest register
			dataRegister = MCP_RXB1D0;
			// increment data register depending on message length
			for(uint8_t i = 0; i < receivedMessage.length; i++)
			{
				// read message data from data register
				receivedMessage.data[i] = MCP2515_read(SS_CAN_CONTROLLER, dataRegister);
				dataRegister++;
			}
		
			// clear receive flag of corresponding buffer
			MCP2515_bitModify(SS_CAN_CONTROLLER, MCP_CANINTF, MCP_RX1IF, 0x00);
		
			return receivedMessage;
	}
}

void CAN_printMessage(can_message_t* msg)
{
	printf("received message: id: %d, length: %d, data:", msg->id, msg->length);
	for(uint8_t i = 0; i < msg->length; i++)
	{
		printf(" %c", msg->data[i]);
	}
	printf("\n");
}