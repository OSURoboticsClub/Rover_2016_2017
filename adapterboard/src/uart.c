/* OSU Robotics Club Rover 2016-2017
 * Radio adapter board firmware
 *
 * uart.c - UART interface to miniboard
 * Author: Aaron Cohen
 */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/atomic.h>
#include <stdint.h>
#include "buf.h"
#include "uart.h"


static inline void uart_wait(void)
{
	while(!(UCSR0A & _BV(UDRE0)));
}


void uart_init(void)
{
	/* Set baud rate */
	UBRR0H = (UART_BAUD >> 8) & 0x0F;
	UBRR0L = UART_BAUD & 0xFF;

	/* Double async transfer rate (for compatibility with miniboard UART) */
	UCSR0A = _BV(U2X0);

	/* Enable reciever, transmitter, and reciever interrupt */
	UCSR0B = _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0);

	/* Set 8-bit character size */
	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);

	/* Remap UART0 to PB2 and PA7 */
	REMAP |= _BV(U0MAP);
}


void uart_tx(void)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		for (uint8_t i = 0; i < buf_size(BUF_IN); i++)
		{
			uart_wait();
			UDR0 = buf_peek_first(BUF_IN);
			buf_pop_first(BUF_IN);
		}
	}
}


ISR(USART0_RX_vect)
{
	buf_add(BUF_OUT, UDR0);
}
