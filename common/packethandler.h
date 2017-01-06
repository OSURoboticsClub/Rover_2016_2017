#ifndef PACKETHANDLER_H
#define PACKETHANDLER_H

#include <QObject>
#include <QIODevice>
#include <QByteArray>
#include <QDataStream>
#include <QtGlobal>



class PacketHandler : public QObject
{
    Q_OBJECT
    /*
        @datastream: m_datastream
        @types_enum: Types
        @crc: crc
    */
public:
    PacketHandler(QObject *parent = 0);
    explicit PacketHandler(QIODevice *d, QObject *parent = 0);
    PacketHandler(QByteArray *a, QIODevice::OpenMode flags, QObject *parent = 0);
    ~PacketHandler();

    void parsePacket(quint8 size);

    QIODevice *device() const;
    void setDevice(QIODevice *d);
    void unsetDevice();

    enum class Types: quint8 {
        /* @packet_types_header */
    };

signals:
    /* @recieved_signals_header */

public slots:
    /* @write_slots_header */

    /* @read_slots_header */

private:
   Types m_types;
   QDataStream m_datastream;
   quint16 crc(void *data, int size, quint16 initial = 0);
};

#endif // PACKETHANDLER_H
