/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * spi.h - SPI peripheral control module. 
 */

#include <stdint.h>

enum ChipSelect{
	SPI_CS_MAG,
	SPI_CS_GYRO,
	SPI_CS_ACCEL,
	SPI_CS_RADIO
};


/* Initializes SPI by setting SPI and DD registers to the appropriate values */
void spi_init(void);


/* Transmits data across the SPI bus */
void spi_tx(enum ChipSelect chip, const uint8_t *data, uint16_t count);


/* Recieves data across the SPI bus */
void spi_rx(enum ChipSelect ship, const uint8_t *data, uint16_t count);


/* Writes data to a register on a chip connected to the SPI bus */
void spi_write_reg(enum ChipSelect chip, uint8_t reg, const uint8_t *data,
				   uint16_t count);


/* Reads data from a register on a chip connected to the SPI bus */
void spi_read_reg(enum ChipSelect chip, uint8_t reg, const uint8_t *data,
				  uint16_t count);
