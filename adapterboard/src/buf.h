/* OSU Robotics Club Rover 2016-2017
 * Radio adapter board firmware
 *
 * buf.h - Global input/output circular buffers
 * Author: Aaron Cohen
 */

#include <stdint.h>

/* Same as values from miniboard firmware */
#define BUF_IN_SIZE 10
#define BUF_OUT_SIZE 100

enum Buf
{
	BUF_IN, /* Buffer for data coming IN to the miniboard */
	BUF_OUT, /* Buffer for data coming OUT of the miniboard */
};

void buf_init(void);

void buf_add(enum Buf sel, uint8_t data);

uint8_t buf_size(enum Buf sel);

uint8_t buf_peek_first(enum Buf sel);

uint8_t buf_at(enum Buf sel, uint8_t ind);

void buf_pop_first(enum Buf sel);

uint8_t buf_packet_check(enum Buf sel);
