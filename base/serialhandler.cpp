#include "serialhandler.h"

SerialHandler::SerialHandler(QObject *parent) : QThread(parent)
{
}

SerialHandler::~SerialHandler()
{
    //delete port;
}

void SerialHandler::run()
{
    if(!this->isReady()) {
        qDebug() << "Serial is not set up";
        return;
    }
    else {
        readData();
    }
}

void SerialHandler::setupPort(QString name)
{
    this->portName = name;
    port.setPortName(this->portName);
    if(port.open(QIODevice::ReadOnly)) {
        if(!port.setBaudRate(QSerialPort::Baud57600)) {
            qDebug() << port.errorString();
            return;
        }
        if(!port.setDataBits(QSerialPort::Data8)) {
            qDebug() << port.errorString();
            return;
        }
        if(!port.setParity(QSerialPort::NoParity)) {
            qDebug() << port.errorString();
            return;
        }
        if(!port.setStopBits(QSerialPort::OneStop)) {
            qDebug() << port.errorString();
            return;
        }
        if(!port.setFlowControl(QSerialPort::NoFlowControl)) {
            qDebug() << port.errorString();
            return;
        }
    }
    else {
        qDebug() << port.errorString();
        return;
    }
    this->state = 1;
}

bool SerialHandler::isReady() const
{
    if(this->state > 0)
        return true;
    return false;
}

void SerialHandler::readData()
{
/*
    uint8_t *buffer = (uint8_t *)malloc(0);
    char curChar[1] = {0};
    size_t size = 0;
    if(!port.isReadable())
    {
        qDebug() << "The port is unreadable";
        return;
    }
    do
    {
        if(!port.getChar(curChar))
            continue;
        size++;
        buffer = (uint8_t *)realloc(buffer, size);
        if(*curChar == ESCAPE_BYTE) {
            this->port.getChar(curChar);
            buffer[size-1] = (uint8_t)*curChar;
            continue;
        }
        if(*curChar == STOP_BYTE) {
            size_t size = 0;
            free(buffer);
            uint8_t *buffer = (uint8_t *)malloc(0);
            messageBuffer *packet = (messageBuffer *)malloc(sizeof(messageBuffer *));
            packet->buffer = buffer;
            packet->size = size;

            if(*packet->buffer == BATTERYVOLTAGE_TYPE)
            {
                packets_BatteryVoltage p = decodeBatteryVoltage(packet);

                qDebug() << p.battery_voltage;
            }
            freeMessageBuffer(packet);
            return;
        }
        buffer[size-1] = (uint8_t)*curChar;
    }
    while(port.isReadable());
*/
}
