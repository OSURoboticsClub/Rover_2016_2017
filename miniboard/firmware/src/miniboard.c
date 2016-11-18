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

/* Setup all peripherals and subsystems. */
void init(void){
	comm_init();
	//sabertooth_init();
}

int main(void){
	init();
	
	while(1){
		uint8_t *str = (uint8_t *) "Hello 123 456 789          Testing A B C";
// 		uart_tx(COMM_UART, str, strlen(str));
// 		uart_tx(COMM_UART, str, strlen(str));
// 		uart_tx(COMM_UART, str, strlen(str));
// 		uart_tx(COMM_UART, str, strlen(str));
// 		uart_tx(COMM_UART, str, strlen(str));
// 		uart_tx(COMM_UART, str, strlen(str));
		_delay_ms(100);
// 		uart_tx(COMM_UART, str, strlen(str));
		UDR0 = 'H';
// 		DDRB |= _BV(PB7);
// 
// 		PORTB |= _BV(PB7);
// 		uint16_t v = adc_voltage(0, ADC_REF_RATIOMETRIC);
// 		PORTB &= ~_BV(PB7);
// 
// 		char str[128];
// 		snprintf(str, 128, "ADC Reading: %d\r\n", v);
// 
// 		_delay_ms(500);
// 		uart_tx(0, str, strlen(str));
// 		_delay_ms(500);
// 		sabertooth_set_speed(0, 0, -20);
// 		_delay_ms(1000);
// 		sabertooth_set_speed(0, 0, 20);
	}
	return(0);
}
