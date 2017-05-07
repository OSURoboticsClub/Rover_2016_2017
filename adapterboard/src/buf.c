/* OSU Robotics Club Rover 2016-2017
 * Radio adapter board firmware
 *
 * buf.c - Global input/output circular buffers
 * Author: Aaron Cohen
 */

#include <util/atomic.h>
#include <stdint.h>
#include "buf.h"

#define BUF_SEL(sel) ((sel == BUF_IN) ? &buf_in : &buf_out)
#define min(x1, x2) ((x1 < x2) ? x1 : x2)


struct circ_buf
{
	uint8_t buf[BUF_SIZE];
	uint8_t first;
	uint8_t size;
} buf_in, buf_out;


void buf_init(void)
{
	buf_in.first = buf_in.size = 0;
	buf_out.first = buf_out.size = 0;
}


void buf_add(enum Buf sel, uint8_t data)
{
	struct circ_buf *buf = BUF_SEL(sel);

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		buf->buf[(buf->first + buf->size) % BUF_SIZE] = data;
		buf->size += 1;

		if (buf->size > BUF_SIZE)
		{
			buf->first += 1;
			buf->size -= 1;
		}
	}
}


uint8_t buf_size(enum Buf sel)
{
	struct circ_buf *buf = BUF_SEL(sel);

	return buf->size;
}


uint8_t buf_peek_first(enum Buf sel)
{
	struct circ_buf *buf = BUF_SEL(sel);

	if (buf->size > 0)
	{
		return buf->buf[buf->first + buf->size - 1];
	}
	else
	{
		return 0xFF;
	}
}


void buf_pop_first(enum Buf sel)
{
	struct circ_buf *buf = BUF_SEL(sel);

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		if (buf->size > 0)
		{
			buf->first += 1;
			buf->size -= 1;
		}
	}
}
