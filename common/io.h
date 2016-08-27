/*
  Main header for communication between rover and base station.
*/

#ifndef IO_H
#define IO_H

#include <pb_encode.h>

#include "packets.pb.h"
#include "io_gen.h"

#ifdef __cplusplus
extern "C" {
#endif

uint8_t *encodePacket(void *message, uint8_t type);
uint8_t *escapePacket(uint8_t *message);
uint8_t *unescapePacket(uint8_t *string);

#ifdef __cplusplus
}
#endif // extern "C"

#endif // IO_H

