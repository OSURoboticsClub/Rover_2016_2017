/* OSU Robotics Club Rover 2017
 * Science Board Firmware
 *
 * comm.c - Miniboard communications module
 * Author(s): Aaron Cohen
 */

#include <stdint.h>
#include <string.h>
#include <avr/interrupt.h>
#include "soilprobe.h"
#include "comm.h"

#define CMD_START_BYTE 0x01

uint8_t cmd_buf[15];
uint8_t current;


/* Initialize the communication interface */
void comm_init(void)
{
	/* Enable SPI interrupt, SPI interface, and set Slave mode */
	SPCR = _BV(SPIE) | _BV(SPE);

	/* Set MISO output */
	DDRB |= _BV(PB4);

	current = 0;
	SPDR = 0;
}


/* Parse command packet from miniboard */
static void _comm_parse_packet(void)
{
	struct soilprobe_cmd cmd;
	struct soilprobe_resp resp;

	uint8_t resp_buf[105];

	/* Temporarily disable SPI interrupt */
	SPCR &= ~_BV(SPIE);

	/* Parse packet */
	cmd.cmd = cmd_buf[2];
	memcpy(cmd.addr, &(cmd_buf[3]), 3);
	memcpy(cmd.arg, &(cmd_buf[6]), cmd_buf[1] - 4);
	cmd.argsize = cmd_buf[1] - 4;

	/* Send command to soil probe */
	soilprobe_send_cmd(&cmd, &resp);

	/* Parse response */
	resp_buf[0] = 0xF0;
	resp_buf[1] = resp.datasize + 3;
	memcpy(&(resp_buf[2]), resp.addr, 3);
	memcpy(&(resp_buf[5]), resp.data, resp.datasize);

	/* Send response packet */
	for (uint8_t i = 0; i < resp.datasize + 5; i++)
	{
		SPDR = resp_buf[i];
		while (!(SPSR & _BV(SPIF)));
	}

	/* Re-enable SPI interrupt */
	SPCR |= _BV(SPIE);
}


/* Interrupt for receiving a command packet from the miniboard */
ISR(SPI_STC_vect)
{
	cmd_buf[current] = SPDR;
	current += 1;
	SPDR = 0;

	/* If the packet is valid */
	if (current >= 2 && cmd_buf[0] == CMD_START_BYTE && current == cmd_buf[1] + 2)
	{
		_comm_parse_packet();
		current = 0;
	}

	/* If the packet is longer than the max length, it is invalid */
	if (current > 17)
	{
		current = 0;
	}
}
