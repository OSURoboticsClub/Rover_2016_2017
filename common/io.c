
#include <stdlib.h>

#include "io.h"

uint8_t *encodePacket(void *message, uint8_t type)
{
    pb_field_t *fieldptr;
    size_t numfields;
    size_t maxsize;
    messageFields(type, &fieldptr, &numfields, &maxsize);
    maxsize += 2;
    pb_field_t fields[numfields];
    memcpy(fields, fieldptr, sizeof(pb_field_t) * numfields);

    uint8_t *buffer = (uint8_t *)calloc(maxsize, 1);
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, maxsize);
    pb_write(&stream, &type, 1);
    pb_encode(&stream, fields, message);
    uint8_t terminator = 0;
    pb_write(&stream, &terminator, 1);
    return(buffer);
}

// TODO
uint8_t *escapePacket(uint8_t *string)
{
    return string;
}

uint8_t *unescapePacket(uint8_t *string)
{
    return string;
}
