/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * ax12.h - AX12 Servo module.
 * Author(s): Jarrod Hollis
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

#define AX12_TORQUE_DISABLE 0x00
#define AX12_TORQUE_ENABLE 0x01
#define AX12_LOCK 0x01

struct ax12_packet {
	uint8_t buffer[15];
	uint8_t length;
} __attribute__((__packed__));

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

/* Construct and send a write packet from supplied data. */
static void ax12_write_packet(uint8_t servo_id, uint8_t *data, uint8_t length) {
	struct ax12_packet packet;
	packet.length = 6 + length;
	packet.buffer[0] = 0xff;
	packet.buffer[1] = 0xff;
	packet.buffer[2] = servo_id;
	packet.buffer[3] = length + 2;
	packet.buffer[4] = AX12_INSTRUCTION_WRITE;
	for (uint8_t i = 0; i < length; i++) {
		packet.buffer[5 + i] = data[i];
	}
	uint8_t checksum = 0;
	for (uint8_t i = 2; i < length + 5; i++) {
		checksum += packet.buffer[i];
	}
	packet.buffer[5 + length] = ~checksum;
	uart_tx(AX12_UART, (uint8_t *) &packet, packet.length);
}

/* Initialize the UART for the AX12s (and connect the AX12s with the switch).
 * Since the UART is shared between the AX12 and saberteeth, this function will
 * be called after using the saberteeth and before using the AX12s. */
void ax12_init(void) {
	uart_enable(AX12_UART, 1000000, 1, 0);
	ax12_switch_tx();
	ax12_status_return_level(AX12_ALL_BROADCAST_ID, NO_RESPONSE);
	ax12_enable(AX12_ALL_BROADCAST_ID);
}

/* Wait until no more data is being sent from the AX12 UART,
 * then disable it. */
void ax12_release(void) {
	while(uart_tx_in_progress(AX12_UART)){
		/* Wait for send to finish. */
	}
	uart_disable(AX12_UART);
}

/* Reset servo settings to factory defaults */
void ax12_reset(uint8_t servo_id) {
	struct ax12_packet packet;
	packet.length = 6;
	packet.buffer[0] = 0xff;
	packet.buffer[1] = 0xff;
	packet.buffer[2] = servo_id;
	packet.buffer[3] = 0x02;
	packet.buffer[4] = AX12_INSTRUCTION_RESET;
	packet.buffer[5] = ~(packet.buffer[2] + packet.buffer[3] + packet.buffer[4]);
	uart_tx(AX12_UART, (uint8_t *) &packet, packet.length);
}

/* Change the broadcast ID of ax12 servo (range 0-253).
 * DO NOT SET MULTIPLE SERVOS TO SAME ID! */
void ax12_set_id(uint8_t prev_id, uint8_t new_id) {
	uint8_t data[] = {0x03, new_id};
	ax12_write_packet(prev_id, data, 2);
}

/* Change the baud rate (initially at 1000000, must adjust UART rate to match).
 * The formula for setting baud rate is BPS = 2000000 / (value + 1). */
void ax12_set_baud_rate(uint8_t servo_id, uint8_t value) {
	uint8_t data[] = {0x04, value};
	ax12_write_packet(servo_id, data, 2);
}

/* Set the delay time for status (return) packets.
 * The formula for setting return delay is t = 2µs * value. */
void ax12_set_return_delay(uint8_t servo_id, uint8_t value) {
	uint8_t data[] = {0x05, value};
	ax12_write_packet(servo_id, data, 2);
}

/* Set the operating angle limits.
 * The operating range is CW <= Goal Position <= CCW.
 * The angle formula is angle = (value / 0x3ff) * 300.
 * The minimum CW limit is 0, and the maximum CWW limit is 0x3ff. */
void ax12_set_operating_angle_limit(uint8_t servo_id, uint16_t cw, uint16_t ccw) {
	uint8_t data_cw[] = {0x06, (uint8_t) cw, (uint8_t) (cw >> 8)};
	uint8_t data_ccw[] = {0x08, (uint8_t) ccw, (uint8_t) (ccw >> 8)};
	ax12_write_packet(servo_id, data_cw, 3);
	ax12_write_packet(servo_id, data_ccw, 3);
}

