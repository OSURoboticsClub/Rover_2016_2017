#ifndef CONTROLHANDLER_H
#define CONTROLHANDLER_H

#include <algorithm>

#include <QObject>
#include <QList>

#include "frsky.h"
#include "abstractcontroller.h"
#include "xboxcontroller.h"
#include "serialhandler.h"
#include "commgen.h"
#include "comm.h"

typedef QSharedPointer<AbstractController> ControllerPointer;


class ControlHandler : public QObject
{
    Q_OBJECT
public:
    explicit ControlHandler(QObject *parent = 0);
    ~ControlHandler();
    // TODO: controllerCount(); (renaming)
    int getControllerCount();
public slots:
    void start();
private:
    QList<ControllerPointer> *controllers;
    int controllerCount;
    int maxUsableControllers = 1;
    void eventLoop();
    void resetControllers();
    void connectControllers();
    void connectDriveController(ControllerPointer controller);
    // TODO: connect other controllers

};

#endif // CONTROLHANDLER_H
