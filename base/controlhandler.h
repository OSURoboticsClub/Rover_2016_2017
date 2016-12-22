#ifndef CONTROLHANDLER_H
#define CONTROLHANDLER_H

#include <QObject>

#include "frsky.h"
#include "serialhandler.h"
#include "commgen.h"
#include "comm.h"

class ControlHandler : public QObject
{
    Q_OBJECT
public:
    explicit ControlHandler(QString mainControllerPort, QObject *parent = 0);
    void setMainControllerPort(QString port);

private:
    FrSky *mainController;
    void connectMainController();

signals:

public slots:
    void sendDriveMotorPower(int left_drive, int right_drive);
};

#endif // CONTROLHANDLER_H
