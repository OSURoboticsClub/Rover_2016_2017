/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * sabertooth.c - Sabertooth 2x12 packet serial control module.
 * Author(s) Aaron Cohen
 */
#include <stdint.h>
#include "uart.h"
#include "sabertooth.h"
#include <avr/interrupt.h>

/* Macro for translating from local sabertooth address (0-7) to global
 * sabertooth address (128-135). */
#define SABERTOOTH_ADDR(addr) (addr + 128)

/* Macro for calculating the sabertooth serial packet checksum from the first
 * three packet bytes (address, command, data). */
#define SABERTOOTH_CHECKSUM(packet) \
	((packet.addr + packet.cmd + packet.data) & 127)

/* Packed structure containing packet data to be sent to a sabertooth motor
 * driver. */
struct sabertooth_packet {
	uint8_t addr; /* Address of the motor driver, from 128-135 */
	uint8_t cmd; /* Command byte, from 0-17 */
	uint8_t data; /* Data byte to accompany the command byte */
	uint8_t checksum; /* Packet checksum */
} __attribute__((__packed__));

/* Compute a packet's checksum and send packetized serial information to a
 * sabertooth motor driver via UART.
 *  packet is the packet to send, containing the address, command, and data. */
void sabertooth_tx(struct sabertooth_packet *packet) {
	packet->checksum = SABERTOOTH_CHECKSUM((*packet));

	uart_tx(SABERTOOTH_UART, (uint8_t *) packet, 4);
}

void sabertooth_init(void) {
	DDRC |= _BV(PC6);
	PORTC &= ~_BV(PC6);
	uart_enable(SABERTOOTH_UART, 9600, 1, 0);
}

void sabertooth_release(void) {
	uart_disable(SABERTOOTH_UART);
}

void sabertooth_set_speed(uint8_t addr, uint8_t motor, int8_t speed) {
	struct sabertooth_packet packet;

	packet.addr = SABERTOOTH_ADDR(addr);
	packet.cmd = 0  | (motor << 2)  /* Choose motor command */
					| (speed < 0); /* Choose direction command */
	packet.data = ((packet.cmd & 1) == 0) ?
					(uint8_t) speed       : /* Forward */
					(uint8_t) (-1 * speed); /* Backwards */

	sabertooth_tx(&packet);
}

void sabertooth_set_vmin(uint8_t addr, uint8_t vmin) {
	struct sabertooth_packet packet;

	packet.addr = SABERTOOTH_ADDR(addr);
	packet.cmd = 2;
	packet.data = vmin;

	sabertooth_tx(&packet);
}

void sabertooth_set_vmax(uint8_t addr, uint8_t vmax) {
	struct sabertooth_packet packet;

	packet.addr = SABERTOOTH_ADDR(addr);
	packet.cmd = 3;
	packet.data = vmax;

	sabertooth_tx(&packet);
}

void sabertooth_set_timeout(uint8_t addr, uint8_t timeout) {
	struct sabertooth_packet packet;

	packet.addr = SABERTOOTH_ADDR(addr);
	packet.cmd = 14;
	packet.data = timeout;

	sabertooth_tx(&packet);
}

void sabertooth_set_baud(uint8_t addr, uint8_t rate) {
	struct sabertooth_packet packet;

	packet.addr = SABERTOOTH_ADDR(addr);
	packet.cmd = 15;
	packet.data = rate;

	sabertooth_tx(&packet);
}

void sabertooth_set_ramping(uint8_t addr, uint8_t ramp) {
	struct sabertooth_packet packet;

	packet.addr = SABERTOOTH_ADDR(addr);
	packet.cmd = 16;
	packet.data = ramp;

	sabertooth_tx(&packet);
}

void sabertooth_set_deadband(uint8_t addr, uint8_t deadband) {
	struct sabertooth_packet packet;

	packet.addr = SABERTOOTH_ADDR(addr);
	packet.cmd = 17;
	packet.data = deadband;

	sabertooth_tx(&packet);
}
