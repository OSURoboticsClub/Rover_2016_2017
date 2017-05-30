/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * time.h - Millisecond timekeeping.
 * Author(s): Nick Ames
 */
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "commgen.h"


ISR(TIMER3_COMPA_vect){
	Data->time_ms++;
}

/* Return the current time value (milliseconds since the rover was
 * powered on). */
uint32_t get_ms(void){
	uint32_t temp;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		temp = Data->time_ms;
	}
	return temp;
}
	
/* Start the timer used for autonomous driving. */
void time_init(void){
	TCCR3B = _BV(WGM32);
	TCNT3 = 0;
	OCR3A = (uint16_t) 249;
	TIMSK3 = _BV(OCIE3A);
	TCCR3B |= _BV(CS31) | _BV(CS30); /* Set 64 prescaler. */
}