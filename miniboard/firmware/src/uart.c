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
#include <util/atomic.h>
#include "uart.h"
#include <stdlib.h>
#include <stdbool.h>
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
 * Start indexes to the first character in the buffer. 
 * End indexes one past the last character in the buffer.
 * If Start == End, the buffer is empty. */
typedef struct {
	uint8_t *data;
	uint8_t start;
	uint8_t end;
	uint8_t capacity;
} circbuf_t;
static uint8_t URBuf[4][UART_RX_BUFFER];
static uint8_t UTBuf[4][UART_TX_BUFFER];
static circbuf_t UT[4];
static circbuf_t UR[4];

#define CIRC_EOF 0xFFEE

/* Remove a character from the circular buffer and return it.
 * Returns CIRC_EOF if there are no more characters in the buffer. */
static uint16_t circ_remove(circbuf_t *buf){
	if(buf->start == buf->end)return CIRC_EOF;
	uint8_t c = buf->data[buf->start];
	buf->start++;
	if(buf->start == buf->capacity){
		buf->start = 0;
	}
	return c;
}

/* Add a character to a circular buffer.
 * Returns 0 on success, -1 on error (not enough space). */
static int8_t circ_add(circbuf_t *buf, uint8_t c){
	if(buf->end == (buf->start - 1)
	   || (buf->start == 0 && buf->end == buf->capacity - 1)
	   )return -1;
	buf->data[buf->end] = c;
	buf->end++;
	if(buf->end == buf->capacity){
		buf->end = 0;
	}
	return 0;
}

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
	UR[uart].data = URBuf[uart];
	UR[uart].capacity = UART_RX_BUFFER;
	UR[uart].start = 0;
	UR[uart].end = 0;
	
	UT[uart].data = UTBuf[uart];
	UT[uart].capacity = UART_TX_BUFFER;
	UT[uart].start = 0;
	UT[uart].end = 0;
	
	UBRRn(uart) = (16000000l/(8*baud)) - 1;
	UCSRnA(uart) = _BV(U2X0);
	UCSRnC(uart) = _BV(UCSZ01) | _BV(UCSZ00)
	               | ((stopbits == 2) ? _BV(USBS0) : 0)
	               | ((parity == 1) ? _BV(UPM01) | _BV(UPM00) : 0)
	               | ((parity == 2) ? _BV(UPM01) : 0);
	UCSRnB(uart) = _BV(RXCIE0) | _BV(TXCIE0) | _BV(RXEN0) | _BV(TXEN0);
}

/* Disable a uart. */
void uart_disable(uint8_t uart){
	UCSRnB(uart) = 0;
}

/* UART receive interrupts and handler. */
static void uart_rx_isr(uint8_t uart){
	
	
	if(uart == 0 && UART0RXHandler != NULL){
		UDR0 = 'r';
		int y = UDR0;
		//UART0RXHandler(UDRn(uart));
	} else
	if(uart == 1 && UART1RXHandler != NULL){
		UART1RXHandler(UDRn(uart));
	} else
	if(uart == 2 && UART2RXHandler != NULL){
		UART2RXHandler(UDRn(uart));
	} else
	if(uart == 3 && UART3RXHandler != NULL){
		UART3RXHandler(UDRn(uart));
	} else {
		circ_add(UR + uart, UDRn(uart));
	}
}

ISR(USART0_RX_vect){
	uart_rx_isr(0);
}

ISR(USART1_RX_vect){
	uart_rx_isr(1);
}

ISR(USART2_RX_vect){
	uart_rx_isr(2);
}

ISR(USART3_RX_vect){
	uart_rx_isr(3);
}

/* UART transmit (data register empty) interrupts and handler. */
static void uart_tx_isr(uint8_t uart){
	uint16_t r = circ_remove(UT + uart);
	if(r != CIRC_EOF){
		UDRn(uart) = r;
	}
}

ISR(USART0_TX_vect){
	uart_tx_isr(0);
}

ISR(USART1_TX_vect){
	uart_tx_isr(1);
}

