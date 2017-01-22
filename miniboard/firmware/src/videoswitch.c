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
	switch(input){
		
		case 1	:
			PORTH &= ~_BV(PH6);
			PORTB &= ~_BV(PB4);
			PORTB &= ~_BV(PB5);
			PORTB |= _BV(PB6);
			
		case 2	:
			PORTH |= _BV(PH6);
			PORTB &= ~_BV(PB4);
			PORTB &= ~_BV(PB5);
			PORTB |= _BV(PB6);
			
		case 3	:
			PORTH &= ~_BV(PH6);
			PORTB |= _BV(PB4);
			PORTB &= ~_BV(PB5);
			PORTB |= _BV(PB6);
			
		case 4	:
			PORTH |= _BV(PH6);
			PORTB |= _BV(PB4);
			PORTB &= ~_BV(PB5);
			PORTB |= _BV(PB6);
			
		case 5	:
			PORTH &= ~_BV(PH6);
			PORTB &= ~_BV(PB4);
			PORTB |= _BV(PB5);
			PORTB |= _BV(PB6);
			
		case 6	:
			PORTH |= _BV(PH6);
			PORTB &= ~_BV(PB4);
			PORTB |= _BV(PB5);
			PORTB |= _BV(PB6);
			
		default	:
			PORTH |= _BV(PH6);
			PORTB |= _BV(PB4);
			PORTB |= _BV(PB5);
			PORTB |= _BV(PB6);
	}
}