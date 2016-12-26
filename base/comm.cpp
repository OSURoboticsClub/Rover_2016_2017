#include "comm.h"
#include "serialhandler.h"

/**
 * The send_packet writes count bytes in data to the serial stream. It calls a
 * global instance of the SerialHandler class for writing data.
 **/
void send_packet(uint8_t *data, uint16_t count)
{
    // TOOO: should be something like SerialHandler::instance().write() maybe?
    serial.write(data, count);
}

void debugging_info_trigger()
{

}


void camera_command_trigger()
{

}

void callsign_trigger()
{

}

void build_info_trigger()
{

}
