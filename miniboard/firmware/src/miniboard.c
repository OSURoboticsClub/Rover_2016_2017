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
#include "tetrad.h"
#include "callsign.h"
#include "gps.h"
#include "compass.h"
#include "ax12.h"
#include "s-bus.h"
#include "videoswitch.h"
#include "imu.h"
#include "gpio.h"
#include "soil_sensor.h"
#include "uart.h"
#include <stdio.h>
#include <string.h>

/* AX12 limits */
#define PAN_MIN 50
#define PAN_MAX 1000
#define TILT_MIN 100
#define TILT_MAX 450
#define PANTILT_DIV 16

/* AX12 addresses */
#define PAN_AX12 1
#define TILT_AX12 2
#define PITCH_AX12 4
#define WRIST_AX12 5
#define SQUEEZE_AX12 6
#define CFLEX1_AX12 7
#define CFLEX2_AX12 8
#define CSEAL_AX12 9


/* Triggers for data read commands. */
void camera_command_trigger(void){
	
}

void callsign_trigger(void){
	
}

void soil_sensor_send_trigger(void){
	
}

void soil_sensor_recv_trigger(void){
	
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

#define COMM_TIMEOUT 2 /* If this time (in seconds) elapses without a
                        * message from the computer, the connection is
                        * considered to be lost. In that case, S-BUS control
                        * may be enabled, or the rover may be paused.
                        * The maximum timeout is 4. */
                        
static bool CommTimedOut; /* If true, the connection to the computer has been lost. */

/* Start a countdown timer for COMM_TIMEOUT seconds.
 * Once it expires, CommTimedOut will be set to true */
void reset_timeout_timer(void){
	TCCR4B = 0;
	TCNT4 = 0;
	OCR4A = (uint16_t) 15625*COMM_TIMEOUT;
	TIMSK4 = _BV(OCIE4A);
	TCCR4B = _BV(CS42) | _BV(CS40); /* Set 1024 prescaler. */
	CommTimedOut = false;
}

ISR(TIMER4_COMPA_vect){
	CommTimedOut = true;
	TCCR4B = 0;
}

/* Setup all peripherals and subsystems. */
void init(void){
	comm_init();
	gps_init();
	comp_init();
	imu_init();
	sbus_init();
	soil_init();
	sei();
	reset_timeout_timer();
	ax12_init(AX12_BAUD);
	ax12_set_angle_mode(PAN_AX12);
	ax12_set_angle_mode(TILT_AX12);
	ax12_set_continuous_mode(PITCH_AX12);
	ax12_set_continuous_mode(WRIST_AX12);
	ax12_set_continuous_mode(SQUEEZE_AX12);
	ax12_set_angle_mode(CFLEX1_AX12);
	ax12_set_angle_mode(CFLEX2_AX12);
	ax12_set_continuous_mode(CSEAL_AX12);
}

/* Get a value, then atomically set the target variable. */
#define atomic_set(target, value) do {typeof(target) __valstore = value; ATOMIC_BLOCK(ATOMIC_RESTORESTATE){target = __valstore;} } while(0)

/* When inverting a int8_t, -128 would become 128, which is bad. This
 * function clamps an input to [127, 127]. */
uint8_t clamp127(int8_t value){
	if(value == -128){
		return -127;
	} else {
		return value;
	}
}
	
void miniboard_main(void){
	init();
	
	bool super_pause;
	/* Miniboard main loop. */
	while(1){
		super_pause = !Data->pause_state || (CommTimedOut && !Data->sbus_active);
		/* GPS */
		/* (handled in-module) */
		
		/* Saberteeth */
		uart_wait(AX12_UART);
		tetrad_init();
		if(super_pause){
			/* Paused */
			tetrad_set_speed(0, 0, 0);
			tetrad_set_speed(1, 0, 0);
			tetrad_set_speed(2, 0, 0);
			tetrad_set_speed(3, 0, 0);
			tetrad_set_speed(4, 0, 0);
			tetrad_set_speed(5, 0, 0);
		} else {
			/* Not Paused */
			tetrad_set_speed(0, -clamp127(Data->l_f_drive), Data->r_m_drive);
			tetrad_set_speed(1, -clamp127(Data->l_m_drive), Data->r_f_drive);
			tetrad_set_speed(2, -clamp127(Data->l_b_drive), Data->r_b_drive);
			int8_t swerve_speed;
			if(1 == Data->swerve_state){
				/* Staight */
				swerve_speed = -127;
			} else if(2 == Data->swerve_state){
				/* Turn */
				swerve_speed = 127;
			} else {
				/* No motion */
				swerve_speed = 0;
			}
			tetrad_set_speed(3, swerve_speed, -clamp127(Data->arm_motor_1));
			tetrad_set_speed(4, Data->arm_motor_2, -clamp127(Data->arm_motor_3));
			tetrad_set_speed(5, Data->arm_motor_4, Data->arm_motor_5);
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
		uart_wait(AX12_UART);
		ax12_init(AX12_BAUD);
		if(super_pause) {
			ax12_disable(AX12_ALL_BROADCAST_ID);
		} else {
			ax12_enable(AX12_ALL_BROADCAST_ID);
			int16_t new_pan, new_tilt;
			new_pan = Data->pan_angle + Data->pan_speed/PANTILT_DIV;
			new_tilt = Data->tilt_angle + Data->tilt_speed/PANTILT_DIV;
			if(new_pan < PAN_MIN){
				new_pan = PAN_MIN;
			}
			if(new_pan > PAN_MAX){
				new_pan = PAN_MAX;
			}
			if(new_tilt < TILT_MIN){
				new_tilt = TILT_MIN;
			}
			if(new_tilt > TILT_MAX){
				new_tilt = TILT_MAX;
			}
			Data->pan_angle = new_pan;
			Data->tilt_angle = new_tilt;
			ax12_set_goal_position(Data->ax12_addr, Data->ax12_angle);
			ax12_set_goal_position(PAN_AX12, Data->pan_angle);
			ax12_set_goal_position(TILT_AX12, Data->tilt_angle);
			if(Data->arm_mode != 0){
				ax12_continuous_speed(PITCH_AX12, Data->ee_speed);
				if(Data->arm_mode == 1){
					/* Grabber */
					ax12_continuous_speed(WRIST_AX12, Data->grabber_rotation_speed);
					ax12_continuous_speed(SQUEEZE_AX12, Data->grabber_speed);
				} else if(Data->arm_mode == 2){
					/* Science */
					ax12_set_goal_position(CFLEX1_AX12, Data->cflex1_angle);
					ax12_set_goal_position(CFLEX2_AX12, Data->cflex1_angle);
					ax12_continuous_speed(CSEAL_AX12, Data->clid_speed);
				}
			}

		}

		/* S-Bus */
		/* Handled in module. */
		
		/* Compass */
		compass_retrieve();
		
		/* IMU */
 		imu_accel(&Data->accel_x, &Data->accel_y, &Data->accel_z);
		imu_gyro(&Data->gyro_x, &Data->gyro_y, &Data->gyro_z);
		
		/* GPIO */
		Data->gpio_state = gpio_get_state();
		
		/* Sample Camera Actions */
		sample_cam_button(Data->cam_action);
		
		/* Blink LED. */
		DDRB |= _BV(PB7);
		PORTB ^= _BV(PB7);
	}
}

// void copy_txd3(void){
// 	/* A0 = PF0 = non-inverting */
// 	/* A1 = PF1 = inverting */
// 	/* TXD3 = PJ1 */
// 	PORTF |= _BV(PF0);
// 	PORTF &= ~_BV(PF1);
// 	DDRF |= _BV(PF1) | _BV(PF0);
// 	_delay_ms(1);
// 	while(uart_tx_in_progress(3)){
// 		if(PINJ & _BV(PJ1)){
// 			PORTF |= _BV(PF0);
// 			PORTF &= ~_BV(PF1);
// 		} else {
// 			PORTF &= ~_BV(PF0);
// 			PORTF |= _BV(PF1);
// 		}
// 	}
// 	for(int i=10000;i>0;i--){
// 		if(PINJ & _BV(PJ1)){
// 			PORTF |= _BV(PF0);
// 			PORTF &= ~_BV(PF1);
// 		} else {
// 			PORTF &= ~_BV(PF0);
// 			PORTF |= _BV(PF1);
// 		}
// 	}
// 	DDRF &= ~(_BV(PF1) | _BV(PF0));
// 	PORTF &= ~(_BV(PF1) | _BV(PF0));
// }
// 
// 
// void soil_sensor_test(void){
// 	uart_enable(0, 9600, 1, 0);
// 	uart_enable(3, 9600, 1, 0);
// 	sei();
// 	char str[256];
// 	snprintf(str, 256, "Hello\r\n");
// 	uart_tx(0, str, strlen(str));
// 	snprintf(str, 256, "///FV=?\r\n");
// 	uart_tx(3, str, strlen(str));
// 	copy_txd3();
// 	while(1){
// 		char c;
// 		if(uart_rx(3, &c, 1)){
// 			uart_tx(0, &c, 1);
// 		}
// 	}
// 	
// 	while(1);
// }
// 
void ax12_test(void){
	init();
	uint8_t target_addr = 4;
	while(1){
		ax12_init(1000000);
		ax12_enable(AX12_ALL_BROADCAST_ID);
		ax12_toggle_led(AX12_ALL_BROADCAST_ID, 1);
		ax12_set_id(AX12_ALL_BROADCAST_ID, target_addr);
		ax12_set_baud_rate(AX12_ALL_BROADCAST_ID, 9);
		_delay_ms(300);
		DDRB |= _BV(PB7);
 		PORTB ^= _BV(PB7);
	}
}

void ax12_reset_addr(void){
	uint8_t target_addr = 4;
	init();
	while(1){
		for(uint16_t i=0;i<255;i++){
			uint32_t speed = 2000000UL / (i + 1);
			for(uint8_t i=0;i<3;i++){
				ax12_init(speed);
				ax12_toggle_led(AX12_ALL_BROADCAST_ID, 1);
				ax12_set_id(AX12_ALL_BROADCAST_ID, target_addr);
				ax12_set_baud_rate(AX12_ALL_BROADCAST_ID, 9);
				uart_wait(AX12_UART);
				_delay_ms(4);
				DDRB |= _BV(PB7);
				PORTB ^= _BV(PB7);
			}
		}
	}
}

// void ax12_test2(){
// 	ax12_init();
// 	while(1){
// 		/* Put an AX12 into continuous rotation mode. */
// 		ax12_set_continuous_mode(5);
// 		ax12_continuous_speed(5, 0);
// 		_delay_ms(100);
// 		DDRB |= _BV(PB7);
// 		PORTB ^= _BV(PB7);
// 	}
// }

int main(void){
	/* For testing, remove the following call and insert your code below.
	 * You might need to copy stuff from init(). Don't commit your modified
	 * miniboard.c to the main branch! */
	miniboard_main();
	//soil_sensor_test();
	//ax12_test();
	//ax12_reset_addr();
	return(0);
}
