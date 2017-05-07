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
	uint8_t *buf;
	uint8_t first;
	uint8_t size;
	uint8_t cap;
} buf_in, buf_out;


uint8_t raw_buf_in[BUF_IN_SIZE];
uint8_t raw_buf_out[BUF_OUT_SIZE];


void buf_init(void)
{
	buf_in.buf = raw_buf_in;
	buf_in.first = buf_in.size = 0;
	buf_in.cap = BUF_IN_SIZE;

	buf_out.buf = raw_buf_out;
	buf_out.first = buf_out.size = 0;
	buf_out.cap = BUF_OUT_SIZE;
}


void buf_add(enum Buf sel, uint8_t data)
{
	struct circ_buf *buf = BUF_SEL(sel);

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		buf->buf[(buf->first + buf->size) % buf->cap] = data;
		buf->size += 1;

		if (buf->size > buf->cap)
		{
			buf->first = (buf->first + 1) % buf->cap;
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
		return buf->buf[buf->first];
	}
	else
	{
		return 0xFF;
	}
}


uint8_t buf_at(enum Buf sel, uint8_t ind)
{
	struct circ_buf *buf = BUF_SEL(sel);

	if (ind < buf->size)
	{
		return buf->buf[(buf->first + ind) % buf->cap];
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
			buf->first = (buf->first + 1) % buf->cap;
			buf->size -= 1;
		}
	}
}


uint8_t buf_packet_check(enum Buf sel)
{
	struct circ_buf *buf = BUF_SEL(sel);

	/* TODO: CRC checking */
	if (buf->size >= 2 && buf_at(sel, 0) == 0x01 && (buf_at(sel, 1) == buf->size || buf->size >= buf->cap))
		return 1;
	else
		return 0;
}
