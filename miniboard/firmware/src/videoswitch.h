/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * videoswitch.h - Camera multiplexer and FCB-EX1020 camera controller.
 */
#include <stdint.h>
#include <stdbool.h>

/* Set to true by the callsign sender when a callsign has been entered. */
extern bool CallsignSet;

/* Set the output of the video switch.
 * 0 = None
 * 1 = Input 1
 * 2 = Input 2
 * ...
 * 6 = Input 6 */
void videoswitch_select(uint8_t input);

/* Press a button on the sample camera to perform an action.
 * Action values:
 *   0 - no button pressed
 *   1 - Shutter
 *   2 - Focus
 *   3 - Zoom in
 *   4 - Zoom out */
void sample_cam_button(uint8_t action);