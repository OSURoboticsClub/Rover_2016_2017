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
#include <string.h>
#include <stdlib.h>
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

/* Get send a command to the soil sensor. The reply will be stored in Data->soil_recv_data*. */
static void soil_send(const char *cmd){
	strncpy(Data->soil_send_data, cmd, 197);
	Data->soil_send_data_length = strnlen(cmd, 197);
	soil_talk();
}

/* Parse the soil measurement. */
void soil_parse(void){
	//Data->soil_recv_data[Data->soil_recv_data_length] = 0;
	uint16_t i = 0;
	char buf[300];
	strncpy(buf, Data->soil_recv_data, Data->soil_recv_data_length);
	char *token;
	for (token = strtok(buf + 3, ","); token; token = strtok(NULL, ",")){
		if(i == 0){
			Data->temperature = atof(token) * 1000.0;
		}
		if(i == 2){
			Data->moisture = atof(token) * 1000.0;
		}
		if(i == 4){
			Data->salinity = atof(token) * 6.4 * 1000.0;
			break;
		}
		i++;
	}
}

/* Get soil data string, to be parsed by the base station.
 * After being called, this function will leave its result
 * in Data->soil_result* and set Data->soil_measure to 2. */
void soil_measure(void){
	soil_send("marPE=1\r\n");
	_delay_ms(1000);
	soil_send("marTR\r\n");
	_delay_ms(1000);
	soil_send("marT3\r\n");
	soil_parse();
	Data->soil_measure = 2;
}