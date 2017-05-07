/* OSU Robotics Club Rover 2016-2017
 * Radio adapter board firmware
 *
 * spi.h - SPI interface to RFM9X radio
 * Author: Aaron Cohen
 */

#include <stdint.h>

void spi_init(void);

void spi_fifo_write(void);
