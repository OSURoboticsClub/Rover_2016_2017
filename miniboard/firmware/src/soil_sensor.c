/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * soil_sensor.h - Soil sensor control module.
 */
#include <soil_sensor.h>

#define SOIL_TX_UART 2
#define SOIL_RX_UART 1

/* Using data in Data, send a line to the soil sensor and store its reply.
 * This function uses the UARTs of the GPS and motors.
 * This function blocks until data transfer is complete. */
void talk_with_soil_sensor(void);