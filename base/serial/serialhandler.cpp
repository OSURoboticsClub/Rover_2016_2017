/**
 *The SerialHandler class will handle serial interfacing with the radio that
 * will transmit data to the rover. This class will contain methods for sending
 * packets to the rover. It will also emit read packets from the serial and
 * emit signals when a packet is recieved.
 **/


#include "serial/serialhandler.h"
#include "serial/singleton.h"
#include <QSerialPortInfo>
#include <QBuffer>


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
    delete m_packets;
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
    m_packets->setDevice(d);
}

void SerialHandler::setBuffer(QByteArray *a)
{
    QBuffer *buffer = new QBuffer(a);
    m_packets->setDevice(buffer);
}

QIODevice *SerialHandler::device()
{
    return m_packets->device();
}

SerialHandler::SerialHandler(QObject *parent)
    : QThread(parent),
      m_packets(new Packets())
{
    m_run = true;
}
