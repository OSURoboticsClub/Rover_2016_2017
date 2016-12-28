
#include "packethandler.h"

PacketHandler::PacketHandler(QObject *parent = 0)
    m_datastream()
{
}

PacketHandler::PacketHandler(QIODevice *d, QObject *parent = 0) :
    m_datastream(d)
{
}

PacketHandler(QByteArray *a, QIODevice::OpenMode flags, QObject *parent = 0) :
    m_datastream(d, flags)
{
}

PacketHandler::~PacketHandler()
{
    delete m_datastream;
}

/* start and end bytes, as well as escape bytes removed */
PacketHandler::parsePacket(const QByteArray &packet)
{
    QDataStream *stream = new QDataStream(packet)
    /* @parse_read_cases */

    /* @parse_write_cases */

        // error handling
}

/* @write_slots_source */

/* @read_slots_source */
