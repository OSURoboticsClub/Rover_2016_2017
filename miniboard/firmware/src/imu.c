/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * imu.h - Bosch BMX055 IMU module.
 */
#include <stdint.h>
#include "spi.h"
#include "imu.h"
#include <avr/interrupt.h>
#include <util/atomic.h>
#define F_CPU 16000000
#include <util/delay.h>


/* Setup the IMU. Assumes that SPI has already been initialized. */
void imu_init(void){
	/* Accelerometer config */
	spi_write_reg_byte(SPI_CS_ACCEL, 0x0F, 5); /* 4G range */
	spi_write_reg_byte(SPI_CS_ACCEL, 0x10, 9); /* 15Hz bandwidth */
}

/* Get acceleration values.
 * TODO: units. */
void imu_accel(volatile int16_t *ax, volatile int16_t *ay, volatile int16_t *az){
	union {
		uint8_t u[6];
		int16_t i[3];
	} buf;

	spi_read_reg(SPI_CS_ACCEL, 0x02, buf.u, 6);

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		*ax = buf.i[0];
		*ay = buf.i[1];
		*az = buf.i[2];
	}
}

/* Get rotation rate values.
 * TODO: units. */
void imu_gyro(volatile int16_t *gx, volatile int16_t *gy, volatile int16_t *gz){
	union {
		uint8_t u[6];
		int16_t i[3];
	} buf;

	spi_read_reg(SPI_CS_GYRO, 0x02, buf.u, 6);

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		*gx = buf.i[0];
		*gy = buf.i[1];
		*gz = buf.i[2];
	}
}

/* Get magnetometer values. */
// void imu_mag(int16_t *mx, int16_t *my, int16_t *mz){
// 	
// }

