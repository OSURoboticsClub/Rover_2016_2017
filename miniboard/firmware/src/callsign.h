/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * callsign.h - Video stream morse code callsign sender.
 */
#include <stdint.h>
#include <stdbool.h>

/* Set to true by the callsign sender when a callsign has been entered. */
extern bool CallsignSet;

/* To comply with FCC regulations, the FPV video transmitter system must
 * send the callsign of the licensed ham radio operator responsible for it.
 * One way to do this is via morse code audio tones in the NTSC signal's audio
 * stream.
 * This module must modulate an I/O pin to send the callsign. It should turn the
 * I/O pin off and on at a constant frequency (around 200Hz, for example) to produce
 * a tone, then turn this tone on and off to produce the dits and dahs of morse code.
 * 
 * I recommend using one of the 16-bit timers to call an interrupt at twice the
 * frequency of the tone. Each time the interrupt runs, it should determine if the given
 * character, morse symbol (dit, dah, dit/dah space, character space, word space), and
 * cycle within the symbol require the pin to high or low, then set the pin to that state.
 * 
 * Since the I/O pin that will be used hasn't been determined yet, make functions
 * such as pin_on() and pin_off() that can easily be changed later.
 * 
 * P.S. RAM is limited, FLASH is not. Look into the avr-libc progmem system. It should
 * be possible to implement this module with less than 20 bytes of RAM. */

/* Set the FPV callsign and start sending it.
 * This function will copy the callsign into module-specific memory. */
void callsign(uint8_t *callsign_str);