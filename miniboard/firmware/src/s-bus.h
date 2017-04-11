/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * sbus.h - Futaba S-BUS receiver module.
 * Author(s) Aaron Cohen
 */

#include <stdint.h>

#define SBUS_UART 3
#define SBUS_BAUD 100000

/* S-BUS protocol overview:
 * Packets are 25 bytes long, starting with 0xF0 (byte 0) and ending with 0x00
 * (byte 24). Bytes 1-22 contain data for the 16 servo channels with each
 * channel taking up 11 bits. So Channel 1 would use byte 1 + the first three
 * bits of byte 2, Channel 2 would use the last five bytes of byte 2 + the
 * first six bytes of byte 3, etc. Byte 23 contains flags laid out as such:
 *
 * Bit 7 - Digital Channel 17
 * Bit 6 - Digital Channel 18
 * Bit 5 - Frame lost
 * Bit 4 - Failsafe activated
 * Bits 3-0 - Not used */

extern uint16_t sbus_channels[16];
extern uint8_t sbus_failsafe;

/* Initialize the S-BUS receiver module to start listening for S-BUS commands
 * over a UART */
void sbus_init(void);

/* Disable the S-BUS UART so it can be used for other purposes (to send camera
 * control commands). */
void sbus_release(void);
