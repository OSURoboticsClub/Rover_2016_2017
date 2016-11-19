#include <QMainWindow>
#include <QApplication>
#include "comm.h"
#include "serialhandler.h"

/**
 * The send_packet writes count bytes in data to the serial stream. It calls a
 * global instance of the SerialHandler class for writing data.
 **/
void send_packet(uint8_t *data, uint16_t count)
{
    serial.write(data, count);
}

void debugging_info_trigger(void)
{

}


void camera_command_trigger(void)
{

}
