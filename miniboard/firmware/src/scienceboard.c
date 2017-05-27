/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 * 
 * scienceboard.c - Science Board Interface
 * Author(s): Aaron Cohen
 */

#include <avr/io.h>
#include <util/atomic.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "commgen.h"
#include "scienceboard.h"


uint8_t serial_digits[9];
uint8_t probe_addr[3] = {'0', '0', '0'};
uint8_t probe_soil_type = 1;


static void memset_volatile(volatile uint8_t *dest, uint8_t val, uint16_t size)
{
	for (uint16_t i = 0; i < size; i++)
	{
		dest[i] = val;
	}
}


static void memcpy_volatile(volatile uint8_t *dest, const uint8_t *src, uint16_t size)
{
	for (uint16_t i = 0; i < size; i++)
	{
		dest[i] = src[i];
	}
}


static void spi_cs_sci(uint8_t state)
{
	DDRL |= _BV(PL2);
	if(state){
		PORTL |= _BV(PL2);
	} else {
		PORTL &= ~_BV(PL2);
	}
}


static void spi_tx(const uint8_t *data, uint16_t count)
{
	for (uint16_t i = 0; i < count; i++)
	{
		SPDR = data[i];

		while(!(SPSR & _BV(SPIF)));
	}
}


static void spi_tx_imm(uint8_t data)
{
	SPDR = data;
	while(!(SPSR & _BV(SPIF)));
}


static void spi_rx(uint8_t *buf, uint16_t count)
{
	uint8_t discard;

	/* If buf is null, discard received data */
	if (buf == NULL)
		buf = &discard;

	for (uint16_t i = 0; i < count; i++)
	{
		SPDR = 0;
		while (!(SPSR & _BV(SPIF)));
		buf[i] = SPDR;
	}
}


static void scienceboard_send_packet(uint8_t cmd, uint8_t args_len, const uint8_t *args)
{
	spi_tx_imm(0x01);
	spi_tx_imm(args_len + 4);
	spi_tx_imm(cmd);
	spi_tx(probe_addr, 3);

	if (args != NULL)
		spi_tx(args, args_len);
}


static uint8_t scienceboard_receive_packet(uint8_t *payload)
{
	uint8_t payload_len;

	spi_rx(NULL, 1);
	spi_rx(&payload_len, 1);
	spi_rx(payload, payload_len);

	return payload_len;
}


/* Initialize the science board interface */
void scienceboard_init(void)
{
	/* Make sure SPI is initialized */
	if (!(SPCR & _BV(SPE)))
	{
		DDRB |= _BV(PB1); /* SCK */
		DDRB |= _BV(PB2); /* MOSI */
		DDRB |= _BV(PB0); /* SS */
		PORTB |= _BV(PB0); /* MOSI */
		SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR0);
	}

	spi_cs_sci(1);

	/* Get probe serial number */
	uint32_t serial;
	scienceboard_probe_read_serial(serial_digits);
	serial_digits[8] = '\0';
	serial = strtoul((const char *) serial_digits, NULL, 10);

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		Data->probe_serial = serial;
	}

	/* Get probe version string */
	uint8_t firmware_ver[4];
	scienceboard_probe_read_firmware_ver(firmware_ver);

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		Data->probe_ver_3 = firmware_ver[3];
		Data->probe_ver_2 = firmware_ver[2];
		Data->probe_ver_1 = firmware_ver[1];
		Data->probe_ver_0 = firmware_ver[0];
	}

	/* Set default probe address */
	uint8_t address[3] = {'0', '0', '0'};
	scienceboard_probe_write_address(address);

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		Data->probe_addr_2 = '0';
		Data->probe_addr_1 = '0';
		Data->probe_addr_0 = '0';
	}

	/* Set default soil type */
	scienceboard_probe_write_soil_type(1);

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		Data->probe_soil_type = 1;
	}

	/* Set up Data readings buffers */
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		Data->readings_0_len = 0;
		memset_volatile(Data->readings_0, 0, 256);
		Data->readings_1_len = 0;
		memset_volatile(Data->readings_1, 0, 256);
		Data->readings_2_len = 0;
		memset_volatile(Data->readings_2, 0, 256);
		Data->readings_3_len = 0;
		memset_volatile(Data->readings_3, 0, 256);
		Data->readings_4_len = 0;
		memset_volatile(Data->readings_4, 0, 256);
		Data->readings_5_len = 0;
		memset_volatile(Data->readings_5, 0, 256);
	}
}


