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

	sei();
}


void adapterboard_main(void)
{
	adapterboard_init();

	while (1)
	{
		if (buf_size(BUF_OUT) > 0)
		{
			//TODO
		}

		if (buf_size(BUF_IN) > 0)
		{
			//TODO
		}
	}
}


int main(void)
{
	adapterboard_main();

	return 0;
}
