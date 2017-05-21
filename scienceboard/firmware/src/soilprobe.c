/* OSU Robotics Club Rover 2017
 * Science Board Firmware
 *
 * soilprobe.c - Soil probe module
 * Author(s): Aaron Cohen
 */

#include <alloca.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include "soilprobe.h"


static uint8_t _cmd_buf_size(enum soilprobe_cmd_name cmd)
{
	switch (cmd) {
	case CMD_GET_SERIAL:
		return 9;
	case CMD_GET_VERSION:
		return 9;
	case CMD_GET_ADDRESS:
		return 9;
	case CMD_SET_ADDRESS:
		return 19;
	case CMD_GET_SOIL_TYPE:
		return 9;
	case CMD_SET_SOIL_TYPE:
		return 9;
	case CMD_TAKE_READING:
		return 7;
	case CMD_GET_READING:
		return 7;
	default:  /* Invalid command */
		return 0;
	}
}


static uint8_t _max_resp_buf_size(enum soilprobe_cmd_name cmd)
{
	switch (cmd) {
	case CMD_GET_SERIAL:
		return 13;
	case CMD_GET_VERSION:
		return 9;
	case CMD_GET_ADDRESS:
		return 5;
	case CMD_SET_ADDRESS:
		return 5;
	case CMD_GET_SOIL_TYPE:
		return 5;
	case CMD_SET_SOIL_TYPE:
		return 6;
	case CMD_TAKE_READING:
		return 0;
	case CMD_GET_READING:
		return 105;
	default:  /* Invalid command */
		return 0;
	}
}


static uint8_t * _cmd_str(enum soilprobe_cmd_name cmd)
{
	switch (cmd) {
	case CMD_GET_SERIAL:
		return (uint8_t *) "SN=?";
	case CMD_GET_VERSION:
		return (uint8_t *) "FV=?";
	case CMD_GET_ADDRESS:
		return (uint8_t *) "AD=?";
	case CMD_SET_ADDRESS:
		return (uint8_t *) "AD=";
	case CMD_GET_SOIL_TYPE:
		return (uint8_t *) "ST=?";
	case CMD_SET_SOIL_TYPE:
		return (uint8_t *) "ST=";
	case CMD_TAKE_READING:
		return (uint8_t *) "TR";
	case CMD_GET_READING:
		return (uint8_t *) "T";
	default:  /* Invalid command */
		return (uint8_t *) "!!";
	}
}


void soilprobe_init(void)
{
	/* UART initialized to 9600 8N1 */
	/* 2X UART speed */
	UCSR0A = _BV(U2X0);

	/* Enable UART receive and transmit */
	UCSR0B = _BV(RXEN0) | _BV(TXEN0);

	/* Baud Rate = 9600 */
	UBRR0L = 207;

	/* Set up IC enable/disable line */
	DDRC |= _BV(PC0);
}


void soilprobe_cmd(struct soilprobe_cmd *cmd, struct soilprobe_resp *resp)
{
	/* Command size and maximum response size */
	uint8_t cmdbufsize = _cmd_buf_size(cmd->cmd);
	uint8_t maxrespbufsize = _max_resp_buf_size(cmd->cmd);

	/* Command buffers read/written by UART module */
	uint8_t *cmdbuf = alloca(cmdbufsize);
	uint8_t *respbuf = alloca(maxrespbufsize);

	/* Command name enum to string representation */
	uint8_t *cmdstr = _cmd_str(cmd->cmd);

	/* Copy command address */
	memcpy(cmdbuf, cmd->addr, 3);

	/* Copy command string, args, and CRLF */
	if (cmd->cmd == CMD_GET_READING)  /* Get Reading */
	{
		cmdbuf[3] = cmdstr[0];
		cmdbuf[4] = cmd->arg[0];
		memcpy(&(cmdbuf[5]), "\x0D\x0A", 2);
	}
	else if (cmd->cmd == CMD_TAKE_READING)  /* Take Reading */
	{
		memcpy(&(cmdbuf[3]), cmdstr, 2);
		memcpy(&(cmdbuf[5]), "\x0D\x0A", 2);
	}
	else if (cmd->cmd == CMD_SET_ADDRESS || cmd->cmd == CMD_SET_SOIL_TYPE)  /* Set commands */
	{
		memcpy(&(cmdbuf[3]), cmdstr, 3);
		memcpy(&(cmdbuf[6]), cmd->arg, cmd->argsize);
		memcpy(&(cmdbuf[6 + cmd->argsize]), "\x0D\x0A", 2);
	}
	else  /* Get commands */
	{
		memcpy(&(cmdbuf[3]), cmdstr, 4);
		memcpy(&(cmdbuf[7]), "\x0D\x0A", 2);
	}

	/* Enable RS485 Driver */
	PORTC |= _BV(PC0);

	/* Transmit command */
	for (uint8_t i = 0; i < cmdbufsize; i++)
	{
		UDR0 = cmdbuf[i];
		while(!(UCSR0A & _BV(TXC0)));
	}


	/* No response for "Take Reading" command */
	if (cmd->cmd == CMD_TAKE_READING)
		return;

	/* Enable RS485 Receiver */
	PORTC &= ~_BV(PC0);

	/* Current size of response buffer */
	uint8_t respbufsize = 0;

	/* Receive response */
	while(respbufsize < 2
	   || (respbuf[respbufsize - 2] == '\x0D' && respbuf[respbufsize - 1] == '\x0A'))
	{
		while(!(UCSR0A & _BV(RXC0)));
		respbuf[respbufsize] = UDR0;
		respbufsize += 1;

		/* If we've gone over our max size, something went wrong */
		if (respbufsize > maxrespbufsize)
			break;
	}

	/* Parse response */
	memcpy(resp->addr, respbuf, 3);
	memcpy(resp->data, &(respbuf[3]), respbufsize - 5);
	resp->datasize = respbufsize - 5;
}
