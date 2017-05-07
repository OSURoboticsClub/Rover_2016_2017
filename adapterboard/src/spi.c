/* OSU Robotics Club Rover 2016-2017
 * Radio adapter board firmware
 *
 * spi.c - SPI interface to RFM9X radio
 * Author: Aaron Cohen
 */

#include <avr/io.h>
#include <util/atomic.h>
#include <stdint.h>
#include "buf.h"
#include "spi.h"

#define FIFO_RX_BASE_ADDR 0x00
#define FIFO_TX_BASE_ADDR 0x80

#define REG_FIFO 0x00
#define REG_FIFO_ADDR_PTR 0x0D
#define REG_FIFO_RX_CURRENT_ADDR 0x10
#define REG_IRQ_FLAGS 0x12
#define REG_RX_NB_BYTES 0x13

#define IRQ_RX_DONE 0x40

#define WRITE_ADDR(addr) (0x80 | (addr & 0x7F))
#define READ_ADDR(addr) (addr & 0x7F)


inline void spi_wait(void)
{
	while(!(SPSR & _BV(SPIF)));
}


static void spi_single_write(uint8_t addr, uint8_t data)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		SPDR = WRITE_ADDR(addr);
		spi_wait();
		SPDR = data;
		spi_wait();
	}
}


static uint8_t spi_single_read(uint8_t addr)
{
	uint8_t data;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		SPDR = READ_ADDR(addr);
		spi_wait();
		spi_wait(); /* Wait again for radio to send back data */
		data = SPDR;
	}

	return data;
}


static void spi_fifo_write(void)
{
	/* Prepare to write FIFO data by resetting the address pointer */
	spi_single_write(REG_FIFO_ADDR_PTR, FIFO_TX_BASE_ADDR);

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		SPDR = WRITE_ADDR(REG_FIFO);
		spi_wait();
		for (uint8_t i = 0; i < buf_size(BUF_OUT); i++)
		{
			SPDR = buf_peek_first(BUF_OUT);
			buf_pop_first(BUF_OUT);
			spi_wait();
		}
	}
}


static uint8_t spi_fifo_read(void)
{
	uint8_t num_bytes = spi_single_read(REG_RX_NB_BYTES);
	uint8_t current_addr = spi_single_read(REG_FIFO_RX_CURRENT_ADDR);
	/* Prepare to read FIFO data by setting the address pointer */
	spi_single_write(REG_FIFO_ADDR_PTR, current_addr);

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		SPDR = READ_ADDR(REG_FIFO);
		spi_wait();
		for (uint8_t i = 0; i < num_bytes; i++)
		{
			spi_wait();
			buf_add(BUF_IN, SPDR);
		}
	}

	return num_bytes;
}


void spi_init(void)
{
	/* Set DDR output for MOSI, SCK, and /CS */
	DDRA = _BV(DDA1) | _BV(DDA3) | _BV(DDA2);

	/* Enable SPI and set Master mode */
	SPCR = _BV(SPE) | _BV(MSTR);

	/* Remap SPI pins to PA0-PA4 */
	REMAP |= _BV(SPIMAP);

	/* Enable external interrupts from INT0 (PB1) */
	GIMSK = _BV(INT0);

	/* Generate interrupt on falling edge */
	MCUCR = _BV(ISC01);

	/* TODO:
	 * set LoRa mode
	 * set explicit header mode
	 * Probably set other things too
	 */
}


ISR(INT0_vect)
{
	/* Determine what kind of interrupt it is */
	uint8_t irq_flags = spi_single_read(REG_IRQ_FLAGS);

	if (irq_flags & IRQ_RX_DONE)
	{
		spi_fifo_read();
	}
}
