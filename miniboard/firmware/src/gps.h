/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * gps.h - Ublox Neo-7M GPS module interface.
 */
#include <stdint.h>

/* This module needs to parse NMEA strings received
 * from a Ublox Neo-7M (part of https://hobbyking.com/en_us/ublox-neo-7m-gps-with-compass-and-pedestal-mount.html)
 * GPS module over UART.
 * It should have a start function that initializes the UART and takes pointers to variables
 * to write the data into. Then, whenever a byte arrives from the module, an interrupt should run
 * to parse the NMEA string. */