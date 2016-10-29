/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * imu.h - Bosch BMX055 IMU module.
 */
#include <stdint.h>
#include "uart.h"
#include "imu.h"

uint8_t uart_id = 0;//TODO figure this out

/* Setup the IMU. */
void imu_init(void){
	//use uart_id to setup uart
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
	
	*ax = ax_val;
	*ay = ay_val;
	*az = az_val;
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

/* Get acceleration and magetometer values from the IMU,
 * then use them to return the magnetic compass heading
 * of the rover. TODO: Make this an option: yaw angle of
 * the IMU relative to the rover.
 * TODO: Units */
uint16_t imu_heading(void){

}
