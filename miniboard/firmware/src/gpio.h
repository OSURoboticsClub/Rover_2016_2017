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

/* Get GPIO state (high/low).
 * Returns a byte with bits 5, 4, 3, 2, 1 
 * set according to the state of their corresponding pins. */
uint8_t gpio_get_state(void);

/* Set GPIO in/out. 1 = out, 0 = in.
 * Takes a byte with bits 5, 4, 3, 2, 1 
 * set according to the state of their corresponding pins. */
void gpio_set_out(uint8_t out);

/* Set GPIO high/low state.
 * If a pin is set high while an input, its pull-up resistor
 * will be enabled. */
void gpio_set_vale(uint8_t val);