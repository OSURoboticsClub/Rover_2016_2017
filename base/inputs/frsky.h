#ifndef FRSKY_H
#define FRSKY_H
#include <QTime>

#include "abstractcontroller.h"


class FrSky : public AbstractController
{
    Q_OBJECT
public:
    explicit FrSky(QFile *file, QObject *parent = 0);
    ~FrSky();

signals:
    void frSkyModeChange(qint16);

protected:
    void emitAxisChanges(quint8) Q_DECL_OVERRIDE;
    void emitButtonChanges(quint8) Q_DECL_OVERRIDE;
    QTime timeAxis;
    bool axisEnable;
    int _axis_wait_time_ms;

};

#endif // FRSKY_H
