/* OSU Robotics Club Rover 2017
 * Science Board Firmware
 *
 * comm.c - Miniboard communications module
 * Author(s): Aaron Cohen
 */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include "soilprobe.h"
#include "crc.h"
#include "comm.h"

#define CMD_START_BYTE 0xF0
#define CMD_END_BYTE 0x0F

uint8_t cmd_buf[16];
uint8_t current;


/* Initialize the communication interface */
void comm_init(void)
{
	/* Enable SPI interrupt, SPI interface, and set Slave mode */
	SPCR0 = _BV(SPIE0) | _BV(SPE0);

	/* Set MISO output */
	DDRB |= _BV(PB4);

	current = 0;
}


/* Parse command packet from miniboard */
static void _comm_parse_packet(void)
{
	struct soilprobe_cmd cmd;
	struct soilprobe_resp resp;

	uint16_t resp_crc;
	uint8_t resp_buf[108];

	/* Temporarily disable SPI interrupt */
	SPCR0 &= ~_BV(SPIE0);

	/* Parse packet */
	cmd.cmd = cmd_buf[2];
	memcpy(cmd.addr, cmd_buf[3], 3);
	memcpy(cmd.arg, cmd_buf[6], cmd_buf[1] - 4);
	cmd.argsize = cmd_buf[1] - 4;

	/* Send command to soil probe */
	soilprobe_cmd(&cmd, &resp);

	/* Parse response */
	resp_buf[0] = 0xF0;
	resp_buf[1] = resp.datasize + 3;
	memcpy(resp_buf[2], resp.addr, 3);
	memcpy(resp_buf[5], resp.data, resp.datasize);

	resp_crc = calc_crc(&(resp_buf[2]), resp.datasize + 3);
	resp_buf[5 + resp.datasize] = (resp_crc >> 8) & 0xFF;
	resp_buf[6 + resp.datasize] = resp_crc & 0xFF;
	resp_buf[7 + resp.datasize] = 0x0F;

	/* Send response packet */
	for (uint8_t i = 0; i < resp.datasize + 8; i++)
	{
		SPDR = resp_buf[i];
		while (!(SPSR0 & _BV(SPIF)));
	}

	/* Re-enable SPI interrupt */
	SPCR0 |= _BV(SPIE0);
}


/* Interrupt for receiving a command packet from the miniboard */
ISR(SPI_STC_vect)
{
	cmd_buf[current] = SPDR;
	current += 1;

	if (current == 16)
	{
		uint8_t payload_length = cmd_buf[1];

		if (cmd_buf[0] == CMD_START_BYTE
		 && check_crc(&(cmd_buf[2]), payload_length, &(cmd_buf[2 + payload_length]))
		 && cmd_buf[2 + payload_length + 2] == CMD_END_BYTE)
		{
			_comm_parse_packet();
		}

		current = 0;
	}
}
