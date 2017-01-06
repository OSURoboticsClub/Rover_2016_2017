#ifndef SERIALHANDLER_H
#define SERIALHANDLER_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QByteArray>
#include <QDataStream>
#include <QtGlobal>

// http://stackoverflow.com/questions/15103599/qt-serial-port-reading-data-consistently


class SerialHandler : public QThread
{
    Q_OBJECT
    /*
        @datastream: m_datastream
        @types_enum: Types
        @crc: crc
    */
public:
    ~SerialHandler();
    static SerialHandler* instance();

    void run() Q_DECL_OVERRIDE;
    bool connectDevice();

    void setDevice(QIODevice *d);
    QIODevice *device() const;

signals:
    /* @recieved_signals_header */

public slots:
    void start();
    void stop();
    /* @write_slots_header */

    /* @read_slots_header */

private:
    SerialHandler(QObject *parent = 0);
    /*
    explicit SerialHandler(QIODevice *d, QObject *parent = 0);
    SerialHandler(QByteArray *a, QIODevice::OpenMode flags, QObject *parent = 0);
    */
    static SerialHandler* createInstance();

    enum class Types: quint8 {
        /* @packet_types_header */
    };
    Types m_types;
    quint16 crc(void *data, int size, quint16 initial);
    QDataStream m_datastream;
    bool m_run = true;
    void eventLoop();
    void parsePacket(quint8 size);

};

#endif // SERIALHANDLER_H
