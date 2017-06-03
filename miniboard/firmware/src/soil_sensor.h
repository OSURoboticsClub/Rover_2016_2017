/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * soil_sensor.h - Soil sensor control module.
 */
#include <stdint.h>

/* Setup the soil sensor. */
void soil_init(void);

/* Using data in Data, send a line to the soil sensor and store its reply.
 * This function uses the UARTs of the GPS and motors.
 * This function blocks until data transfer is complete. */
void soil_talk(void);

/* Get soil data string, to be parsed by the base station.
 * After being called, this function will leave its result
 * in Data->soil_result* and set Data->soil_measure to 2. */
void soil_measure(void);

