/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * ax12.h - AX12 Servo module.
 */
#include <stdint.h>
#include <stdbool.h>

#define AX12_UART 1
#define AX12_ALL_BROADCAST_ID 0xFE

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

/* Initialize the UART for the AX12s (and connect the AX12s with the
 * switch).
 * Since the UART is shared between the AX12 and saberteeth, this
 * function will be called after using the saberteeth and before using
 * the AX12s. */
void ax12_init(void);

/* Change the broadcast ID of ax12 servo (range 0-253) */
void ax12_set_id(uint8_t prev_id, uint8_t new_id);

/* Toggle the servo LED */
void ax12_toggle_led(uint8_t servo_id, bool toggle);

/* Set the goal position */
void ax12_set_goal_position(uint8_t servo_id, uint16_t angle);

/* Set the moving speed */
void ax12_set_moving_speed(uint8_t servo_id, uint16_t speed);

/* Wait until no more data is being sent from the AX12 UART,
 * then disable it. */
void ax12_release(void);