/* Set the maximum temperature limit.
 * Values are in degrees Celsius.
 * If temperature is ever higher than limit, an over heating error will return. */
void ax12_set_temperature_limit(uint8_t servo_id, uint8_t temp) {
	uint8_t data[] = {0x0B, temp};
	ax12_write_packet(servo_id, data, 2);
}

/* Set the operating voltage limits.
 * The formula for setting voltage limits is voltage = value / 10.
 * If voltage is ever outside limits, a voltage range error will return. */
void ax12_set_voltage_limit(uint8_t servo_id, uint8_t low, uint8_t high) {
	uint8_t data_low[] = {0x0C, high};
	uint8_t data_high[] = {0x0D, high};
	ax12_write_packet(servo_id, data_low, 2);
	ax12_write_packet(servo_id, data_high, 2);
}

/* Set the maximum torque limit.
 * The limit is a fraction of maximum torque (see p.3 of documentation).
 * Fraction is limit / 0x3ff.
 * If torque exceeds limit, an overload error will return. */
void ax12_set_torque_limit(uint8_t servo_id, uint16_t limit) {
	uint8_t data[] = {0x0E, (uint8_t) limit, (uint8_t) (limit >> 8)};
	ax12_write_packet(servo_id, data, 3);
}

/* Set the protocol for status packets.
 * NO_RESPONSE: Never send status packets, even for read requests.
 * READ_ONLY: Only send status packets for read requests.
 * ALL_RESPONSE: Send status packets after any instruction. */
void ax12_status_return_level(uint8_t servo_id, enum AX12_RETURN_LEVEL level) {
	uint8_t data[] = {0x10, (uint8_t) level};
	ax12_write_packet(servo_id, data, 2);
}

/* Set the flags for errors that will cause the LED to blink.
 * Flags can be masked together to allow LED to blink for multiple errors. */
void ax12_set_alarm_led(uint8_t servo_id, uint8_t flags) {
	uint8_t data[] = {0x11, flags};
	ax12_write_packet(servo_id, data, 2);
}

/* Set the flags for errors that will cause the servo to stop.
 * Flags can be masked together to allow servo to stop for multiple errors.
 * If servo stops, ax12_enable must be called to reenable servo. */
void ax12_set_alarm_shutdown(uint8_t servo_id, uint8_t flags) {
	uint8_t data[] = {0x12, flags};
	ax12_write_packet(servo_id, data, 2);
}

/* Enables servo movement.
 * Must be called after shutdown from alarm or call to ax12_disable. */
void ax12_enable(uint8_t servo_id) {
	uint8_t data[] = {0x18, AX12_TORQUE_ENABLE};
	ax12_write_packet(servo_id, data, 2);
}

/* Disable servo movement.
 * Must be followed by a call to ax12_enable to reenable movement. */
void ax12_disable(uint8_t servo_id) {
	uint8_t data[] = {0x18, AX12_TORQUE_DISABLE};
	ax12_write_packet(servo_id, data, 2);
}

/* Toggle the servo LED */
void ax12_toggle_led(uint8_t servo_id, bool toggle) {
	uint8_t data[] = {0x19, toggle};
	ax12_write_packet(servo_id, data, 2);
}

/* Set the goal position.
 * Must be within operating angle limit, or an angle limit error will return.
 * The angle formula is angle = (value / 0x3ff) * 300.
 * The maximum possible value is 0x3ff. */
void ax12_set_goal_position(uint8_t servo_id, uint16_t angle) {
	uint8_t data[] = {0x1e, (uint8_t) angle, (uint8_t) (angle >> 8)};
	ax12_write_packet(servo_id, data, 3);
}

/* Set the moving speed */
void ax12_set_moving_speed(uint8_t servo_id, uint16_t speed) {
	uint8_t data[] = {0x20, (uint8_t) speed, (uint8_t) (speed >> 8)};
	ax12_write_packet(servo_id, data, 3);
}

/* Lock the servo settings.
 * Once locked, the servo can only be unlocked by disconnecting power.
 * Functions available after locking:
 *     ax12_enable, ax12_toggle_led, ax12_set_goal_position,
 *     ax12_set_moving_speed */
void ax12_lock(uint8_t servo_id) {
	uint8_t data[] = {0x2f, AX12_LOCK};
	ax12_write_packet(servo_id, data, 2);
}
