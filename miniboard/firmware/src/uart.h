/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 * 
 * uart.h - UART interface module.
 * This module provides support for UARTs 0-3.
 * When calling functions, give the uart as a
 * number from 0-3.
 */
#include <stdint.h>

/* To enable modules to handle data from the UARTs as it comes it,
 * the following functions will be called from the UART receive interrupts
 * if the symbols are defined in the module. So, if your module needs to handle
 * bytes from UART 1 as they arrive, place the code
 *   UART1RXHandler = module_byte_handler_func;
 * in the top level of the module .c file
 * or use the macro below. */
extern void __attribute__((weak)) (*UART0RXHandler)(uint8_t);
extern void __attribute__((weak)) (*UART1RXHandler)(uint8_t);
extern void __attribute__((weak)) (*UART2RXHandler)(uint8_t);
extern void __attribute__((weak)) (*UART3RXHandler)(uint8_t);

/* This macro provides a parametric interface to the handler function pointers.
 * This way, the UART used by a module can be configured with a #define, like so:
 *   #define MODULE_UART 1
 *   UART_HANDLER(MODULE_UART, module_byte_handler_func);
 */
#define UART_HANDLER(uart, func) void __attribute__((weak)) (*UART##uart##RXHandler)(uint8_t) = func

/* Enable and configure a uart.
 * Parity options:
 *  0 - off
 *  1 - odd
 *  2 - even
 * Stop bits can be 1 or 2. */
void uart_enable(uint8_t uart, uint32_t baud, uint8_t stopbits, uint8_t parity);

/* Disable a uart. */
void uart_disable(uint8_t uart);

/* Send a block of data through the given uart.
 * If there is space in the uart buffer, the data will be copied into the
 * buffer and this function will return immediately. Otherwise,
 * it will block until enough space had been made.
 * This function may be used during interrupts.
 * If the given uart is disabled, this function will do nothing
 * and return immediately. */
void uart_tx(uint8_t uart, uint8_t *data, uint16_t count);

/* Returns 1 if data is being sent through the uart, 0 if not. */
uint8_t uart_tx_in_progress(uint8_t uart);

// send_packet();

/* Receive data from the uart. 
 * Up to capacity bytes will be written to the data buffer.
 * The number of bytes written is returned.
 * If this function is not called frequently enough, causing the receive
 * buffer to overflow, the extra data will be lost.
 * If a receive handler is specified (with UART_HANDLER()),
 * this function will never provide any data. */
uint8_t uart_rx(uint8_t uart, uint8_t *data, uint8_t *capacity);