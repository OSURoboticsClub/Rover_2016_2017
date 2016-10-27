/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * ax12.h - AX12 Servo module.
 */
#include <stdint.h>


/* This module needs to be able to control AX12 servos.
 * There should be a function to tell an AX12 to move to
 * a given position. If the AX12s have other features,
 * like load measurement or whatever, it would be nice to
 * have then too.
 * The AX12s might need to turn around the serial connection,
 * using the same line for transmission and receptions.
 * The serial module doesn't currently support this, but it
 * can be added. Read the docs and let me know what you need.
 * Use a #define to control the UART used. */