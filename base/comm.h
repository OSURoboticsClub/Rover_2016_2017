#ifndef SENDPACKET_H
#define SENDPACKET_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void send_packet(uint8_t *data, uint16_t count);
void debugging_info_trigger(void);
void camera_command_trigger(void);

#ifdef __cplusplus
}
#endif

#endif // SENDPACKET_H
