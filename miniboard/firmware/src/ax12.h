/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * ax12.h - AX12 Servo module.
 */
#include <stdint.h>


/* Set the analog switch to transmit on the AX12 line. */
static void ax12_tx(void){
	//TODO
}

/* Set the analog switch to receive on the AX12 line. */
static void ax12_rx(void){
	//TODO
}

/* This module needs to be able to control AX12 servos.
 * There should be a function to tell an AX12 to move to
 * a given position. If the AX12s have other features,
 * like load measurement or whatever, it would be nice to
 * have then too.
 * The AX12s might need to turn around the serial connection,
 * using the same line for transmission and receptions.
 * The serial module doesn't currently support this, but it
 * can be added. Read the docs and let me know what you need.
 * Use a #define to control the UART used. */

/* Initialize the UART for the AX12s (and connect the AX12s with the
 * switch).
 * Since the UART is shared between the AX12 and saberteeth, this
 * function will be called after using the saberteeth and before using
 * the AX12s. */
void ax12_init(void);

/* Set the angle of a given AX12 servo. */
void ax12_angle(uint8_t servo_addr, int16_t angle){
	//things to do:
	//  -configure uart with AX12 baud rate
	//  -set analog switch to TX mode
	

/* Wait until no more data is being sent from the AX12 UART,
 * then disable it. */
void ax12_release(void){
	while(uart_tx_in_progress(AX12_UART)){
		/* Wait for send to finish. */
	}
	uart_disable(AX12_UART);
}