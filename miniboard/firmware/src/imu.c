/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * imu.h - Bosch BMX055 IMU module.
 */
#include <stdint.h>
#include "uart.h"
#include "imu.h"
#include <avr/interrupt.h>
#include <util/atomic.h>
#define F_CPU 16000000
#include <util/delay.h>

static void spi_cs_accel(uint8_t state);
static void spi_cs_gyro(uint8_t state);
static void spi_cs_mag(uint8_t state);
static void spi_init(void);

/* The tx and rx functions wait until the transaction is complete
 * before returning. */
static void spi_tx(const uint8_t *data, uint16_t count);
static void spi_rx(uint8_t *data, uint16_t count);
static void read_reg(void (*csfunc)(uint8_t), uint8_t reg, uint8_t *data, uint16_t count);
static void write_reg(void (*csfunc)(uint8_t), uint8_t reg, uint8_t *data, uint16_t count);

static void spi_cs_accel(uint8_t state){
	DDRL |= _BV(PL3);
	if(state){
		PORTL |= _BV(PL3);
	} else {
		PORTL &= ~_BV(PL3);
	}
}

static void spi_cs_gyro(uint8_t state){
	DDRL |= _BV(PL1);
	if(state){
		PORTL |= _BV(PL1);
	} else {
		PORTL &= ~_BV(PL1);
	}
}

static void spi_cs_mag(uint8_t state){
	DDRL |= _BV(PL0);
	if(state){
		PORTL |= _BV(PL0);
	} else {
		PORTL &= ~_BV(PL0);
	}
}

static void spi_tx(const uint8_t *data, uint16_t count){
	for(uint16_t i=0;i<count;i++){
		SPDR = data[i];
		while(!(SPSR & _BV(SPIF))){
				/* Wait for transaction to complete. */
		}
	}

}

static void spi_rx(uint8_t *data, uint16_t count){
	for(uint16_t i=0;i<count;i++){
		SPDR = 0;
		while(!(SPSR & _BV(SPIF))){
			/* Wait for transaction to complete. */
		}
		data[i] = SPDR;
	}
}

static void spi_init(void){
	DDRB |= _BV(PB1); /* SCK */
	DDRB |= _BV(PB2); /* MOSI */
	DDRB |= _BV(PB0); /* SS */
	PORTB |= _BV(PB0); /* MOSI */
	spi_cs_mag(1);
	spi_cs_accel(1);
	spi_cs_gyro(1);
	SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR0);
}

static void read_reg(void (*csfunc)(uint8_t), uint8_t reg, uint8_t *data, uint16_t count){
	csfunc(0);
	_delay_us(1);
	reg |= 0x80;
	spi_tx(&reg, 1);
	spi_rx(data, count);
	_delay_us(1);
	csfunc(1);
}

static void write_reg(void (*csfunc)(uint8_t), uint8_t reg, uint8_t *data, uint16_t count){
	csfunc(0);
	_delay_us(1);
	reg &= 0x7F;
	spi_tx(&reg, 1);
	spi_tx(data, count);
	_delay_us(1);
	csfunc(1);
}
/* Setup the IMU. */
void imu_init(void){
	spi_init();
// 	bw = 15Hz 0x9 (ACC 0x010)
// 	range = 4G 0x5 (ACC 0x0F)
}

/* Get acceleration values.
 * TODO: units. */
void imu_accel(volatile int16_t *ax, volatile int16_t *ay, volatile int16_t *az){
	int16_t ax_val = 0;
	int16_t ay_val = 0;
	int16_t az_val = 0;
	
	uint8_t r;
	read_reg(spi_cs_accel, 0, &r, 1);
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		*ax = ax_val;
		*ay = ay_val;
		*az = az_val;
	}
}

/* Get rotation rate values.
 * TODO: units. */
// void imu_gyro(int16_t *gx, int16_t *gy, int16_t *gz){
// 	
// }

/* Get magnetometer values. */
// void imu_mag(int16_t *mx, int16_t *my, int16_t *mz){
// 	
// }

