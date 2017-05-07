/* OSU Robotics Club Rover 2016-2017
 * Radio adapter board firmware
 *
 * spi.h - SPI interface to RFM9X radio
 * Author: Aaron Cohen
 */

void spi_init(void);

void spi_tx(uint8_t *data, uint8_t count);
