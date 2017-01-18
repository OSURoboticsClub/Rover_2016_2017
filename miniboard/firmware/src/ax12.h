/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * ax12.h - AX12 Servo module.
 * Author(s): Jarrod Hollis 
 */
#include <stdint.h>
#include <stdbool.h>

#define AX12_UART 1

/* Use to send command to all servos. */
#define AX12_ALL_BROADCAST_ID 0xFE

/* Use for ax12_set_alarm_led, ax12_set_alarm_shutdown. */
#define AX12_INPUT_VOLTAGE_ERROR 0b00000001
#define AX12_ANGLE_LIMIT_ERROR 0b00000010
#define AX12_OVERHEATING_ERROR 0b00000100
#define AX12_RANGE_ERROR 0b00001000
#define AX12_CHECKSUM_ERROR 0b00010000
#define AX12_OVERLOAD_ERROR 0b00100000
#define AX12_INSTRUCTION_ERROR 0b01000000

/* Use for ax12_status_return_level. */
enum AX12_RETURN_LEVEL {
	NO_RESPONSE = 0, // Never send status packets
	READ_ONLY = 1, // Only send status packets for read requests
	ALL_RESPONSE = 2 // Always send status packets
};

/* This module needs to be able to control AX12 servos.
 * There should be a function to tell an AX12 to move to
 * a given position. If the AX12s have other features,
 * like load measurement or whatever, it would be nice to
 * have then too.
 * The AX12s might need to turn around the serial connection,
 * using the same line for transmission and receptions.
 * The serial module doesn't currently support this, but it
 * can be added. Read the docs and let me know what you need.
 * Use a #define to control the UART used. */

/* Initialize the UART for the AX12s (and connect the AX12s with the switch).
 * Since the UART is shared between the AX12 and saberteeth, this function will
 * be called after using the saberteeth and before using the AX12s. */
void ax12_init(void);

/* Wait until no more data is being sent from the AX12 UART, then disable it. */
void ax12_release(void);

/* Change the broadcast ID of ax12 servo (range 0-253).
 * DO NOT SET MULTIPLE SERVOS TO SAME ID! */
void ax12_set_id(uint8_t prev_id, uint8_t new_id);

/* Change the baud rate (initially at 1000000, must adjust UART rate to match).
 * The formula for setting baud rate is BPS = 2000000 / (value + 1). */
void ax12_set_baud_rate(uint8_t servo_id, uint8_t value);

/* Set the delay time for status (return) packets.
 * The formula for setting return delay is t = 2µs * value. */
void ax12_set_return_delay(uint8_t servo_id, uint8_t value);

/* Set the operating angle limits.
 * The operating range is CW <= Goal Position <= CCW.
 * The angle formula is angle = (value / 0x3ff) * 300.
 * The minimum CW limit is 0, and the maximum CWW limit is 0x3ff. */
void ax12_set_operating_angle_limit(uint8_t servo_id, uint16_t cw, uint16_t ccw);

/* Set the maximum temperature limit.
 * Values are in degrees Celsius.
 * If temperature is ever higher than limit, an over heating error will return. */
void ax12_set_temperature_limit(uint8_t servo_id, uint8_t temp);

/* Set the operating voltage limits.
 * The formula for setting voltage limits is voltage = value / 10.
 * If voltage is ever outside limits, a voltage range error will return. */
void ax12_set_voltage_limit(uint8_t servo_id, uint8_t low, uint8_t high);

/* Set the maximum torque limit.
 * The limit is a fraction of maximum torque (see p.3 of documentation).
 * Fraction is limit / 0x3ff.
 * If torque exceeds limit, an overload error will return. */
void ax12_set_torque_limit(uint8_t servo_id, uint16_t limit);

/* Set the protocol for status packets.
 * NO_RESPONSE: Never send status packets, even for read requests.
 * READ_ONLY: Only send status packets for read requests.
 * ALL_RESPONSE: Send status packets after any instruction. */
void ax12_status_return_level(uint8_t servo_id, enum AX12_RETURN_LEVEL level);

/* Set the flags for errors that will cause the LED to blink.
 * Flags can be masked together to allow LED to blink for multiple errors. */
void ax12_set_alarm_led(uint8_t servo_id, uint8_t flags);

/* Set the flags for errors that will cause the servo to stop.
 * Flags can be masked together to allow servo to stop for multiple errors.
 * If servo stops, ax12_enable must be called to reenable servo. */
void ax12_set_alarm_shutdown(uint8_t servo_id, uint8_t flags);

/* Enable servo movement.
 * Must be called after shutdown from alarm or call to ax12_disable. */
void ax12_enable(uint8_t servo_id);

/* Disable servo movement.
 * Must be followed by a call to ax12_enable to reenable movement. */
void ax12_disable(uint8_t servo_id);

/* Toggle the servo LED. */
void ax12_toggle_led(uint8_t servo_id, bool toggle);

/* Set the goal position.
 * Must be within operating angle limit, or an angle limit error will return.
 * The angle formula is angle = (value / 0x3ff) * 300.
 * The maximum possible value is 0x3ff. */
void ax12_set_goal_position(uint8_t servo_id, uint16_t value);

/* Set the moving speed. */
void ax12_set_moving_speed(uint8_t servo_id, uint16_t speed);

/* Lock the servo settings.
 * Once locked, the servo can only be unlocked by disconnecting power.
 * Functions available after locking:
 *     ax12_enable, ax12_toggle_led, ax12_set_goal_position,
 *     ax12_set_moving_speed */
void ax12_lock(uint8_t servo_id);
