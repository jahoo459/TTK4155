#ifndef __MCP2515_H
#define __MCP2515_H

/*
mcp2515.h

This file contains constants that are specific to the MCP2515.

Version     Date        Description
----------------------------------------------------------------------
v1.00       2003/12/11  Initial release

Copyright 2003 Kimberly Otten Software Consulting
*/

// Define MCP2515 register addresses

#define MCP_RXF0SIDH	0x00
#define MCP_RXF0SIDL	0x01
#define MCP_RXF0EID8	0x02
#define MCP_RXF0EID0	0x03
#define MCP_RXF1SIDH	0x04
#define MCP_RXF1SIDL	0x05
#define MCP_RXF1EID8	0x06
#define MCP_RXF1EID0	0x07
#define MCP_RXF2SIDH	0x08
#define MCP_RXF2SIDL	0x09
#define MCP_RXF2EID8	0x0A
#define MCP_RXF2EID0	0x0B
#define MCP_CANSTAT		0x0E
#define MCP_CANCTRL		0x0F
#define MCP_RXF3SIDH	0x10
#define MCP_RXF3SIDL	0x11
#define MCP_RXF3EID8	0x12
#define MCP_RXF3EID0	0x13
#define MCP_RXF4SIDH	0x14
#define MCP_RXF4SIDL	0x15
#define MCP_RXF4EID8	0x16
#define MCP_RXF4EID0	0x17
#define MCP_RXF5SIDH	0x18
#define MCP_RXF5SIDL	0x19
#define MCP_RXF5EID8	0x1A
#define MCP_RXF5EID0	0x1B
#define MCP_TEC			0x1C
#define MCP_REC			0x1D
#define MCP_RXM0SIDH	0x20
#define MCP_RXM0SIDL	0x21
#define MCP_RXM0EID8	0x22
#define MCP_RXM0EID0	0x23
#define MCP_RXM1SIDH	0x24
#define MCP_RXM1SIDL	0x25
#define MCP_RXM1EID8	0x26
#define MCP_RXM1EID0	0x27
#define MCP_CNF3		0x28
#define MCP_CNF2		0x29
#define MCP_CNF1		0x2A
#define MCP_CANINTE		0x2B
#define MCP_CANINTF		0x2C
#define MCP_EFLG		0x2D

#define MCP_TXB0CTRL	0x30
#define MCP_TXB0SIDH	0x31
#define MCP_TXB0SIDL	0x32
#define MCP_TXB0EID8	0x33
#define MCP_TXB0EID0	0x34
#define MCP_TXB0DLC		0x35
#define MCP_TXB0D0		0x36
#define MCP_TXB0D1		0x37
#define MCP_TXB0D2		0x38
#define MCP_TXB0D3		0x39
#define MCP_TXB0D4		0x3A
#define MCP_TXB0D5		0x3B
#define MCP_TXB0D6		0x3C
#define MCP_TXB0D7		0x3D

#define MCP_TXB1CTRL	0x40
#define MCP_TXB2CTRL	0x50

#define MCP_RXB0CTRL	0x60
#define MCP_RXB0SIDH	0x61
#define MCP_RXB0SIDL	0x62
#define MCP_RXB0EID8	0x63
#define MCP_RXB0EID0	0x64
#define MCP_RXB0DLC		0x65
#define MCP_RXB0D0		0x66
#define MCP_RXB0D1		0x67
#define MCP_RXB0D2		0x68
#define MCP_RXB0D3		0x69
#define MCP_RXB0D4		0x6A
#define MCP_RXB0D5		0x6B
#define MCP_RXB0D6		0x6C
#define MCP_RXB0D7		0x6D

#define MCP_RXB1CTRL	0x70
#define MCP_RXB1SIDH	0x71
#define MCP_RXB1SIDL	0x72
#define MCP_RXB1EID8	0x73
#define MCP_RXB1EID0	0x74
#define MCP_RXB1DLC		0x75
#define MCP_RXB1D0		0x76
#define MCP_RXB1D1		0x77
#define MCP_RXB1D2		0x78
#define MCP_RXB1D3		0x79
#define MCP_RXB1D4		0x7A
#define MCP_RXB1D5		0x7B
#define MCP_RXB1D6		0x7C
#define MCP_RXB1D7		0x7D

