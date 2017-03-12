/* OSU Robotics Club Rover 2017
 * Science Board Firmware
 *
 * soilprobe.c - Soil Probe Module
 * Author(s): Aaron Cohen
 */

#include <alloca.h>
#include <string.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "soilprobe.h"


#define CMD_ADDR_OFF 0
#define CMD_CMD_OFF 3
#define CMD_ARG_OFF 5
#define CMD_CRLF_OFF(type, argsize) ((type == CMD_TYPE_EXECUTE) ? 5 : \
									 (type == CMD_TYPE_QUERY) ? 7 : \
									 argsize + 1)

#define RESP_ADDR_OFF 0
#define RESP_DATA_OFF 3


static uint8_t _cmd_size(enum soilprobe_cmd_name cmd,
						 enum soilprobe_cmd_type type) {
	switch (cmd) {
	case CMD_NAME_SERIAL:
		if (type == CMD_TYPE_QUERY)
			return 9;
		else  // Invalid command type
			return 0;
	case CMD_NAME_FIRMWARE_VER:
		if (type == CMD_TYPE_QUERY)
			return 9;
		else  // Invalid command type
			return 0;
	case CMD_NAME_ADDR:
		if (type == CMD_TYPE_QUERY)
			return 9;
		else if (type == CMD_TYPE_ASSIGN)
			return 19;
		else  // Invalid command type
			return 0;
	case CMD_NAME_SOIL_TYPE:
		if (type == CMD_TYPE_QUERY || type == CMD_TYPE_ASSIGN)
			return 9;
		else  // Invalid command type
			return 0;
	case CMD_NAME_TAKE_READING:
		if (type == CMD_TYPE_EXECUTE)
			return 7;
		else  // Invalid command type
			return 0;
	case CMD_NAME_GET_READING:
		if (type == CMD_TYPE_EXECUTE)
			return 7;
		else  // Invalid command type
			return 0;
	default:  // Invalid command
		return 0;
	}
}


static uint8_t _max_resp_size(enum soilprobe_cmd_name cmd,
							  enum soilprobe_cmd_type type) {
	switch (cmd) {
	case CMD_NAME_SERIAL:
		if (type == CMD_TYPE_QUERY)
			return 13;
		else  // Invalid command type
			return 0;
	case CMD_NAME_FIRMWARE_VER:
		if (type == CMD_TYPE_QUERY)
			return 9;
		else  // Invalid command type
			return 0;
	case CMD_NAME_ADDR:
		if (type == CMD_TYPE_QUERY || type == CMD_TYPE_ASSIGN)
			return 5;
		else  // Invalid command type
			return 0;
	case CMD_NAME_SOIL_TYPE:
		if (type == CMD_TYPE_QUERY || type == CMD_TYPE_ASSIGN)
			return 6;
		else  // Invalid command type
			return 0;
	case CMD_NAME_TAKE_READING:
		if (type == CMD_TYPE_EXECUTE)
			return 1;  // No response
		else  // Invalid command type
			return 0;
	case CMD_NAME_GET_READING:
		if (type == CMD_TYPE_EXECUTE)
			return 105;
		else  // Invalid command type
			return 0;
	default:  // Invalid command
		return 0;
	}
}


static uint8_t * _cmd_str(enum soilprobe_cmd_name cmd, uint8_t reading_set) {
	switch (cmd) {
	case CMD_NAME_SERIAL:
		return (uint8_t *) "SN";
	case CMD_NAME_FIRMWARE_VER:
		return (uint8_t *) "FV";
	case CMD_NAME_ADDR:
		return (uint8_t *) "AD";
	case CMD_NAME_SOIL_TYPE:
		return (uint8_t *) "ST";
	case CMD_NAME_TAKE_READING:
		return (uint8_t *) "TR";
	case CMD_NAME_GET_READING:
		switch (reading_set) {
		case 0:
			return (uint8_t *) "T0";
		case 1:
			return (uint8_t *) "T1";
		case 2:
			return (uint8_t *) "T2";
		case 3:
			return (uint8_t *) "T3";
		case 4:
			return (uint8_t *) "T4";
		case 5:
			return (uint8_t *) "T5";
		default:  // Invalid reading set number
			return (uint8_t *) "T!";
		}
	default:  // Invalid command
		return (uint8_t *) "!!";
	}
}


