/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * soil_sensor.h - Soil sensor control module.
 */
#include <stdint.h>

/* Using data in Data, send a line to the soil sensor and store its reply.
 * This function uses the UARTs of the GPS and motors.
 * This function blocks until data transfer is complete. */
void talk_with_soil_sensor(void);