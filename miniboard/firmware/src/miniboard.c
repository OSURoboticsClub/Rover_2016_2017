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

#include <stdio.h>
#include <string.h>

void camera_command_trigger(void){
	
}

void debugging_info_trigger(void){
	
}

/* Setup all peripherals and subsystems. */
void init(void){
	//comm_init();
}

int main(void){
	init();
	uart_enable(0, 9600, 1, 0);
	
	while(1){
		DDRB |= _BV(PB7);
		
		PORTB |= _BV(PB7);
		uint16_t v = adc_voltage(0, ADC_REF_RATIOMETRIC);
		PORTB &= ~_BV(PB7);	
		
		char str[128];
		snprintf(str, 128, "ADC Reading: %d\r\n", v);
													  
		_delay_ms(500);
		uart_tx(0, str, strlen(str));
	}
	return(0);
}
