/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * ax12.h - AX12 Servo module.
 * Author(s): 
 */
#include "ax12.h"
#include "uart.h"
#include <avr/interrupt.h>

#define AX12_UART 1

#define AX12_INSTRUCTION_PING 0x01
#define AX12_INSTRUCTION_READ 0x02
#define AX12_INSTRUCTION_WRITE 0x03
#define AX12_INSTRUCTION_DELAY_WRITE 0x04
#define AX12_INSTRUCTION_TRIGGER 0x05 //writes delayed command
#define AX12_INSTRUCTION_RESET 0x06
#define AX12_INSTRUCTION_MULTI_WRITE 0x07

struct ax12_packet {
	uint8_t buffer[16];
	uint8_t length;
};

/* Set the analog switch to transmit on the AX12 line. */
static void ax12_switch_tx(void) {
	DDRC |= _BV(PC6);
	PORTC |= _BV(PC6);
}

/* Set the analog switch to receive on the AX12 line. */
static void ax12_switch_rx(void) {
	DDRC |= _BV(PC6);
	PORTC &= ~_BV(PC6);
}

/* Construct and send a write packet from supplied data */
static void ax12_write_packet(uint8_t servo_id, uint8_t *data, uint8_t length) {
	struct ax12_packet packet;
	packet.length = 6 + length;
	packet.buffer[0] = 0xff;
	packet.buffer[1] = 0xff;
	packet.buffer[2] = servo_id;
	packet.buffer[3] = length + 2;
	packet.buffer[4] = AX12_INSTRUCTION_WRITE;
	for (uint8_t i = 0; i < length, i++) {
		packet.buffer[5 + i] = data[i];
	}
	uint8_t checksum = 0;
	for (uint8_t i = 0; i < length + 5) {
		checksum += packet.buffer[i];
	}
	packet.buffer[5 + length] = ~checksum;
	uart_tx(AX12_UART, &packet, packet.length);
}

/* Disable return status packets */
static void ax12_disable_status_packets(void) {
	uint8_t data[] = {0x10, 0x02};
	ax12_write_packet(AX12_ALL_BROADCAST_ID, &data, 2);
}

/* Toggle torque enable flag */
static void ax12_toggle_torque_enable(uint8_t servo_id, bool toggle) {
	uint8_t data[] = {0x18, toggle};
	ax12_write_packet(servo_id, &data, 2);
}

/* Initialize the UART for the AX12s (and connect the AX12s with the
 * switch).
 * Since the UART is shared between the AX12 and saberteeth, this
 * function will be called after using the saberteeth and before using
 * the AX12s. */
void ax12_init(void) {
	uart_enable(AX12_UART, 1000000, 1, 0);
	ax12_switch_tx();
	ax12_disable_status_packets();
}

/* Change the broadcast ID of ax12 servo (range 0-253) */
void ax12_set_id(uint8_t prev_id, uint8_t new_id) {
	uint8_t data[] = {0x03, new_id};
	ax12_write_packet(prev_id, &data, 2);
}

/* Toggle the servo LED */
void ax12_toggle_led(uint8_t servo_id, bool toggle) {
	uint8_t data[] = {0x19, toggle};
	ax12_write_packet(servo_id, &data, 2);
}

/* Set the goal position */
void ax12_set_goal_position(uint8_t servo_id, uint16_t angle) {
	uint8_t data[] = {0x1e, (uint8_t) angle, (uint8_t) (angle >> 8)};
	ax12_write_packet(servo_id, &data, 3);
}

/* Set the moving speed */
void ax12_set_moving_speed(uint8_t servo_id, uint16_t speed) {
	uint8_t data[] = {0x20, (uint8_t) speed, (uint8_t) (speed >> 8)};
	ax12_write_packet(servo_id, &data, 3);
}

/* Wait until no more data is being sent from the AX12 UART,
 * then disable it. */
void ax12_release(void) {
	while(uart_tx_in_progress(AX12_UART)){
		/* Wait for send to finish. */
	}
	uart_disable(AX12_UART);
}
	
