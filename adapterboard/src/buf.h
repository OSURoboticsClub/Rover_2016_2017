/* OSU Robotics Club Rover 2016-2017
 * Radio adapter board firmware
 *
 * buf.h - Global input/output circular buffers
 * Author: Aaron Cohen
 */

#define BUF_SIZE 20

enum Buf
{
	BUF_IN,
	BUF_OUT,
};

void buf_init(void);

void buf_add(enum Buf sel, uint8_t data);
uint8_t buf_size(enum Buf sel);
uint8_t buf_peek_first(enum Buf sel);
void buf_pop_first(enum Buf sel);
