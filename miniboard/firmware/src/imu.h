/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * imu.h - Bosch BMX055 IMU module.
 */
#include <stdint.h>

/* Setup the IMU. */
void imu_init(void);

/* Get acceleration values.
 * TODO: units. */
void imu_accel(int16_t *ax, int16_t *ay, int16_t *az);

/* Get rotation rate values.
 * TODO: units. */
void imu_gyro(int16_t *gx, int16_t *gy, int16_t *gz);

/* Get magnetometer values. */
void imu_mag(int16_t *mx, int16_t *my, int16_t *mz);

/* Get acceleration and magetometer values from the IMU,
 * then use them to return the magnetic compass heading
 * of the rover. TODO: Make this an option: yaw angle of
 * the IMU relative to the rover.
 * TODO: Units */
uint16_t imu_heading(void);