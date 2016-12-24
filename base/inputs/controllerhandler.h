#ifndef CONTROLLERHANDLER_H
#define CONTROLLERHANDLER_H

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


class ControllerHandler : public QObject
{
    Q_OBJECT
public:
    explicit ControllerHandler(QObject *parent = 0);
    ~ControllerHandler();
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

#endif // CONTROLLERHANDLER_H