void scienceboard_update_readings_buffers(void)
{
}


/* Read the serial number of the soil probe
 *   serial_buf - The buffer to write the serial number into (8 bytes) */
void scienceboard_probe_read_serial(uint8_t *serial_buf)
{
	uint8_t payload[11];

	spi_cs_sci(0);

	scienceboard_send_packet(0x00, 0, NULL);
	scienceboard_receive_packet(payload);

	spi_cs_sci(1);

	memcpy(serial_buf, &(payload[3]), 8);
}


/* Read the firmware version of the soil probe
 *   ver_buf - The buffer to write the firmware version into (3 or 4 bytes) */
void scienceboard_probe_read_firmware_ver(uint8_t *ver_buf)
{
	uint8_t payload[7];

	spi_cs_sci(0);

	scienceboard_send_packet(0x01, 0, NULL);
	scienceboard_receive_packet(payload);

	spi_cs_sci(1);

	memcpy(ver_buf, &(payload[3]), 4);
}


/* Read the address of the soil probe
 *   addr_buf - The buffer to store the probe's address into (3 bytes) */
void scienceboard_probe_read_address(uint8_t *addr_buf)
{
	uint8_t payload[3];

	spi_cs_sci(0);

	scienceboard_send_packet(0x02, 0, NULL);
	scienceboard_receive_packet(payload);

	spi_cs_sci(1);

	memcpy(addr_buf, payload, 3);
}


/* Set the address of the soil probe
 *   new_addr - The buffer containing the probe's new address */
void scienceboard_probe_write_address(const uint8_t *new_addr)
{
	uint8_t args[11];

	memcpy(args, serial_digits, 8);
	memcpy(&(args[8]), new_addr, 3);
	memcpy(probe_addr, new_addr, 3);

	spi_cs_sci(0);

	scienceboard_send_packet(0x03, 11, args);
	scienceboard_receive_packet(NULL);

	spi_cs_sci(1);
}


/* Read the soil probe's current soil type (See manual) */
uint8_t scienceboard_probe_read_soil_type(void)
{
	uint8_t payload[4];

	spi_cs_sci(0);

	scienceboard_send_packet(0x04, 0, NULL);
	scienceboard_receive_packet(payload);

	spi_cs_sci(1);

	return payload[3];
}


/* Set a new soil type for the soil probe 
 *   new_type - The new soil type (See manual) */
void scienceboard_probe_write_soil_type(uint8_t new_type)
{
	uint8_t args[1];

	args[0] = new_type;
	probe_soil_type = new_type;

	spi_cs_sci(0);

	scienceboard_send_packet(0x05, 1, args);
	scienceboard_receive_packet(NULL);

	spi_cs_sci(1);
}


/* Tell the soil probe to take a reading */
void scienceboard_probe_take_reading(void)
{
	spi_cs_sci(0);

	scienceboard_send_packet(0x06, 0, NULL);

	spi_cs_sci(1);
}


/* Get a reading set from the soil probe
 *   set - The reading set to return (See manual)
 *   reading_buf - The buffer to store the reading set into (1 - 100 bytes)
 *   returns - The length of the reading set */
uint8_t scienceboard_probe_get_reading(uint8_t set, volatile uint8_t *reading_buf)
{
	uint8_t args[1];
	uint8_t payload[103];
	uint8_t payload_len;

	args[0] = set;

	spi_cs_sci(0);

	scienceboard_send_packet(0x07, 1, args);
	payload_len = scienceboard_receive_packet(payload);

	spi_cs_sci(1);

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		memcpy_volatile(reading_buf, &(payload[3]), payload_len - 3);
	}

	return payload_len - 3;
}
