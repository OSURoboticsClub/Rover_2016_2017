#ifndef FRSKY_H
#define FRSKY_H

#include "abstractcontroller.h"


class FrSky : public AbstractController
{
    Q_OBJECT
public:
    explicit FrSky(QFile *file, QObject *parent = 0);
    ~FrSky();

signals:

protected:
    void emitAxisChanges(int);
    void emitButtonChanges(int);

};

#endif // FRSKY_H
