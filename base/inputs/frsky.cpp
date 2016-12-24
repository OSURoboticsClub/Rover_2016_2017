#include "frsky.h"


FrSky::FrSky(int id, QObject *parent)
    : AbstractController(id, parent)
{
    m_priority = 1;
}

FrSky::~FrSky()
{

}
