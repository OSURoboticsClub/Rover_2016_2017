/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * sabertooth.h - Sabertooth 2x12 packet serial control module.
 */
#include <stdint.h>

/* Set the speed of a sabertooth motor driver.
 * addr is the sabertooth address, from 0-7.
 * motor is motor 1 or motor 2. 
 * speed if the motor speed. 0 = stop, 127 = full forward, -127 = full reverse. */
void sabertooth(uint8_t addr, uint8_t motor, int8_t speed);