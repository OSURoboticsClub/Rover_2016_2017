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
 *   void (*UART1RXHandler)(uint8_t) = module_byte_handler_func;
 * in the top level of the module .c file. */
extern void (*UART0RXHandler)(uint8_t);
extern void (*UART1RXHandler)(uint8_t);
extern void (*UART2RXHandler)(uint8_t);
extern void (*UART3RXHandler)(uint8_t);

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
 * Once this function returns, the data block may be reused.
 * This function may used by an interrupt as long as the uart employed
 * is only accessed in interrupts. To put it another way,
 * this function is re-entrant among uarts but not for the same uart.
 * If the given uart is disabled, this function will do nothing
 * and return immediately.
 * WARNING: If called from an interrupt, the first byte of data will be sent immediately.
 * However, subsequent bytes will not be sent until the interrupt exits.
 * (Unless there's not enough space in the buffer; in that case, data will be sent
 * until there's enough space, then this function will return, then no more data
 * will be sent until the interrupt exits.) */
void uart_tx(uint8_t uart, const uint8_t *data, uint16_t count);

/* Returns 1 if data is being sent through the uart, 0 if not. */
uint8_t uart_tx_in_progress(uint8_t uart);

/* Receive data from the uart. 
 * Up to capacity bytes will be written to the data buffer.
 * The number of bytes written is returned.
 * If this function is not called frequently enough, causing the receive
 * buffer to overflow, the extra data will be lost.
 * If a receive handler is specified (with UART_HANDLER()),
 * this function will never provide any data. */
uint8_t uart_rx(uint8_t uart, uint8_t *data, uint8_t capacity);
