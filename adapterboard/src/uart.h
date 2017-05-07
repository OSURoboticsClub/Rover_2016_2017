/* OSU Robotics Club Rover 2016-2017
 * Radio adapter board firmware
 *
 * uart.h - UART interface to miniboard
 * Author: Aaron Cohen
 */

#include <stdint.h>

#define UART_BAUD 9600

void uart_init(void);

void uart_tx(void);
