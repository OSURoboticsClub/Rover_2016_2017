/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 * 
 * miniboard.c - Main control loop.
 * Author(s): Nick Ames
 */
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "uart.h"

int main(void){
	while(1){
		DDRB |= _BV(PB7);
		PORTB |= _BV(PB7);
		_delay_ms(200);
		PORTB &= ~_BV(PB7);
		_delay_ms(200);
	}
	return(0);
}
