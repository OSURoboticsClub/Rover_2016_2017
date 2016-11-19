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

/* Setup all peripherals and subsystems. */
void init(void){
	comm_init();
	//sabertooth_init();
	sei();
}

int main(void){
	//init();
	
	while(1){
		uint8_t *str = (uint8_t *) "Hello 123 456 789          Testing A B C";
//uart_tx(COMM_UART, str, strlen(str));
// 		uart_tx(COMM_UART, str, strlen(str));
// 		uart_tx(COMM_UART, str, strlen(str));
// 		uart_tx(COMM_UART, str, strlen(str));
// 		uart_tx(COMM_UART, str, strlen(str));
// 		uart_tx(COMM_UART, str, strlen(str));
		_delay_ms(500);
// 		uart_tx(COMM_UART, str, strlen(str));
		//UDR0 = 'H';
 		DDRB |= _BV(PB7);
		PORTB ^= _BV(PB7);
	}
	return(0);
}
