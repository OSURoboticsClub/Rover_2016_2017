/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * sbus.c - Futaba S-BUS receiver module.
 * Author(s) Aaron Cohen
 */

#include <stdint.h>
#include <string.h>
#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "sabertooth.h"
#include "uart.h"

#include "s-bus.h"

#define SBUS_PACKET_LENGTH 25
#define SBUS_START_BYTE 0x0F
#define SBUS_END_BYTE 0x00

#define SBUS_FLAGS_INDEX 23
#define SBUS_FLAG_FAILSAFE 4

#define SBUS_NUM_CHANNELS 16

void sbus_byte_handler(uint8_t b);
void (*UART3RXHandler)(uint8_t) = sbus_byte_handler;

uint8_t packet_buffer[SBUS_PACKET_LENGTH];
uint8_t buffer_current;

uint16_t sbus_channels[SBUS_NUM_CHANNELS];
uint8_t sbus_failsafe;

void sbus_init(void) {
	/* Two stop bits + even parity */
	uart_enable(SBUS_UART, SBUS_BAUD, 2, 2);

	/* Reset packet buffer */
	buffer_current = 0;

	/* Reset sbus channel values and set failsafe until first packet */
	memset(sbus_channels, 0, SBUS_NUM_CHANNELS);
	sbus_failsafe = 1;
}

void sbus_release(void) {
	uart_disable(SBUS_UART);
}

/* Extract an 11-bit, LSB-first, unsigned value from a bit stream.
 * stream is a uint8_t pointer to the stream of bytes.
 * bit_index is the bit number (starting at 0) of the first bit in
 * the value to be extracted. */
uint16_t extract_11b(uint8_t *stream, uint16_t bit_index){
	const uint16_t bil = bit_index;
	const uint16_t biu = bit_index + 10;
	if(((bil/8)+1) != ((biu)/8)){
		/* Value spans three bytes. */
		return 33;
	} else {
		/* Value spans two bytes. */
		uint16_t lbyte = stream[bil/8];
		uint16_t ubyte = stream[biu/8];
		uint16_t umask = ((1 << ((biu%8)+1))-1);
		ubyte &= umask;
		return (lbyte >> (bil%8)) | (ubyte << 8);
	}
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
		sbus_failsafe = 1;
		return;
	}

	sbus_failsafe = 0;

// 	sbus_channels[0] = packet_buffer[1] | (((uint16_t)packet_buffer[2]) & 0b0111)<<8;
	for(int i=0;i<16;i++){
		sbus_channels[i] = extract_11b(packet_buffer + 1, i * 11);
	}
	/* Convert multiple bytes of the buffer at a time to numbers and bitmask
	 * them to isolate each channel */
// 	sbus_channels[0] = (*((uint16_t *) packet_buffer + 1) & 0xFFE0) >> 5;
// 	sbus_channels[1] = (*((uint16_t *) packet_buffer + 2) & 0x1FFC) >> 2;
// 	sbus_channels[2] = (uint16_t) ((*((uint32_t *) packet_buffer + 3) & 0x03FF8000) >> 15);
// 	sbus_channels[3] = (*((uint16_t *) packet_buffer + 5) & 0x7FF0) >> 4;
// 	sbus_channels[4] = (*((uint16_t *) packet_buffer + 6) & 0x0FFE) >> 1;
// 	sbus_channels[5] = (uint16_t) ((*((uint32_t *) packet_buffer + 7) & 0x01FFC000) >> 14);
// 	sbus_channels[6] = (*((uint16_t *) packet_buffer + 9) & 0x3FF8) >> 3;
// 	sbus_channels[7] = *((uint16_t *) packet_buffer + 10) & 0x07FF;
// 	sbus_channels[8] = (*((uint16_t *) packet_buffer + 12) & 0xFFE0) >> 5;
// 	sbus_channels[9] = (*((uint16_t *) packet_buffer + 13) & 0x1FFC) >> 2;
// 	sbus_channels[10] = (uint16_t) ((*((uint32_t *) packet_buffer + 14) & 0x03FF8000) >> 15);
// 	sbus_channels[11] = (*((uint16_t *) packet_buffer + 16) & 0x7FF0) >> 4;
// 	sbus_channels[12] = (*((uint16_t *) packet_buffer + 17) & 0x0FFE) >> 1;
// 	sbus_channels[13] = (uint16_t) ((*((uint32_t *) packet_buffer + 18) & 0x01FFC000) >> 14);
// 	sbus_channels[14] = (*((uint16_t *) packet_buffer + 20) & 0x3FF8) >> 3;
// 	sbus_channels[15] = *((uint16_t *) packet_buffer + 21) & 0x07FF;
}

/* Recieve S-BUS protocol bytes as they come in over the UART. When a full
 * packet has been recieved, validate it and handle it. */
void sbus_byte_handler(uint8_t b){
	/* If the expected start and end bytes haven't been recieved, discard the
	 * packet. */
	if (buffer_current == 0 && b != SBUS_START_BYTE) {
		buffer_current = 0;
	} else if (buffer_current == (SBUS_PACKET_LENGTH - 1) && b != SBUS_END_BYTE) {
		//TODO
	//Arduino pin 7 = PH4
	DDRH |= _BV(PH4);
	PORTH |= _BV(PH4);
	_delay_us(1);
	PORTH &= ~_BV(PH4);
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
