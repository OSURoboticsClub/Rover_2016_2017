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

void trigger_camera_command(void){
	
}

/* Setup all peripherals and subsystems. */
void init(void){
	comm_init();
}

int main(void){
	init();
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
