/**
 *The SerialHandler class will handle serial interfacing with the radio that
 * will transmit data to the rover. This class will contain methods for sending
 * packets to the rover. It will also emit read packets from the serial and
 * emit signals when a packet is recieved.
 **/


#include "serial/serialhandler.h"
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
    qDebug() << "starting serial read";
    eventLoop();
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


void SerialHandler::parsePacket(int size)
{

}

void SerialHandler::stop() {
    m_run = false;
}
