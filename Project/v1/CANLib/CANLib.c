/*
 * CANLib.c
 *
 * Created: 06.10.2017 17:42:15
 *  Author: lukasah
 */ 

#include <avr/io.h>
#include "CANLib.h"

int function(void)
{
    //TODO:: Please write your application code

    return 0;
}

void CAN_sendMessage(can_message_t* msg)
{
	//send ID
	//send length
	//send data
	int i = msg->length;
	
	for(i; i > 0; i++)
	{
		//MCP2515_write();
	}
}
