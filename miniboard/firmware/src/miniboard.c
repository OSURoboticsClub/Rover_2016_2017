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
#include "comm.h"
#include "commgen.h"
#include "adc.h"
#include "sabertooth.h"
#include "callsign.h"

#include <stdio.h>
#include <string.h>

void camera_command_trigger(void){
	
}

void debugging_info_trigger(void){
	
}

void callsign_trigger(void){
	
}


/* ISR that fires if an interrupt is enabled without a corresponding handler. */
ISR(BADISR_vect){
	DDRB |= _BV(PB7);
	while(1){
		PORTB |= _BV(PB7);
		_delay_ms(100);
		PORTB &= ~_BV(PB7);
		_delay_ms(200);
		PORTB |= _BV(PB7);
		_delay_ms(300);
		PORTB &= ~_BV(PB7);
		_delay_ms(300);
	}
}

/* Setup all peripherals and subsystems. */
void init(void){
	comm_init();
	//sabertooth_init();
	//set_callsign("asdf");
	sei();
}

void miniboard_main(void){
	init();
	while(1){
		/* Miniboard main loop. */
		DDRB |= _BV(PB7);
		PORTB ^= _BV(PB7);
		_delay_ms(300);
	}
}

int main(void){
	/* For testing, remove the following call and insert your code below.
	 * You might need to copy stuff from init(). Don't commit your modified
	 * miniboard.c to the main branch! */
	miniboard_main();
	return(0);
}
