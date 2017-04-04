/**
 *The SerialHandler class will handle serial interfacing with the radio that
 * will transmit data to the rover. This class will contain methods for sending
 * packets to the rover. It will also emit read packets from the serial and
 * emit signals when a packet is recieved.
 **/

#include "serial/serialhandler.h"

#include <QSerialPortInfo>
#include <QBuffer>
#include <QDebug>

#include "serial/singleton.h"


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
    connectDevice();
    if(m_packets->device() == NULL) {
        QByteArray *buffer = new QByteArray();
        setBuffer(buffer);
    }

    eventLoop();
    m_run = true;
}

void SerialHandler::eventLoop()
{
    emit changeButtonColor("#169d06", true);
    while (m_run){
        if(m_packets->device()->bytesAvailable() >= 2) {

            quint8 start, size;
            *m_packets->datastream() >> start;
            if(start != 0x01) continue;
            qDebug() << "Successfully read start byte.";
            *m_packets->datastream() >> size;
            if(size < 3) continue;
            qDebug() << "Read Packet size of: " << size;
            while(m_packets->device()->bytesAvailable() < size) {
                msleep(50);
            }
            m_packets->parsePacket(size);
            qDebug() << m_packets;
            qDebug() << "--------------------";
        }
        msleep(100);
    }
    qDebug() << "exiting serial read";
    emit changeButtonColor("#9d0606", false);
}

void SerialHandler::stop() {
    m_run = false;
}


void SerialHandler::connectDevice()
{
    QList<QSerialPortInfo> serialPorts = QSerialPortInfo::availablePorts();
    if(!serialPorts.isEmpty()){
            qDebug() << serialPorts[1].portName();

            qDebug() << "identifyed serial";
            QSerialPort *serial = new QSerialPort(serialPorts[1]);
            if(!serial->open(QIODevice::ReadWrite)) {
                qDebug() << tr("error %1").arg(serial->error());
            }
            serial->setBaudRate(QSerialPort::Baud9600);
            serial->setDataBits(QSerialPort::Data8);
            serial->setParity(QSerialPort::NoParity);
            serial->setStopBits(QSerialPort::OneStop);
            serial->setFlowControl(QSerialPort::NoFlowControl);
            setDevice(serial);
        //}
    }
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

void SerialHandler::queryStatus()
{
    qDebug() << "Pulling Reads";
    p()->readBatteryVoltage();
    p()->readDriveMotorPower();
    p()->readSwerveDriveState();
    p()->readArmMotors();
    p()->readPotentiometers();
    p()->readSelectCamera();
    //p()->readCallsign();
    p()->readGpsPosition();
    p()->readGpsTrack();
    p()->readMagnetometer();
    p()->readAccelerometer();
    p()->readGyroscope();
    p()->readCompassHeading();
    p()->readGpioDirection();
    p()->readGpioOutValue();
    p()->readGpioReadState();
    //p()->readDebuggingInfo();
    //p()->readBuildInfo();
    qDebug() << "Done Pulling";
}
