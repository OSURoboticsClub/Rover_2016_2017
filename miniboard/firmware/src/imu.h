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