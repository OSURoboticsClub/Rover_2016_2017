/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * tetrad.h - Tetrad motor driver control module.
 * Author(s): Nick Ames
 */
#ifndef TETRAD_H
#define TETRAD_H

#include <stdint.h>

/* Set the UART that will be used to communicate with the motor drivers */
#define TETRAD_UART 1
#define TETRAD_BAUD 115200
/* Initialize tetrad communication by enabling the tetrad UART */
void tetrad_init(void);

/* Release tetrad UART */
void tetrad_release(void);

/* Set the speed of the tetrad motor driver.
 *  addr is the tetrad address, from 0-7.
 *  speed values range from -128 to 127. */
void tetrad_set_speed(uint8_t addr, int8_t motor1_speed, int8_t motor2_speed);

#endif 
