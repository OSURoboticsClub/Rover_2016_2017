/* OSU Robotics Club Rover 2016-2017
 * Radio adapter board firmware
 *
 * adapterboard.c - Main control loop
 * Author: Aaron Cohen
 */

#include <avr/interrupt.h>
#include "buf.h"
#include "spi.h"
#include "uart.h"


void adapterboard_init(void)
{
	buf_init();
	uart_init();
	spi_init();

	/* Enable interrupts */
	sei();
}


void adapterboard_main(void)
{
	adapterboard_init();

	while (1)
	{
		if (buf_packet_check(BUF_OUT))
		{
			spi_fifo_write();
		}

		if (buf_packet_check(BUF_IN))
		{
			uart_tx();
		}
	}
}


int main(void)
{
	adapterboard_main();

	return 0;
}
