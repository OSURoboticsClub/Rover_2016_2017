/* OSU Robotics Club Rover 2017
 * Science Board Firmware
 *
 * soilprobe.h - Soil Probe Module
 * Author(s): Aaron Cohen
 */

#include <stdint.h>
#include <avr/io.h>


#define SOILPROBE_UART 0
#define SOILPROBE_BAUD 9600

// Enable Pin 10
#define SOILPROBE_ENABLE_DDR DDRB
#define SOILPROBE_ENABLE_BANK PORTB
#define SOILPROBE_ENABLE_PIN PB4

#define SOILPROBE_ADDR "///"

#define SOILPROBE_TIMEOUT_US 1000


enum soilprobe_cmd_name {
	CMD_NAME_SERIAL,
	CMD_NAME_FIRMWARE_VER,
	CMD_NAME_ADDR,
	CMD_NAME_SOIL_TYPE,
	CMD_NAME_TAKE_READING,
	CMD_NAME_GET_READING,
};


enum soilprobe_cmd_type {
	CMD_TYPE_EXECUTE,
	CMD_TYPE_QUERY,
	CMD_TYPE_ASSIGN
};


struct soilprobe_cmd {
	uint8_t addr[3];  // Probe address
	enum soilprobe_cmd_name cmd;  // Command name
	enum soilprobe_cmd_type type;  // Command type
	uint8_t reading_set;  // Reading set to transmit for "Transmit Reading"
	uint8_t arg[16];  // Command argument for "set" commands
	uint8_t argsize;  // The size of the data in "arg"
};


struct soilprobe_resp {
	uint8_t addr[3];  // Probe address
	uint8_t data[101];  // The body of the response (minus CRLF)
	uint8_t datasize;  // The size of the data in "data"
};


/** Initializes the soil probe UART */
void soilprobe_init(void);


/** Sends a soil probe command and parses the response
  *
  * cmd (in) - Pointer to the soilprobe_cmd struct representing the command
  * resp (out) - Pointer to the soilprobe_resp struct to fill with the
  *              response */
void soilprobe_cmd(struct soilprobe_cmd *cmd, struct soilprobe_resp *resp);


/** Gets the serial number of the soil probe
  *
  * Returns the serial number of the soil probe as a uint32_t */
uint32_t soilprobe_get_serial(void);


/** Gets the firmware version of the soil probe
  *
  * versionbuf (out) - Pointer to the buffer (4 bytes) to fill with the firmware
  * 			       string */
void soilprobe_get_firmware_version(uint8_t *versionbuf);


/** Gets the address of the soil probe
  *
  * addrbuf (out) - Pointer to the buffer (3 bytes) to fill with the address */
void soilprobe_get_address(uint8_t *addrbuf);


/** Sets the address of the soil probe
  *
  * serial - Serial number of the probe to set
  * newaddrbuf (in) - Pointer to the buffer (3 bytes) containing the new
  *                   address */
void soilprobe_set_address(uint32_t serial, uint8_t *newaddrbuf);


/** Gets the probe's soil type setting
  *
  * Returns the soil type (see manual) */
uint8_t soilprobe_get_soil_type(void);


/** Sets the soil type of the probe
  *
  * soiltype - The new soil type of the probe (see manual)*/
void soilprobe_set_soil_type(uint8_t soiltype);


/** Tells the probe to take a soil reading and save the results */
void soilprobe_take_reading(void);


/** Tells the probe to transmit a set of readings
  *
  * set - The set of readings for the probe to transmit (see manual)
  * readingsbuf (out) - Buffer of chars representing the readings (variable
  *                     length for each different readings set, though 100
  *                     bytes is a safe estimate) */
void soilprobe_get_reading(uint8_t set, uint8_t *readingsbuf);
