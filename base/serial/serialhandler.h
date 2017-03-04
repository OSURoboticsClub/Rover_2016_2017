#ifndef SERIALHANDLER_H
#define SERIALHANDLER_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QIODevice>
#include <QtSerialPort/QSerialPort>


#include <packets.h>
// http://stackoverflow.com/questions/15103599/qt-serial-port-reading-data-consistently


class SerialHandler : public QThread
{
    Q_OBJECT
public:
    ~SerialHandler();
    static SerialHandler* instance();

    void run() Q_DECL_OVERRIDE;

    SerialHandler(QObject *parent = 0);
    void setDevice(QIODevice *d);
    void setBuffer(QByteArray *a);
    QIODevice *device();

    Packets *p() const {return m_packets;}

signals:
    void changeButtonColor(QString, bool);

public slots:
    void stop();
    void connectDevice();

private:
    Packets *m_packets;

    static SerialHandler* createInstance();

    bool m_run = true;
    void eventLoop();

};

#endif // SERIALHANDLER_H
