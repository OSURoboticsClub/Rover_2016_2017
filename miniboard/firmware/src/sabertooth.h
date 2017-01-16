/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * sabertooth.h - Sabertooth 2x12 packet serial control module.
 * Author(s) Aaron Cohen
 */
#ifndef SABERTOOTH_H
#define SABERTOOTH_H

#include <stdint.h>

/* Set the UART that will be used to communicate with the motor drivers */
#define SABERTOOTH_UART 1

/* Initialize sabertooth communication by enabling the sabertooth UART */
void sabertooth_init(void);

/* Release sabertooth UART */
void sabertooth_release(void);

/* Set the speed of the sabertooth motor driver.
 *  addr is the sabertooth address, from 0-7.
 *  motor is a value that specifies which motor to set.
 *        A value of 0 corresponds to motor 1.
 *        A value of 1 corresponds to motor 2.
 *  speed is a value that represents the new motor speed.
 *        A value of 0 corresponds to full stop.
 *        A value of 127 corresponds to full forward.
 *        A value of -127 corresponds to full reverse. */
void sabertooth_set_speed(uint8_t addr, uint8_t motor, int8_t speed);

/* Set the minimum voltage threshold for the sabertooth before the output shuts
 * down. This setting resets every time the sabertooth is powercycled.
 *  addr is the sabertooth address, from 0-7.
 *  vmin is a value that represents the desired minimum voltage, from 0-120.
 *       The conversion from vmin to voltage is vmin=(voltage-6)*5. */
void sabertooth_set_vmin(uint8_t addr, uint8_t vmin);

/* Set the maximum allowed voltage during regenerative braking. If the motors
 * are running off of a rechargable battery, setting a custom vmax is not
 * recommended. This setting resets every time the sabertooth is powercycled.
 *  addr is the sabertooth address, from 0-7.
 *  vmax is a value that represents the desired maximum allowed voltage.
 *       The conversion from vmax to voltage is vmax=voltage*5.12. */
void sabertooth_set_vmax(uint8_t addr, uint8_t vmax);

/* Set the sabertooth timeout duration. If this duration passes after a command
 * is received and no further commands are issued then the sabertooth will shut
 * off. This setting resets every time the sabertooth is powercycled.
 *  addr is the sabertooth address, from 0-7.
 *  timeout is a value that represents the desired timeout duration.
 *          One unit of timeout is equal to 100 ms. */
void sabertooth_set_timeout(uint8_t addr, uint8_t timeout);

/* Set the baud rate for serial communication with the sabertooth. This setting
 * persists even if the sabertooth is powercycled.
 *  addr is the sabertooth address, from 0-7.
 *  rate is a value that represents the desired baud rate.
 *       A value of 1 corresponds to 2400 baud.
 *       A value of 2 corresponds to 9600 baud. (Factory default)
 *       A value of 3 corresponds to 19200 baud.
 *       A value of 4 corresponds to 38400 baud. */
void sabertooth_set_baud(uint8_t addr, uint8_t rate);

/* Set the sabertooth's ramping. Ramping sets the time elapsed between full
 * forward velocity and full reverse velocity.
 *  addr is the sabertooth address, from 0-7.
 *  ramp is the value used to set the ramping.
 *       Values of ramp between 1 and 10 correspond to Fast Ramp.
 *       Values of ramp between 11 and 20 correspond to Slow Ramp.
 *       Values of ramp between 21 and 80 correspond to Intermediate Ramp.
 *       Consult the datasheet for ramp value to time value conversions. */
void sabertooth_set_ramping(uint8_t addr, uint8_t ramp);

/* Set the sabertooth's deadband. The deadband is the range of values that
 * the sabertooth will interpret as "zero". This setting persists even if the
 * sabertooth is powercycled.
 *  addr is the sabertooth address, from 0-7.
 *  deadband is the offset of values from 0 that the sabertooth will interpret
 *           as 0 (Defaults to 3). */
void sabertooth_set_deadband(uint8_t addr, uint8_t deadband);

#endif /* SABERTOOTH_H */