ISR(USART2_TX_vect){
	uart_tx_isr(2);
}

ISR(USART3_TX_vect){
	uart_tx_isr(3);
}

/* Send a block of data through the given uart.
 * If there is space in the uart buffer, the data will be copied into the
 * buffer and this function will return immediately. Otherwise,
 * it will block until enough space had been made.
 * Once this function returns, the data block may be reused.
 * This function may used by an interrupt as long as the uart employed
 * is only accessed in interrupts. To put it another way,
 * this function is re-entrant among uarts but not for the same uart.
 * If the given uart is disabled, this function will do nothing
 * and return immediately.
 * WARNING: If called from an interrupt, the first byte of data will be sent immediately.
 * However, subsequent bytes will not be sent until the interrupt exits.
 * (Unless there's not enough space in the buffer; in that case, data will be sent
 * until there's enough space, then this function will return, then no more data
 * will be sent until the interrupt exits.) */
void uart_tx(uint8_t uart, const uint8_t *data, uint16_t count){
 	/* possible states:
	 * if not sending, need to prime pump.
   	 * -thread:
   	 *   -enough space in buffer: stuff and exit
     *   -not enough space in buffer: delay until there is
   	 * -interrupt:
     *   -enough space in buffer: stuff and shepard
	 *   -not enough space in buffer: shepard and stuff
	 *  After sheparding, clear interrupt flag. */
	if(!UART_ENABLED(uart))return;
	if(SREG & _BV(7)){
		/* Thread Mode. */
		uint16_t i = 0;
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
			if(!uart_tx_in_progress(uart)){ /* If buffer empty, we'll need to set UDR for the first byte. */
				UDRn(uart) = data[i];
				i++;
			}
			int r = 0;
			/* Fill up the buffer. */
			while(r == 0 && i < count){
				r = circ_add(UT + uart, data[i]);
				if(r == 0)i++;
			}
		}
		/* At this point, the buffer has been filled and the interrupt should be running.
		 * However, there may be more data to send. */
		while(i < count){
			int r;
			ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
				r = circ_add(UT + uart, data[i]);
			}
			if(r == 0)i++;
		}
	} else {
		/* Interrupt Mode. */
		uint16_t i = 0;
		if(!uart_tx_in_progress(uart)){ /* If buffer empty, we'll need to set UDR for the first byte. */
			UDRn(uart) = data[i];
			i++;
		}
		int r = 0;
		/* Fill up the buffer. */
		while(r == 0 && i < count){
			r = circ_add(UT + uart, data[i]);
			if(r == 0)i++;
		}
		/* At this point, the buffer has been filled and the interrupt should be running.
		 * However, there may be more data to send. Shepard the existing bytes out of the
		 * buffer, since the TXC interrupt can't run right now. */
		while(i < count){
			while(!(UCSRnA(uart) & _BV(UDRE0))){
				/* Wait for byte to be sent */
			}
			uart_tx_isr(uart);
			int r;
			r = circ_add(UT + uart, data[i]);
			if(r == 0)i++;
		}
		
	}
}

/* Returns 1 if data is being sent through the uart, 0 if not. */
uint8_t uart_tx_in_progress(uint8_t uart){
	return !(UCSRnA(uart) & _BV(UDRE0)) || (UT[uart].start !=  UT[uart].end);
}

/* Receive data from the uart. 
 * Up to capacity bytes will be written to the data buffer.
 * The number of bytes written is returned.
 * If this function is not called frequently enough, causing the receive
 * buffer to overflow, the extra data will be lost.
 * If a receive handler is specified (with UART_HANDLER()),
 * this function will never provide any data. */
uint8_t uart_rx(uint8_t uart, uint8_t *data, uint8_t capacity){
	uint16_t i;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		for(i=0; i<capacity; i++){
			uint16_t r = circ_remove(UR + uart);
			if(r == CIRC_EOF){
				break;
			} else {
				data[i] = r;
			}
		}
	}
	return i;
}
