/**
 *The SerialHandler class will handle serial interfacing with the radio that
 * will transmit data to the rover. This class will contain methods for sending
 * packets to the rover. It will also emit read packets from the serial and
 * emit signals when a packet is recieved.
 **/


#include "serial/serialhandler.h"
#include "singleton.h"
#include <QSerialPortInfo>


SerialHandler* SerialHandler::createInstance()
{
    return new SerialHandler();
}

SerialHandler* SerialHandler::instance()
{
    return Singleton<SerialHandler>::instance(SerialHandler::createInstance);
}

SerialHandler::~SerialHandler()
{
}

void SerialHandler::start()
{
    run();
}

void SerialHandler::run()
{
    qDebug() << "starting serial read";
    eventLoop();
}

void SerialHandler::eventLoop()
{
    while (m_run){
        qDebug() << "Reading Data";
        sleep(2);
        qDebug() << "------------";
        sleep(2);
    }
    qDebug() << "exciting serial read";
}

void SerialHandler::stop() {
    m_run = false;
}


bool SerialHandler::connectDevice()
{
    QList<QSerialPortInfo> serialPorts = QSerialPortInfo::availablePorts();
    for(int i = 0; i < serialPorts.size(); i++) {
        if(serialPorts[i].serialNumber() == "serial string") {
            setDevice(new QSerialPort(serialPorts[i]));
            return true;
        }
    }
    return false;
}

void SerialHandler::setDevice(QIODevice *d)
{
    m_datastream.setDevice(d);
}

SerialHandler::SerialHandler(QObject *parent) : QThread(parent)
{
    m_run = true;
}
/*
SerialHandler::SerialHandler(QIODevice *d, QObject *parent) :
    QThread(parent),
    m_datastream(d)
{
}

SerialHandler::SerialHandler(QByteArray *a, QIODevice::OpenMode flags, QObject *parent) :
    QThread(parent),
    m_datastream(a, flags)
{
}
*/

void SerialHandler::parsePacket(quint8 size)
{
    /* datastream head at CRC */
    /* @parse_packets */
}

quint16 SerialHandler::crc(void *data, int size, quint16 initial)
{
    quint16 remainder = initial;
    quint8 *bytes = reinterpret_cast<quint8 *>(data);
    for (int i = 0; i < size; i++){
        remainder ^= bytes[i] << 8;
        for (int bit = 8; bit > 0; bit--){
            if (remainder & 0x8000){
                remainder = (remainder << 1) ^ 0x1021;
            } else {
                remainder = (remainder << 1);
            }
        }
    }
    return remainder;
}

/* @write_slots_source */


/* @read_slots_source */


