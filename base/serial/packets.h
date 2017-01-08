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
    explicit Packets(QObject *parent = 0);
    ~Packets();

    void parsePacket(quint8 size);

    QIODevice *device();
    void setDevice(QIODevice *d);
    
    QDataStream *datastream() {return &m_datastream;}

    quint16 crc(void *data, int size, quint16 initial);
    enum class Types: quint8 {
        /* @packet_types_header */
    };

signals:
    /* @recieved_signals_header */


public slots:
    /* @write_slots_header */

    /* @read_slots_header */

private:
    QDataStream m_datastream;
};

#endif // PACKETS_H
