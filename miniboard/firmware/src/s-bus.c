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
#define SBUS_START_BYTE 0xF0
#define SBUS_END_BYTE 0x00
#define SBUS_NUM_CHANNELS 16

#define SBUS_FLAGS_INDEX 23
#define SBUS_FLAG_CH17 7
#define SBUS_FlAG_CH18 6
#define SBUS_FLAG_FRAME_LOST 5
#define SBUS_FLAG_FAILSAFE 4

void sbus_byte_handler(uint8_t b);
void (*UART3RXHandler)(uint8_t) = sbus_byte_handler;

void sbus_init(void) {
	/* Unconfirmed whether it's even or odd parity. Will have to verify. */
	/* Two stop bits + parity check */
	uart_enable(SBUS_UART, SBUS_BAUD, 2, 2);
}

void sbus_release(void) {
	uart_disable(SBUS_UART);
}

uint8_t packet_buffer[SBUS_PACKET_LENGTH];
uint8_t buffer_current = 0;

/* Handle S-BUS packets */
void sbus_handle_packet(void) {
	uint16_t servo_value;

	/* Verify packet integrity */
	if (packet_buffer[0]                      != SBUS_START_BYTE ||
		packet_buffer[SBUS_PACKET_LENGTH - 1] != SBUS_END_BYTE) {
		return;
	}

	/* If failsafe is active, don't set the servos */
	if (packet_buffer[SBUS_FLAGS_INDEX] & _BV(SBUS_FLAG_FAILSAFE)) {
		return;
	}

	/* Initialize UART to transmit commands to AX12 servos */
	sabertooth_release();
	ax12_init();

	/* Convert multiple bytes of the buffer at a time to numbers and bitmask
	 * them to isolate each channel */
	ax12_angle(0, *((uint16_t *) packet_buffer + 1) & 0xFFE0);
	ax12_angle(1, *((uint16_t *) packet_buffer + 2) & 0x1FFC);
	ax12_angle(2, *((uint32_t *) packet_buffer + 3) & 0x03FF8000);
	ax12_angle(3, *((uint16_t *) packet_buffer + 5) & 0x7FF0);
	ax12_angle(4, *((uint16_t *) packet_buffer + 6) & 0x0FFE);
	ax12_angle(5, *((uint32_t *) packet_buffer + 7) & 0x01FFC000);
	ax12_angle(6, *((uint16_t *) packet_buffer + 9) & 0x3FF8);
	ax12_angle(7, *((uint16_t *) packet_buffer + 10) & 0x07FF);
	ax12_angle(8, *((uint16_t *) packet_buffer + 12) & 0xFFE0);
	ax12_angle(9, *((uint16_t *) packet_buffer + 13) & 0x1FFC);
	ax12_angle(10, *((uint32_t *) packet_buffer + 14) & 0x03FF8000);
	ax12_angle(11, *((uint16_t *) packet_buffer + 16) & 0x7FF0);
	ax12_angle(12, *((uint16_t *) packet_buffer + 17) & 0x0FFE);
	ax12_angle(13, *((uint32_t *) packet_buffer + 18) & 0x01FFC000);
	ax12_angle(14, *((uint16_t *) packet_buffer + 20) & 0x3FF8);
	ax12_angle(15, *((uint16_t *) packet_buffer + 21) & 0x07FF);

	/* Transfer control of the UART back to the sabertooth module */
	ax12_release();
	sabertooth_init();
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
	} else {
		packet_buffer[buffer_current] = b;
		buffer_current += 1;
	}

	if (buffer_current == SBUS_PACKET_LENGTH){
		buffer_current = 0;
		sbus_handle_packet();
	}
}
