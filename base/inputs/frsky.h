#ifndef FRSKY_H
#define FRSKY_H

#include "abstractcontroller.h"


class FrSky : public AbstractController
{
    Q_OBJECT
public:
    explicit FrSky(int id, QObject *parent = 0);
    ~FrSky();

signals:

protected:


};

#endif // FRSKY_H
