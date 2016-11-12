#include <QMainWindow>
#include <QApplication>
#include "sendpacket.h"
#include "serialhandler.h"

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
