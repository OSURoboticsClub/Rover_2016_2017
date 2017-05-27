/* OSU Robotics Club Rover 2017
 * Science Board Firmware
 *
 * sciboard.c - Main control loop
 * Author(s): Aaron Cohen
 */

#include <avr/interrupt.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "comm.h"
#include "soilprobe.h"


void sciboard_init(void) {
	soilprobe_init();
	comm_init();

	/* Enable interrupts */
	sei();
}


void sciboard_main(void) {
	sciboard_init();
	while (1)  /* Wait until SPI interrupt */
	{
		struct soilprobe_cmd cmd;
		struct soilprobe_resp resp;

		cmd.cmd = CMD_GET_SERIAL;
		cmd.addr[0] = '/';
		cmd.addr[1] = '/';
		cmd.addr[2] = '/';
		cmd.argsize = 0;

		soilprobe_cmd(&cmd, &resp);

		_delay_ms(1);
	}
}


int main(void) {
	sciboard_main();
	return 0;
}
