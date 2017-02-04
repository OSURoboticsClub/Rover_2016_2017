/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * compass.h - HMC5883l external compass module.
 * Author(s): Zachary Homen
 */
#ifndef COMPASS_H
#define COMPASS_H

#include <stdint.h>

/* This module will need to communicate with an HMC5883l compass over I2C
 * (built into https://hobbyking.com/en_us/ublox-neo-7m-gps-with-compass-and-pedestal-mount.html)
 * 3-axis magnetometer to determine the magnetic heading.
 * If nessecary, the current gravity vector can be supplied from the
 * IMU (if heading calculation depends on the tilt of the compass).
 * The compass will be on its own SPI bus. */

/* Get acceleration and magnetometer values from the IMU,
 * then use them to return the magnetic compass heading
 * of the rover. 
 * TODO: Units */


void comp_init(void);
//void compass_wr(struct compass_packet *packet);
void config_rega(void);
void config_regb(void);
void mode(void);
void retrieve(void);
void read_x(void);
void read_z(void);
void read_y(void);

void twi_init(void);
void twi_start(void);
void twi_stop(void);
uint8_t twi_getStatus(void);
void twi_write(uint8_t u8data);
uint8_t twi_readACK(void);
uint8_t twi_readNACK(void);
uint8_t twi_writeto(uint16_t u16addr, uint8_t u8data);
uint8_t twi_readfr(uint16_t u16addr);

#endif /* COMPASS_H */
//uint16_t compass_heading(void);