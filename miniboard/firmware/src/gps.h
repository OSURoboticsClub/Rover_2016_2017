/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * gps.h - Telit SE880GPSR22R003 GPS module interface.
 */
#include <stdint.h>

/* This module needs to retrieve NMEA strings
 * from the GPS over SPI, parse them, then set
 * variables to the GPS readings.
 * This should occur at ~5Hz using a timer.
 * The SPI API and timer selection are TBD;
 * read the datasheets and work on an NMEA parser first. */