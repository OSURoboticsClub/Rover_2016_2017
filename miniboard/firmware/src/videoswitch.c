/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * videoswitch.h - Camera multiplexer and FCB-EX1020 camera controller.
 * Author(s): Blair Wilson
 */
#include <stdint.h>
#include <stdbool.h>
#include <videoswitch.h>
#include <avr/interrupt.h>

/* Set to true by the callsign sender when a callsign has been entered. */
extern bool CallsignSet;

/* Set the output of the video switch.
 * 0 = None
 * 1 = Input 1
 * 2 = Input 2
 * ...
 * 6 = Input 6 */
void videoswitch_select(uint8_t input){
	DDRH |= _BV(PH6);
	DDRB |= _BV(PB4) | _BV(PB5) | _BV(PB6);
	switch(input){
		case 1	:
			PORTH &= ~_BV(PH6);
			PORTB &= ~_BV(PB4);
			PORTB &= ~_BV(PB5);
			PORTB |= _BV(PB6);
			break;
		case 2	:
			PORTH |= _BV(PH6);
			PORTB &= ~_BV(PB4);
			PORTB &= ~_BV(PB5);
			PORTB |= _BV(PB6);
			break;
		case 3	:
			PORTH &= ~_BV(PH6);
			PORTB |= _BV(PB4);
			PORTB &= ~_BV(PB5);
			PORTB |= _BV(PB6);
			break;
		case 4	:
			PORTH |= _BV(PH6);
			PORTB |= _BV(PB4);
			PORTB &= ~_BV(PB5);
			PORTB |= _BV(PB6);
			break;
		case 5	:
			PORTH &= ~_BV(PH6);
			PORTB &= ~_BV(PB4);
			PORTB |= _BV(PB5);
			PORTB |= _BV(PB6);
			break;
		case 6	:
			PORTH |= _BV(PH6);
			PORTB &= ~_BV(PB4);
			PORTB |= _BV(PB5);
			PORTB |= _BV(PB6);
			break;
		default	:
			PORTH |= _BV(PH6);
			PORTB |= _BV(PB4);
			PORTB |= _BV(PB5);
			PORTB |= _BV(PB6);
			break;
	}
}