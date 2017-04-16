/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * spi.c - SPI peripheral control module
 */
#include <stdint.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#define F_CPU 16000000
#include <util/delay.h>
#include "spi.h"


static void _spi_cs(enum ChipSelect chip, uint8_t state){
	DDRL |= _BV(chip);
	if (state){
		PORTL |= _BV(chip);
	}
	else{
		PORTL &= ~_BV(chip);
	}
}


void spi_init(void){
	DDRB |= _BV(PB1); /* SCK */
	DDRB |= _BV(PB2); /* MOSI */
	DDRB |= _BV(PB3); /* MISO */
	DDRB |= _BV(PB0); /* SS */
	PORTB |= _BV(PB0); /* MOSI */

	_spi_cs(SPI_CS_MAG, 1); /* IMU Magnetometer */
	_spi_cs(SPI_CS_GYRO, 1); /* IMU Gyro */
	_spi_cs(SPI_CS_ACCEL, 1); /* IMU Accelerometer */
	_spi_cs(SPI_CS_RADIO, 1); /* Data Radio */

	/* SPI Control Register - Enable SPI, Set SPI Master, SCK=f_osc/16 */
	SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR0);
}


void spi_tx(enum ChipSelect chip, const uint8_t *data, uint16_t count){
	_spi_cs(chip, 0);
	_delay_us(1);
	for(uint16_t i = 0; i < count; i++){
		SPDR = data[i];
		while(!(SPSR & _BV(SPIF))){} /* Wait for transaction to complete */
	}
	_spi_cs(chip, 1);
}


void spi_rx(enum ChipSelect chip, uint8_t *data, uint16_t count){
	_spi_cs(chip, 0);
	_delay_us(1);
	for(uint16_t i = 0; i < count; i++){
		SPDR = data[i];
		while(!(SPSR & _BV(SPIF))){} /* Wait for transaction to complete */
	}
}


void spi_write_reg(enum ChipSelect chip, uint8_t reg, const uint8_t *data,
				   uint16_t count){
	reg |= 0x80; /* Indicate that this is a write commend */

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		spi_tx(chip, &reg, 1);
		for(uint8_t i = 0; i < count; i++){
			spi_tx(chip, data + i, 1);
		}
	}
}


void spi_write_reg_byte(enum ChipSelect chip, uint8_t reg, uint8_t data){
	spi_write_reg(chip, reg, &data, 1);
}


void spi_read_reg(enum ChipSelect chip, uint8_t reg, uint8_t *data,
				  uint16_t count){
	reg &= 0x7F; /* Indicate that this is a read command */

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		spi_tx(chip, &reg, 1);
		for(uint8_t i = 0; i < count; i++){
			spi_rx(chip, data + i, 1);
		}
	}
}
