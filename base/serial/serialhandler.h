#ifndef SERIALHANDLER_H
#define SERIALHANDLER_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QtSerialPort/QSerialPort>


#include "packets.h"
// http://stackoverflow.com/questions/15103599/qt-serial-port-reading-data-consistently


class SerialHandler : public QThread
{
    Q_OBJECT
public:
    ~SerialHandler();
    static SerialHandler* instance();

    void run() Q_DECL_OVERRIDE;
    bool connectDevice();

    void setDevice(QIODevice *d);
    QIODevice *device() const;

signals:


public slots:
    void stop();

private:
    Packets m_packets;
    SerialHandler(QObject *parent = 0);
    static SerialHandler* createInstance();

    quint16 crc(void *data, int size, quint16 initial);
    bool m_run = true;
    void eventLoop();

};

#endif // SERIALHANDLER_H
