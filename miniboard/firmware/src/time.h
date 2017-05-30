/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * time.h - Millisecond timekeeping.
 */

/* Start the timer used for timekeeping. */
void time_init(void);

/* Return the current time value (milliseconds since the rover was
 * powered on). */
uint32_t get_ms(void);