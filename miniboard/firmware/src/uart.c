/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 * 
 * uart.h - UART interface module.
 * This module provides support for UARTs 0-3.
 * When calling functions, give the uart as a
 * number from 0-3.
 * Author(s)? Nick Ames
 */
#include <avr/interrupt.h>
#include "uart.h"
#include <stdlib.h>
#include "comm.h"

void __attribute__((weak)) (*UART0RXHandler)(uint8_t) = NULL;
void __attribute__((weak)) (*UART1RXHandler)(uint8_t) = NULL;
void __attribute__((weak)) (*UART2RXHandler)(uint8_t) = NULL;
void __attribute__((weak)) (*UART3RXHandler)(uint8_t) = NULL;

#define UART_TX_BUFFER 150 /* Size of per-UART transmission buffer.
                            * When trying to send a chunk of data larger than this,
	                        * the uart_tx() function will block until transmission is complete.
	                        * Must not be greater than 255. */
#define UART_RX_BUFFER 10 /* Size of per-UART reception buffer.
                           * If the buffer fills up, incoming data will be discarded.
                           * Must not be greater than 255. */

/* UART Circular Buffers.
 * Start points to the next character to be sent, or the first character
 * received. End points one past the last to be sent or most recently
 * received. If Start == End, the buffer is empty. */
// typedef struct {
// 	uint8_t *data;
// 	uint8_t start;
// 	uint8_t end;
// 	uint8_t capacity;
// } circbuf_t;
// static uint8_t URBuf[4][UART_RX_BUFFER];
// static uint8_t UTBuf[4][UART_TX_BUFFER];

/* These macros evaluate to the specific uart control registers. */
#define UDRn(uart)   *((uart==0)?   &UDR0 : ((uart==1)?   &UDR1 : ((uart==2)?   &UDR2 : ((uart==3)?   &UDR3 : 0)))) /* TODO? error handling: */
#define UCSRnA(uart) *((uart==0)? &UCSR0A : ((uart==1)? &UCSR1A : ((uart==2)? &UCSR2A : ((uart==3)? &UCSR3A : 0)))) /* TODO? error handling: */
#define UCSRnB(uart) *((uart==0)? &UCSR0B : ((uart==1)? &UCSR1B : ((uart==2)? &UCSR2B : ((uart==3)? &UCSR3B : 0)))) /* TODO? error handling: */
#define UCSRnC(uart) *((uart==0)? &UCSR0C : ((uart==1)? &UCSR1C : ((uart==2)? &UCSR2C : ((uart==3)? &UCSR3C : 0)))) /* TODO? error handling: */
#define UBRRn(uart)  *((uart==0)?  &UBRR0 : ((uart==1)?  &UBRR1 : ((uart==2)?  &UBRR2 : ((uart==3)?  &UBRR3 : 0)))) /* TODO? error handling: */

/* Evaluates to true if the given uart is enabled. */
#define UART_ENABLED(uart) (UCSRnB(uart) & _BV(TXEN0))

/* Enable and configure a uart.
 * Parity options?
 *  0 - off
 *  1 - odd
 *  2 - even
 * Stop bits can be 1 or 2. */
void uart_enable(uint8_t uart, uint32_t baud, uint8_t stopbits, uint8_t parity){
	UBRRn(uart) = (16000000l/(8*baud)) - 1;
	UCSRnA(uart) = _BV(U2X0);
	UCSRnB(uart) = 0; /* TODO: Enable interrupts. */
	UCSRnC(uart) = _BV(UCSZ01) | _BV(UCSZ00)
	               | ((stopbits == 2) ? _BV(USBS0) : 0)
	               | ((parity == 1) ? _BV(UPM01) | _BV(UPM00) : 0)
	               | ((parity == 2) ? _BV(UPM01) : 0);
	UCSRnB(uart) |= _BV(RXEN0) | _BV(TXEN0);
}

/* Disable a uart. */
void uart_disable(uint8_t uart){
	UCSRnB(uart) = 0;
}

/* Send a block of data through the given uart.
 * If there is space in the uart buffer, the data will be copied into the
 * buffer and this function will return immediately. Otherwise,
 * it will block until enough space had been made.
 * This function may used by an interrupt as long as the uart employed
 * is only accessed in interrupts. To put it another way,
 * this function is re-entrant amoung uarts but not for the same uart.
 * If the given uart is disabled, this function will do nothing
 * and return immediately. */
void uart_tx(uint8_t uart, const uint8_t *data, uint16_t count){
	if(!UART_ENABLED(uart))return;
	const uint8_t *end = data + count;
	while(data < end){
		while(!(UCSRnA(uart) & _BV(UDRE0))){
			/* Loop until ready to transmit. */
		}
		UDRn(uart) = *data;
		data++;
	}
	//if interrupts not enabled, need to handle transmission manually
}

/* Returns 1 if data is being sent through the uart, 0 if not. */
uint8_t uart_tx_in_progress(uint8_t uart);

/* Send a packet to the computer, automatically inserting
 * start, end, and escape bytes.
 * This function must only be used by the communications module
 * (comm.c). */
void send_packet(uint8_t *data, uint16_t count){
	//TODO: make not bad
	uint8_t c = 0x01;
	uart_tx(COMM_UART, &c, 1);
	for(uint16_t i=0;i<count;i++){
		if(data[i] == 0x01 || data[i] == 0x02 || data[i] == 0x03){
			c = 0x02;
			uart_tx(COMM_UART, &c, 1);
		}
		uart_tx(COMM_UART, &(data[i]), 1);
	}
	c = 0x03;
	uart_tx(COMM_UART, &c, 1);
}

/* Receive data from the uart. 
 * Up to capacity bytes will be written to the data buffer.
 * The number of bytes written is returned.
 * If this function is not called frequently enough, causing the receive
 * buffer to overflow, the extra data will be lost.
 * If a receive handler is specified (with UART_HANDLER()),
 * this function will never provide any data. */
uint8_t uart_rx(uint8_t uart, uint8_t *data, uint8_t *capacity);