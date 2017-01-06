/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * gps.h - Ublox Neo-7M GPS module interface.
 */
#include <stdint.h>

/* Start the GPS listener.
 * Once started, it will update the appropriate fields in the Data
 * struct as GPS data is received. */
void gps_init(void);