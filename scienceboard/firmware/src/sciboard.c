/* OSU Robotics Club Rover 2017
 * Science Board Firmware
 *
 * sciboard.c - Main control loop
 * Author(s): Aaron Cohen
 */

#include <avr/interrupt.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <util/atomic.h>


void sciboard_init(void) {
	//...
}


void sciboard_main(void) {
	sciboard_init();
	while (1) {
		//...
	}
}


int main(void) {
	sciboard_main();
	return 0;
}
