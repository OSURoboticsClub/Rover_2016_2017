#ifndef SERIALHANDLER_H
#define SERIALHANDLER_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QByteArray>

// http://stackoverflow.com/questions/15103599/qt-serial-port-reading-data-consistently


class SerialHandler : public QThread
{
    Q_OBJECT
public:

    SerialHandler(QObject *parent = 0);
    explicit SerialHandler(QIODevice *d, QObject *parent = 0);
    SerialHandler(QByteArray *a, QIODevice::OpenMode flags, QObject *parent = 0);
    ~SerialHandler();

    QIODevice *device() const;
    void setDevice(QIODevice *d);
    void unsetDevice();

private:
    bool m_run;
    void run();

public slots:
    void readData();
    void stopThread();


};

#endif // SERIALHANDLER_H
