#ifndef PACKETS_H
#define PACKETS_H

#include <QObject>
#include <QIODevice>
#include <QByteArray>
#include <QDataStream>
#include <QtGlobal>



class Packets : public QObject
{
    Q_OBJECT
    /*
        @datastream: m_datastream
        @types_enum: Types
        @crc: crc
    */
public:
    Packets(QObject *parent = 0);
    explicit Packets(QIODevice *d, QObject *parent = 0);
    Packets(QByteArray *a, QIODevice::OpenMode flags, QObject *parent = 0);
    ~Packets();

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

    /* read_slots_header */

private:
   Types m_types;
   QDataStream m_datastream;
   quint16 crc(void *data, int size, quint16 initial);
};

#endif // PACKETS_H
