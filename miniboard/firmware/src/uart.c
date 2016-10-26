/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 * 
 * uart.h - UART interface module.
 * This module provides support for UARTs 0-3.
 * When calling functions, give the uart as a
 * number from 0-3.
 * Author(s): Nick Ames
 */
#include <avr/interrupt.h>
#include "uart.h"
#include <stdlib.h>

void __attribute__((weak)) (*UART0RXHandler)(uint8_t) = NULL;
void __attribute__((weak)) (*UART1RXHandler)(uint8_t) = NULL;
void __attribute__((weak)) (*UART2RXHandler)(uint8_t) = NULL;
void __attribute__((weak)) (*UART3RXHandler)(uint8_t) = NULL;

#define UART_TX_BUFFER 150 /* Size of per-UART transmission buffer.
                            * When trying to send a chunk of data larger than this,
	                        * the uart_tx() function will block until transmission is complete.
	                        * Must not be greater than 256. */
#define UART_RX_BUFFER 10 /* Size of per-UART reception buffer.
                           * If the buffer fills up, incoming data will be discarded.
                           * Must not be greater than 256. */

/* UART Buffers. */
static uint8_t URBuf[4][UART_RX_BUFFER];
static uint8_t URStart[4];
static uint8_t UREnd[4];
static uint8_t UTBuf[4][UART_TX_BUFFER];
static uint8_t UTStart[4];
static uint8_t UTEnd[4];

/* These macros evaluate to the specific uart control registers. */
#define MR(reg,uart) *((uart==0): &reg##0 ? (uart==1): &reg##1 ? (uart==2): &reg##2 ? (uart==3): &reg##3 ? 0) /* TODO: error handling? */
#define UDRn(uart) MR(UDR, uart)

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