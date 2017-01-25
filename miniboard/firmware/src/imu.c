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

uint8_t uart_id = 0;//TODO figure this out

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
	}
	while(!(SPSR & _BV(SPIF))){
		/* Wait for transaction to complete. */
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
	SPCR = _BV(SPE) | _BV(MSTR);
	spi_cs_mag(1);
	spi_cs_accel(1);
	spi_cs_gyro(1);
}

static void read_reg(void (*csfunc)(uint8_t), uint8_t reg, uint8_t *data, uint16_t count){
	csfunc(0);
	reg &= 0x7F;
	spi_tx(&reg, 1);
	spi_rx(data, count);
	csfunc(1);
}

static void write_reg(void (*csfunc)(uint8_t), uint8_t reg, uint8_t *data, uint16_t count){
	csfunc(0);
	reg |= 0x80;
	spi_tx(&reg, 1);
	spi_tx(data, count);
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
void imu_accel(int16_t *ax, int16_t *ay, int16_t *az){
	//cache x, y, z values
	static int16_t ax_val = 0;
	static int16_t ay_val = 0;
	static int16_t az_val = 0;
	
	//If the new_data bit is set, write that data to the values
		//read LSBs
		//read MSBs
		//convert to int16, though the data is in 12 bits
	//repeat for Y and Z
		
	//other stuff?
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		*ax = ax_val;
		*ay = ay_val;
		*az = az_val;
	}
}

/* Get rotation rate values.
 * TODO: units. */
void imu_gyro(int16_t *gx, int16_t *gy, int16_t *gz){
	//cache dx, dy, dz values
	static int16_t gx_val = 0;
	static int16_t gy_val = 0;
	static int16_t gz_val = 0;
	
	//do things, see imu_accel for the rough layout
	
	*gx = gx_val;
	*gy = gy_val;
	*gz = gz_val;
}

/* Get magnetometer values. */
void imu_mag(int16_t *mx, int16_t *my, int16_t *mz){
	//cache x, y, x values
	static int16_t mx_val = 0;
	static int16_t my_val = 0;
	static int16_t mz_val = 0;
	
	//doing stuff, see imu_accel for rough layout
	
	*mx = mx_val;
	*my = my_val;
	*mz = mz_val;
}

