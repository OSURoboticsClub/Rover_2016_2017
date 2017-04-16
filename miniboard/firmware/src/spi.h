/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * spi.h - SPI peripheral control module. 
 */

#include <stdint.h>
#include <avr/interrupt.h>

enum ChipSelect{
	SPI_CS_MAG = PL0,
	SPI_CS_GYRO = PL1,
	SPI_CS_ACCEL = PL3,
	SPI_CS_RADIO = PL2
};


/* Initializes SPI by setting SPI and DD registers to the appropriate values */
void spi_init(void);


/* Transmits data across the SPI bus */
void spi_tx(enum ChipSelect chip, const uint8_t *data, uint16_t count);


/* Receives data across the SPI bus */
void spi_rx(enum ChipSelect ship, uint8_t *data, uint16_t count);


/* Writes data to a register on a chip connected to the SPI bus */
void spi_write_reg(enum ChipSelect chip, uint8_t reg, const uint8_t *data,
				   uint16_t count);


/* Writes a single byte to a register on a chip connected to the SPI bus */
void spi_write_reg_byte(enum ChipSelect chip, uint8_t reg, uint8_t data);


/* Reads data from a register on a chip connected to the SPI bus */
void spi_read_reg(enum ChipSelect chip, uint8_t reg, uint8_t *data,
				  uint16_t count);