void soilprobe_init(void) {
	// 9600 8N1
	uart_enable(SOILPROBE_UART, SOILPROBE_BAUD, 1, 0);

	// Also set up IC enable/disable line
	SOILPROBE_ENABLE_DDR |= _BV(SOILPROBE_ENABLE_PIN);
}


void soilprobe_cmd(struct soilprobe_cmd *cmd, struct soilprobe_resp *resp) {
	// Command size and maximum response size
	uint8_t cmdsize = _cmd_size(cmd->cmd, cmd->type);
	uint8_t maxrespsize = _max_resp_size(cmd->cmd, cmd->type);

	// Command buffers read/written by UART module
	uint8_t *cmdbuf = alloca(cmdsize);
	uint8_t *respbuf = alloca(maxrespsize);

	// Command name enum to string representation
	uint8_t *cmdstr = _cmd_str(cmd->cmd, cmd->reading_set);

	// Control variables for RX loop
	uint8_t respwritten = 0;
	uint8_t respsofar = 0;
	uint16_t timesincelast = 0;

	// Variable used to calculate response data size
	uint8_t *respend;

	// Copy command address
	memcpy(&(cmdbuf[CMD_ADDR_OFF]), cmd->addr, 3);

	// Copy command string
	memcpy(&(cmdbuf[CMD_CMD_OFF]), cmdstr, 2);

	// Copy command args
	if (cmd->type == CMD_TYPE_QUERY) {
		memcpy(&(cmdbuf[CMD_ARG_OFF]), "=?", 2);
	} else if (cmd->type == CMD_TYPE_ASSIGN) {
		cmdbuf[CMD_ARG_OFF] = '=';
		memcpy(&(cmdbuf[CMD_ARG_OFF + 1]), cmd->arg, cmd->argsize);
	}

	// Add CRLF to end of command
	memcpy(&(cmdbuf[CMD_CRLF_OFF(cmd->type, cmd->argsize)]), "\x0D\x0A", 2);

	// Enable RS485 Driver
	SOILPROBE_ENABLE_BANK |= _BV(SOILPROBE_ENABLE_PIN);

	// Transmit command
	uart_tx(SOILPROBE_UART, cmdbuf, cmdsize);
	while (uart_tx_in_progress(SOILPROBE_UART)) {
	}

	// No response for "Take Reading" command
	if (cmd->cmd == CMD_NAME_TAKE_READING)
		return;

	// Enable RS485 Receiver
	SOILPROBE_ENABLE_BANK ^= _BV(SOILPROBE_ENABLE_PIN);

	// Receive response
	// Loop until all bytes written or response timeout
	while (respsofar < maxrespsize && timesincelast < SOILPROBE_TIMEOUT_US) {
		respwritten = uart_rx(SOILPROBE_UART, &(respbuf[respsofar]),
							  maxrespsize - respsofar);

		if (respwritten == 0) {  // No bytes written since last loop
			timesincelast += 1;  // Increase timeout
		} else if (respsofar + respwritten <= maxrespsize) {  // Bytes written
			respsofar += respwritten;
			timesincelast = 0;
		} else {  // Total bytes written more than expected
			break;
		}

		// Check for CRLF packet ending (Useful for variable length "Transmit
		// Reading" packet type)
		if (respsofar >= 2
		 && strncmp((const char *) &(respbuf[respsofar - 2]), "\x0D\x0A", 2)) {
			break;
		}

		_delay_us(1);
	}

	// Parse response address
	memcpy(resp->addr, &(respbuf[RESP_ADDR_OFF]), 3);

	// Parse response body
	if (cmd->cmd != CMD_NAME_ADDR) {
		// Copy response data and simulataneously check for packet validity
		// by looking for CRLF
		respend = memccpy(resp->data, &(respbuf[RESP_DATA_OFF]), '\x0D',
						  maxrespsize - 3 + 1);

		if (respend != NULL) {  // CRLF was found, data is valid
			resp->datasize = respend - &(respbuf[RESP_DATA_OFF]);
		} else {  // No CRLF found, data invalid
			resp->datasize = 0;
		}
	} else {  // Empty body for get/set address commands
		resp->data[0] = '\0';
		resp->datasize = 0;
	}
}