#define MCP_TX_INT		0x1C		// Enable all transmit interrupts
#define MCP_TX01_INT	0x0C		// Enable TXB0 and TXB1 interrupts
#define MCP_RX_INT		0x03		// Enable receive interrupts
#define MCP_NO_INT		0x00		// Disable all interrupts

#define MCP_TX01_MASK	0x14
#define MCP_TX_MASK		0x54

// Define SPI Instruction Set

#define MCP_WRITE		0x02

#define MCP_READ		0x03

#define MCP_BITMOD		0x05

#define MCP_LOAD_TX0	0x40
#define MCP_LOAD_TX1	0x42
#define MCP_LOAD_TX2	0x44

#define MCP_RTS_TX0		0x81
#define MCP_RTS_TX1		0x82
#define MCP_RTS_TX2		0x84
#define MCP_RTS_ALL		0x87

#define MCP_READ_RX0	0x90
#define MCP_READ_RX1	0x94

#define MCP_READ_STATUS	0xA0

#define MCP_RX_STATUS	0xB0

#define MCP_RESET		0xC0


// CANCTRL Register Values

#define MODE_NORMAL     0x00
#define MODE_SLEEP      0x20
#define MODE_LOOPBACK   0x40
#define MODE_LISTENONLY 0x60
#define MODE_CONFIG     0x80
#define MODE_POWERUP	0xE0
#define MODE_MASK		0xE0
#define ABORT_TX        0x10
#define MODE_ONESHOT	0x08
#define CLKOUT_ENABLE	0x04
#define CLKOUT_DISABLE	0x00
#define CLKOUT_PS1		0x00
#define CLKOUT_PS2		0x01
#define CLKOUT_PS4		0x02
#define CLKOUT_PS8		0x03


// CNF1 Register Values

#define SJW1            0x00
#define SJW2            0x40
#define SJW3            0x80
#define SJW4            0xC0


// CNF2 Register Values

#define BTLMODE			0x80
#define SAMPLE_1X       0x00
#define SAMPLE_3X       0x40


// CNF3 Register Values

#define SOF_ENABLE		0x80
#define SOF_DISABLE		0x00
#define WAKFIL_ENABLE	0x40
#define WAKFIL_DISABLE	0x00

// CANCTRL
#define MCP_REQOP2		7
#define MCP_REQOP1		6
#define MCP_REQOP0		5
#define MCP_ABAT		4
#define MCP_CLKEN		2
#define MCP_CLKPRE1		1
#define MCP_CLKPRE0		0

// CANSTAT Register Bits
#define MCP_OPMOD2		7
#define MCP_OPMOD1		6
#define MCP_OPMOD0		5
#define MCP_ICOD2		3
#define MCP_ICOD1		2
#define MCP_ICOD0		1

// CANINTE Register Bits
#define MCP_MERRE		7
#define MCP_WAKIE		6
#define MCP_ERRIE		5
#define MCP_TX2IE		4
#define MCP_TX1IE		3
#define MCP_TX0IE		2
#define MCP_RX1IE		1
#define MCP_RX0IE		0

// CANINTF Register Bits

#define MCP_RX0IF		0x01
#define MCP_RX1IF		0x02
#define MCP_TX0IF		0x04
#define MCP_TX1IF		0x08
#define MCP_TX2IF		0x10
#define MCP_ERRIF		0x20
#define MCP_WAKIF		0x40
#define MCP_MERRF		0x80

// TXBnCTRL Register Bits

#define MCP_ABTF		6
#define MCP_MLOA		5
#define MCP_TXERR		4
#define MCP_TXREQ		3
#define MCP_TXP1		1
#define MCP_TXP0		0

// RXBnDLC (n = 0, 1) Register Bits

 #define MCP_RTR		6
 #define MCP_DLC3		3
 #define MCP_DLC2		2
 #define MCP_DLC1		1
 #define MCP_DLC0		0
 
// RXB0CTRL Register Bits
 #define MCP_RXM1			6
 #define MCP_RXM0			5
 #define MCP_RXRTR			3
 #define MCP_BUKT			2
 #define MCP_BUKT1			1
 #define MCP_FILHIT0		0

#endif
