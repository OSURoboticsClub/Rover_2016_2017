/* OSU Robotics Club Rover 2017
 * Science Board Firmware
 *
 * soilprobe.h - Soil probe module
 * Author(s): Aaron Cohen
 */

#include <stdint.h>


enum soilprobe_cmd_name
{
	CMD_GET_SERIAL,
	CMD_GET_VERSION,
	CMD_GET_ADDRESS,
	CMD_SET_ADDRESS,
	CMD_GET_SOIL_TYPE,
	CMD_SET_SOIL_TYPE,
	CMD_TAKE_READING,
	CMD_GET_READING,
};


struct soilprobe_cmd
{
	enum soilprobe_cmd_name cmd;  /* Command name */
	uint8_t addr[3];              /* Probe address */
	uint8_t arg[11];              /* Command argument for "set" commands */

	uint8_t argsize;              /* The size of the data in "arg" */
};


struct soilprobe_resp
{
	uint8_t addr[3];    /* Probe address */
	uint8_t data[100];  /* The body of the response (minus CRLF) */

	uint8_t datasize;   /* The size of the data in "data" */
};


/* Initializes the soil probe UART */
void soilprobe_init(void);


/* Sends a soil probe command and parses the response */
void soilprobe_cmd(struct soilprobe_cmd *cmd, struct soilprobe_resp *resp);
