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

#include <stdio.h>
#include <string.h>

void camera_command_trigger(){
	
}

void debugging_info_trigger(){
	
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
	sei();
}

int main(void){
	init();
	while(1){
		uint8_t buf[5];
		uint16_t r = uart_rx(COMM_UART, buf, 5);
		if(r){
			uart_tx(COMM_UART, buf, r);
		}
	}
	
	while(1){
		uint8_t *str = (uint8_t *) "Hello 123 456 789          Testing A B C\n\r";
		uart_tx(COMM_UART, str, strlen((char *) str));
 		uart_tx(COMM_UART, str, strlen((char *) str));
 		uart_tx(COMM_UART, str, strlen((char *) str));
 		uart_tx(COMM_UART, str, strlen((char *) str));
 		uart_tx(COMM_UART, str, strlen((char *) str));
 		uart_tx(COMM_UART, str, strlen((char *) str));
		_delay_ms(400);
 		uart_tx(COMM_UART, str, strlen((char *) str));
 		DDRB |= _BV(PB7);
		PORTB ^= _BV(PB7);
	}
	return(0);
}
