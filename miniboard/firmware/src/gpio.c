/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * gps.c - Ublox Neo-7M GPS module interface.
 * Author(s): Nick Ames
 */
#include "gpio.h"
#include <avr/interrupt.h>

/* Get GPIO state (high/low).
 * Returns a byte with bits 5, 4, 3, 2, 1 
 * set according to the state of their corresponding pins. */
uint8_t gpio_get_state(void){
	return ((PINK & _BV(PK3)) << 2) | ((PINF & _BV(PF6)) >> 2)
	       | ((PINK & _BV(PK6)) >> 3) | ((PINK & _BV(PK7)) >> 5)
		   | ((PINK & _BV(PK5)) >> 4);
}

/* Set GPIO in/out. 1 = out, 0 = in.
 * Takes a byte with bits 5, 4, 3, 2, 1 
 * set according to the state of their corresponding pins. */
void gpio_set_state(uint8_t out){
	DDRK |= (_BV(PK5) * !!(out & _BV(1))) | 
	        (_BV(PK7) * !!(out & _BV(2))) |
			(_BV(PK6) * !!(out & _BV(3))) |
			(_BV(PK3) * !!(out & _BV(5)));
    DDRK &= 0xC1 | 
            (_BV(PK5) * !!(out & _BV(1))) | 
	        (_BV(PK7) * !!(out & _BV(2))) |
			(_BV(PK6) * !!(out & _BV(3))) |
			(_BV(PK3) * !!(out & _BV(5)));
	DDRF |= (_BV(PF6) * !!(out & _BV(4)));
	DDRF &= ~(_BV(PK3) * !!(out & _BV(5)));
}

/* Set GPIO high/low state.
 * If a pin is set high while an input, its pull-up resistor
 * will be enabled. */
void gpio_set_out(uint8_t val){
	PORTK |= (_BV(PK5) * !!(val & _BV(1))) | 
	         (_BV(PK7) * !!(val & _BV(2))) |
			 (_BV(PK6) * !!(val & _BV(3))) |
			 (_BV(PK3) * !!(val & _BV(5)));
    PORTK &= 0xC1 | 
             (_BV(PK5) * !!(val & _BV(1))) | 
	         (_BV(PK7) * !!(val & _BV(2))) |
		 	 (_BV(PK6) * !!(val & _BV(3))) |
		  	 (_BV(PK3) * !!(val & _BV(5)));
	PORTF |= (_BV(PF6) * !!(val & _BV(4)));
	PORTF &= ~(_BV(PK3) * !!(val & _BV(5)));
}