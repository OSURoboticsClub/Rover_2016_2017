/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * tetrad.h - Tetrad motor driver control module.
 * Author(s): Nick Ames
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
	((packet.addr + ((uint8_t)packet.m1) + ((uint8_t)packet.m2)) & 127)

/* Packed structure containing packet data to be sent to a tetrad motor
 * driver. */
struct tetrad_packet {
	uint8_t addr; /* Address of the motor driver, from 128-135 */
	int8_t m1;
	int8_t m2; 
	uint8_t checksum; /* Packet checksum */
} __attribute__((__packed__));

/* Compute a packet's checksum and send packetized serial information to a
 * tetrad motor driver via UART.
 *  packet is the packet to send, containing the address, command, and data. */
void tetrad_tx(struct tetrad_packet *packet) {
	packet->checksum = TETRAD_CHECKSUM((*packet));
	uart_tx(TETRAD_UART, (uint8_t *) packet, 4);
}

void tetrad_init(void) {
	DDRC |= _BV(PC6);
	PORTC &= ~_BV(PC6);
	uart_enable(TETRAD_UART, 9600, 1, 0);
}

void tetrad_release(void) {
	uart_disable(TETRAD_UART);
}

/* Set the speed of the tetrad motor driver.
 *  addr is the tetrad address, from 0-7.
 *  speed values range from -128 to 127. */
void tetrad_set_speed(uint8_t addr, int8_t motor1_speed, int8_t motor2_speed){
	struct tetrad_packet packet;
	packet.addr = TETRAD_ADDR(addr);
	packet.m1 = motor1_speed;
	packet.m2 = motor2_speed;
	tetrad_tx(&packet);
}
