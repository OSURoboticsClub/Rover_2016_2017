/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * ax12.h - AX12 Servo module.
 * Author(s): 
 */
#include "ax12.h"
#include "uart.h"

#define AX12_UART 1

/* Initialize the UART for the AX12s (and connect the AX12s with the
 * switch).
 * Since the UART is shared between the AX12 and saberteeth, this
 * function will be called after using the saberteeth and before using
 * the AX12s. */
void ax12_init(void);

/* Set the angle of a given AX12 servo. */
void ax12_angle(uint8_t servo_addr, int16_t angle);

/* Wait until no more data is being sent from the AX12 UART,
 * then disable it. */
void ax12_release(void){
	