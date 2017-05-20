/* OSU Robotics Club Rover 2017
 * Science Board Firmware
 *
 * crc.h - Payload CRC calculation module
 * Author(s): Aaron Cohen
 */

#include <stdint.h>


uint16_t calc_crc(uint8_t *buf, uint8_t count);

uint8_t check_crc(uint8_t *buf, uint8_t count, uint8_t *ref);
