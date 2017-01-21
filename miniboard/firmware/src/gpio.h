/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * gpio.h - General-purpose I/O pin control module.S
 */
#include <stdint.h>

/* Get the state of each miniboard I/O pin.
 * If low, the corresponding variable will be set to 0.
 * If high, it will be set to 1.
 * Variables will be set atomically. */
