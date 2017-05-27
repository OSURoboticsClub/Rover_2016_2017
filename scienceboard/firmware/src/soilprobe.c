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
#define F_CPU 16000000UL
#include <util/delay.h>
#include "uart.h"
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
	uart_enable(0, 9600, 1, 0);

	/* Set up IC enable/disable line */
	DDRD |= _BV(PD3) | _BV(PD2);

	/* Enable RS485 Driver */
	PORTD |= _BV(PD2);
	PORTD |= _BV(PD3);
}


void soilprobe_send_cmd(struct soilprobe_cmd const *cmd, struct soilprobe_resp *resp)
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

	/* Transmit command */
	uart_tx(0, cmdbuf, cmdbufsize);
	while(!uart_tx_in_progress(0));

	/* No response for "Take Reading" command */
	if (cmd->cmd == CMD_TAKE_READING)
		return;

	/* Enable RS485 Receiver */
	PORTD &= ~_BV(PD2);
	PORTD &= ~_BV(PD3);

	/* Current size of response buffer */
	uint8_t respbufsize = 0;

	/* Number of bytes previously written */
	uint8_t respwritten = 0;

	/* Time since last byte received */
	uint16_t timesincelast = 0;

 	// Receive response
 	// Loop until all bytes written or response timeout
 	while (respbufsize < maxrespbufsize && timesincelast < 1000)
	{
 		respwritten = uart_rx(0, &(respbuf[respbufsize]), maxrespbufsize - respbufsize);

		// No bytes written since last loop
 		if (respwritten == 0)
		{
 			timesincelast += 1;  // Increase timeout
 		}
		else if (respbufsize + respwritten <= maxrespbufsize)  // Bytes written
		{
 			respbufsize += respwritten;
 			timesincelast = 0;
 		}
		else  // Total bytes written more than expected
		{
 			break;
 		}

 		// Check for CRLF packet ending (Useful for variable length "Transmit
 		// Reading" packet type)
 		if (respbufsize >= 2 && memcmp(&(respbuf[respbufsize - 2]), "\x0D\x0A", 2) == 0)
 			break;

 		_delay_us(1);
	}

	/* Parse response */
	//memcpy(resp->addr, respbuf, 3);
	//memcpy(resp->data, &(respbuf[3]), respbufsize - 5);
	//resp->datasize = respbufsize - 5;

	/* Enable RS485 Driver */
	PORTD |= _BV(PD2);
	PORTD |= _BV(PD3);
}
