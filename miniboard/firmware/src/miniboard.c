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
	
	uart_enable(0, 9600, 1, 0);
	
	while(1){
		DDRB |= _BV(PB7);
		PORTB |= _BV(PB7);
		_delay_ms(200);
		PORTB &= ~_BV(PB7);
		_delay_ms(200);
		uart_tx(0, "Hello\n\r", 7);
	}
	return(0);
}
