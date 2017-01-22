/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 * 
 * miniboard.c - Main control loop.
 * Author(s): Nick Ames
 */
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <util/atomic.h>
#include "uart.h"
#include "comm.h"
#include "commgen.h"
#include "adc.h"
#include "sabertooth.h"
#include "callsign.h"
#include "gps.h"
#include "ax12.h"
#include "videoswitch.h"
#include <stdio.h>
#include <string.h>

/* TODO: Watchdog timer. */
/* TODO: Debug facilities. */
/* TODO: Fix unnecessary memory consumption of variable-length registers. */

/* Triggers for data read commands. */
void camera_command_trigger(void){
	
}

void debugging_info_trigger(void){
	
}

void callsign_trigger(void){
	
}

void build_info_trigger(void){

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
	gps_init();
	//sabertooth_init();
	//set_callsign("asdf");
	sei();
}

/* Get a value, then atomically set the target variable. */
#define atomic_set(target, value) do {typeof(target) __valstore = value; ATOMIC_BLOCK(ATOMIC_RESTORESTATE){target = __valstore;} } while(0)

void miniboard_main(void){
	init();
	/* Miniboard main loop. */
	while(1){
		/* GPS */
		/* (handled in-module) */
		
		/* Saberteeth */
		while(uart_tx_in_progress(AX12_UART)){
			/* Wait for AX12 stuff to finish. */
		}
		sabertooth_init();
		if(0 == Data->pause_state){
			/* Paused */
			sabertooth_set_speed(0, 0, 0);
			sabertooth_set_speed(0, 1, 0);
			sabertooth_set_speed(1, 0, 0);
			sabertooth_set_speed(1, 1, 0);
			sabertooth_set_speed(2, 0, 0);
			sabertooth_set_speed(2, 1, 0);
			sabertooth_set_speed(3, 0, 0);
			sabertooth_set_speed(3, 1, 0);
			sabertooth_set_speed(4, 0, 0);
			sabertooth_set_speed(4, 1, 0);
			sabertooth_set_speed(5, 0, 0);
			sabertooth_set_speed(5, 1, 0);
		} else {
			/* Not Paused */
			sabertooth_set_speed(0, 0, Data->l_f_drive);
			sabertooth_set_speed(0, 1, Data->r_f_drive);
			sabertooth_set_speed(1, 0, Data->l_m_drive);
			sabertooth_set_speed(1, 1, Data->r_m_drive);
			sabertooth_set_speed(2, 0, Data->l_b_drive);
			sabertooth_set_speed(2, 1, Data->r_b_drive);
			if(1 == Data->swerve_state){
				/* Staight */
				sabertooth_set_speed(3, 0, 127);
			} else if(2 == Data->swerve_state){
				/* Turn */
				sabertooth_set_speed(3, 0, -127);
			} else {
				/* No motion */
				sabertooth_set_speed(3, 0, 0);
			}
			sabertooth_set_speed(3, 1, Data->arm_motor_1);
			sabertooth_set_speed(4, 0, Data->arm_motor_2);
			sabertooth_set_speed(4, 1, Data->arm_motor_3);
			sabertooth_set_speed(5, 0, Data->arm_motor_4);
			sabertooth_set_speed(5, 1, Data->arm_motor_5);
		}
		
		/* ADC (Pot channels and battery.) */
		atomic_set(Data->battery_voltage, battery_mV());
		atomic_set(Data->pot_1, pot_channel(1));
		atomic_set(Data->pot_2, pot_channel(2));
		atomic_set(Data->pot_3, pot_channel(3));
		atomic_set(Data->pot_4, pot_channel(4));
		atomic_set(Data->pot_5, pot_channel(5));
		
		/* Video Switch */
		//TODO: add callsign restriction
		videoswitch_select(Data->selected_camera);
		
		/* AX12 */
		while(uart_tx_in_progress(AX12_UART)){
			/* Wait for sabertooth stuff to finish. */
		}
		//TODO
		
		DDRB |= _BV(PB7);
		PORTB ^= _BV(PB7);
		_delay_ms(100);
	}
}

int main(void){
	/* For testing, remove the following call and insert your code below.
	 * You might need to copy stuff from init(). Don't commit your modified
	 * miniboard.c to the main branch! */
	miniboard_main();
	return(0);
}
