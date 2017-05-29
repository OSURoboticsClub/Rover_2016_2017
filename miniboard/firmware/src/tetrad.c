/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * tetrad.h - Tetrad motor driver control module.
 * Author(s): Nick Ames, Aaron Cohen
 */
#include <stdint.h>
#include "uart.h"
#include "tetrad.h"
#include <avr/interrupt.h>

/* Macro for translating from local tetrad address (0-7) to global
 * tetrad address (128-135). */
#define TETRAD_ADDR(addr) (addr + 128)

/* Macro for calculating the tetrad serial packet checksum from the first
 * three packet bytes (address, command, data). */
#define TETRAD_CHECKSUM(packet) \
	((packet.addr + packet.dir + ((uint8_t)packet.m1) + ((uint8_t)packet.m2)) & 127)

/* Packed structure containing packet data to be sent to a tetrad motor
 * driver. */
struct tetrad_packet {
	uint8_t addr; /* Address of the motor driver, from 128-135 */
	uint8_t dir; /* Bit 1 = m1 dir, bit 2 = m2 dir */
	uint8_t m1; /* Motor 1 speed; 0-127 */
	uint8_t m2; /* Motor 2 speed; 0-127*/
	uint8_t checksum; /* Packet checksum */
} __attribute__((__packed__));

/* Compute a packet's checksum and send packetized serial information to a
 * tetrad motor driver via UART.
 *  packet is the packet to send, containing the address, command, and data. */
void tetrad_tx(struct tetrad_packet *packet){
	packet->checksum = TETRAD_CHECKSUM((*packet));
	uart_tx(TETRAD_UART, (uint8_t *) packet, 5);
}

void tetrad_init(void) {
	DDRC |= _BV(PC6);
	PORTC &= ~_BV(PC6);
	uart_enable(TETRAD_UART, TETRAD_BAUD, 1, 0);
}

void tetrad_release(void) {
	uart_disable(TETRAD_UART);
}

static uint8_t absclamp(int8_t val){
	if(val >= 0){
		return val;
	} else {
		if(val == -128){
			val = -127;
		}
		return -val;
	}
}

/* Set the speed of the tetrad motor driver.
 *  addr is the tetrad address, from 0-7.
 *  speed values range from -128 to 127. */
void tetrad_set_speed(uint8_t addr, int8_t m1_speed, int8_t m2_speed){
	struct tetrad_packet packet;
	packet.addr = TETRAD_ADDR(addr);
	packet.dir = ((m1_speed > 0) << 1) | ((m2_speed > 0) << 2);
	packet.m1 = absclamp(m1_speed);
	packet.m2 = absclamp(m2_speed);
	tetrad_tx(&packet);
}
