/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * compass.h - LIS3MDLTR external compass module.
 */
#include <stdint.h>

/* This module will need to communicate with an ST LIS3MDLTR over SPI
 * 3-axis magnetometer to determine the magnetic heading.
 * If nessecary, the current gravity vector can be supplied from the
 * IMU (if heading calculation depends on the tilt of the compass).
 * The compass will be on its own SPI bus. */

/* Get acceleration and magnetometer values from the IMU,
 * then use them to return the magnetic compass heading
 * of the rover. 
 * TODO: Units */
uint16_t compass_heading(void);