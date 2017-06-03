/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * soil_sensor.h - Soil sensor control module.
 */
#include "soil_sensor.h"
#include <avr/interrupt.h>
#define F_CPU 16000000
#include <util/delay.h>
#include "uart.h"
#include "commgen.h"
#include "tetrad.h"

#define SOIL_TX_UART 2
#define SOIL_RX_UART 1
#define TIMEOUT_ITERATION 250000UL

/* Drive enable = pin 25 = PA3 */
#define DEN_PIN 3
#define DEN_PORT PORTA
#define DEN_DDR DDRA

/* Receive enable = pin 23 = PA1 */
#define REN_PIN 1
#define REN_PORT PORTA
#define REN_DDR DDRA

/* Put the RS-485 transceiver into send mode. */
static void rs485_send(void){
	REN_DDR |= _BV(REN_PIN);
	DEN_DDR |= _BV(DEN_PIN);
	REN_PORT |= _BV(REN_PIN);
	DEN_PORT |= _BV(DEN_PIN);
}

/* Put the RS-485 transceiver into receive mode. */
static void rs485_recv(void){
	REN_DDR |= _BV(REN_PIN);
	DEN_DDR |= _BV(DEN_PIN);
	REN_PORT &= ~_BV(REN_PIN);
	DEN_PORT &= ~_BV(DEN_PIN);
}

/* Put the RS-485 transceiver into idle mode. */
static void rs485_idle(void){
	REN_DDR |= _BV(REN_PIN);
	DEN_DDR |= _BV(DEN_PIN);
	REN_PORT |= _BV(REN_PIN);
	DEN_PORT &= ~_BV(DEN_PIN);
}

/* Setup the soil sensor. */
void soil_init(void){
	rs485_idle();
}

/* Using data in Data, send a line to the soil sensor and store its reply.
 * This function uses the UARTs of the GPS and motors.
 * This function blocks until data transfer is complete. */
void soil_talk(void){
	/* Put the motor uart in tetrad mode, then reconfigure its baud rate. */
	uart_wait(SOIL_RX_UART);
	tetrad_init();
	uart_enable(SOIL_RX_UART, 9600, 1, 0);
	/* The GPS TX UART is already set for 9600 baud. */
	rs485_send();
	_delay_ms(3);
 	uart_tx(SOIL_TX_UART, (uint8_t *) Data->soil_send_data, Data->soil_send_data_length);
 	uart_wait(SOIL_TX_UART);
	rs485_recv();
	uint8_t count = 0;
	uint32_t iterations = 0;
	while(count < 200 && iterations < TIMEOUT_ITERATION){
		count += uart_rx(SOIL_RX_UART, (uint8_t *) Data->soil_recv_data + count, 200-count);
		if(count > 0 && Data->soil_recv_data[count-1] == '\n'){
			break;
		}
		iterations++;
	}
	Data->soil_recv_data_length = count;
	rs485_idle();
}