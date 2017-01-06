#ifndef SERIALHANDLER_H
#define SERIALHANDLER_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QByteArray>
#include <QDataStream>

// http://stackoverflow.com/questions/15103599/qt-serial-port-reading-data-consistently


class SerialHandler : public QThread
{
    Q_OBJECT
    ~SerialHandler();
    static SerialHandler* instance();


    bool connectDevice();
    QIODevice *device() const;

private:
    SerialHandler(QObject *parent = 0);
    explicit SerialHandler(QIODevice *d, QObject *parent = 0);
    SerialHandler(QByteArray *a, QIODevice::OpenMode flags, QObject *parent = 0);
    static SerialHandler* createInstance();

    bool m_run;
    QDataStream m_datastream;
    void eventLoop();
    void parsePacket(int size);

public slots:
    void start();
    void stop();
};

#endif // SERIALHANDLER_H
