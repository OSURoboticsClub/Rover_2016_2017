/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 * 
 * uart.h - UART interface module.
 * This module provides support for UARTs 0-3.
 * When calling functions, give the uart name as a
 * number from 0-3.
 */

#define UART_TX_BUFFER 150 /* Size of per-UART transmission buffer.
                              When trying to send a chunk of data larger than this,
	                          the uart_tx() function will block until transmission is complete. */
#define UART_RX_BUFFER 10; /* Size of per-UART reception buffer.
                              If the buffer fills up, incoming data will be discarded. */
typedef uint8_t uart_t;

/* 
extern UART1RXHandler;
extern UART2RXHandler;
extern UART3XHandler;
extern UART4RXHandler;

extern UARTHandlers[4];
send_packet();
uart_tx(uart, uint8_t *data, uint8_t count);
uart_rx_handler(func);
uart_rx()*/