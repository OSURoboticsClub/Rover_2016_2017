/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * videoswitch.h - Camera multiplexer and FCB-EX1020 camera controller.
 */
#include <stdint.h>
#include <stdbool.h>

/* Set to true by the callsign sender when a callsign has been entered. */
extern bool CallsignSet;

/* This module needs to provide a function to select a camera input
 * with an 6:1 analog multiplexer. If CallsignSet is false, it
 * must actually select channel 0, and select the correct channel
 * only when CallsignSet becomes true.
 * It must also provide a function for issuing commands to a
 * Sony FCB-EX1020 camera. The actual commands will be provided
 * by the control software. */

/* Set the output of the video switch.
 * 0 = None
 * 1 = Input 1
 * 2 = Input 2
 * ...
 * 6 = Input 6 */
void videoswitch_select(uint8_t input);