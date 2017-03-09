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
void imu_accel(volatile int16_t *ax, volatile int16_t *ay, volatile int16_t *az);

/* Get rotation rate values.
 * TODO: units. */
void imu_gyro(volatile int16_t *gx, volatile int16_t *gy, volatile int16_t *gz);