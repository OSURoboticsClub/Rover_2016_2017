/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 * 
 * scienceboard.h - Science Board Interface
 * Author(s): Aaron Cohen
 */

#include <stdint.h>


extern uint8_t serial_digits[9];
extern uint8_t probe_addr[3];
extern uint8_t probe_soil_type;


/* Initialize the science board interface */
void scienceboard_init(void);


/* Read the serial number of the soil probe
 *   serial_buf - The buffer to write the serial number into (8 bytes) */
void scienceboard_probe_read_serial(uint8_t *serial_buf);


/* Read the firmware version of the soil probe
 *   ver_buf - The buffer to write the firmware version into (3 or 4 bytes) */
void scienceboard_probe_read_firmware_ver(uint8_t *ver_buf);


/* Read the address of the soil probe
 *   addr_buf - The buffer to store the probe's address into (3 bytes) */
void scienceboard_probe_read_address(uint8_t *addr_buf);


/* Set the address of the soil probe
 *   new_addr - The buffer containing the probe's new address */
void scienceboard_probe_write_address(const uint8_t *new_addr);


/* Read the soil probe's current soil type (See manual) */
uint8_t scienceboard_probe_read_soil_type(void);


/* Set a new soil type for the soil probe 
 *   new_type - The new soil type (See manual) */
void scienceboard_probe_write_soil_type(uint8_t new_type);


/* Tell the soil probe to take a reading */
void scienceboard_probe_take_reading(void);


/* Get a reading set from the soil probe
 *   set - The reading set to return (See manual)
 *   reading_buf - The buffer to store the reading set into (1 - 100 bytes)
 *   returns - The length of the reading set */
uint8_t scienceboard_probe_get_reading(uint8_t set, volatile uint8_t *reading_buf);
