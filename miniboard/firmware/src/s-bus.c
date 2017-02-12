/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * sbus.c - Futaba S-BUS receiver module.
 * Author(s) Aaron Cohen
 */

#include <stdint.h>
#include <avr/io.h>
#include "ax12.h"
#include "sabertooth.h"
#include "uart.h"

#include "s-bus.h"

#define SBUS_PACKET_LENGTH 25
#define SBUS_START_BYTE 0x0F
#define SBUS_END_BYTE 0xFF

#define SBUS_FLAGS_INDEX 23
#define SBUS_FLAG_FAILSAFE 4

void sbus_byte_handler(uint8_t b);
void (*UART3RXHandler)(uint8_t) = sbus_byte_handler;

uint8_t packet_buffer[SBUS_PACKET_LENGTH];
uint8_t buffer_current;

void sbus_init(void) {
	/* Two stop bits + even parity */
	uart_enable(SBUS_UART, SBUS_BAUD, 2, 2);

	/* Reset packet buffer */
	buffer_current = 0;
}

void sbus_release(void) {
	uart_disable(SBUS_UART);
}

/* Handle S-BUS packets */
void sbus_handle_packet(void) {
	/* Verify packet integrity */
	if (packet_buffer[0]                      != SBUS_START_BYTE ||
		packet_buffer[SBUS_PACKET_LENGTH - 1] != SBUS_END_BYTE) {
		return;
	}

	/* If failsafe is active, don't set the servos */
	if (packet_buffer[SBUS_FLAGS_INDEX] & _BV(SBUS_FLAG_FAILSAFE)) {
		ax12_disable(AX12_ALL_BROADCAST_ID);
		return;
	}

	/* Initialize UART to transmit commands to AX12 servos */
	ax12_init();
	ax12_enable(AX12_ALL_BROADCAST_ID);

	/* Convert multiple bytes of the buffer at a time to numbers and bitmask
	 * them to isolate each channel */
	ax12_set_goal_position(0, (*((uint16_t *) packet_buffer + 1) & 0xFFE0) >> 5);
	ax12_set_goal_position(1, (*((uint16_t *) packet_buffer + 2) & 0x1FFC) >> 2);
	ax12_set_goal_position(2, (uint16_t) ((*((uint32_t *) packet_buffer + 3) & 0x03FF8000) >> 15));
	ax12_set_goal_position(3, (*((uint16_t *) packet_buffer + 5) & 0x7FF0) >> 4);
	ax12_set_goal_position(4, (*((uint16_t *) packet_buffer + 6) & 0x0FFE) >> 1);
	ax12_set_goal_position(5, (uint16_t) ((*((uint32_t *) packet_buffer + 7) & 0x01FFC000) >> 14));
	ax12_set_goal_position(6, (*((uint16_t *) packet_buffer + 9) & 0x3FF8) >> 3);
	ax12_set_goal_position(7, *((uint16_t *) packet_buffer + 10) & 0x07FF);
	ax12_set_goal_position(8, (*((uint16_t *) packet_buffer + 12) & 0xFFE0) >> 5);
	ax12_set_goal_position(9, (*((uint16_t *) packet_buffer + 13) & 0x1FFC) >> 2);
	ax12_set_goal_position(10, (uint16_t) ((*((uint32_t *) packet_buffer + 14) & 0x03FF8000) >> 15));
	ax12_set_goal_position(11, (*((uint16_t *) packet_buffer + 16) & 0x7FF0) >> 4);
	ax12_set_goal_position(12, (*((uint16_t *) packet_buffer + 17) & 0x0FFE) >> 1);
	ax12_set_goal_position(13, (uint16_t) ((*((uint32_t *) packet_buffer + 18) & 0x01FFC000) >> 14));
	ax12_set_goal_position(14, (*((uint16_t *) packet_buffer + 20) & 0x3FF8) >> 3);
	ax12_set_goal_position(15, *((uint16_t *) packet_buffer + 21) & 0x07FF);
}

/* Recieve S-BUS protocol bytes as they come in over the UART. When a full
 * packet has been recieved, validate it and handle it. */
void sbus_byte_handler(uint8_t b) {
	/* If the expected start and end bytes haven't been recieved, discard the
	 * packet. */
	if (buffer_current == 0 && b != SBUS_START_BYTE) {
		buffer_current = 0;
	} else if (buffer_current == SBUS_PACKET_LENGTH - 1 && b != SBUS_END_BYTE) {
		buffer_current = 0;
	} else { /* Data byte is okay */
		packet_buffer[buffer_current] = b;
		buffer_current += 1;
	}

	/* If a full packet has been received, reset buffer_current and call the
	 * packet handling function. */
	if (buffer_current >= SBUS_PACKET_LENGTH){
		buffer_current = 0;
		sbus_handle_packet();
	}
}
