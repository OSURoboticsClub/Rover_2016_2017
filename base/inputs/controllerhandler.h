#ifndef CONTROLLERHANDLER_H
#define CONTROLLERHANDLER_H


#include <QObject>
#include <QList>
#include <QSharedPointer>

#include "inputs/abstractcontroller.h"
#include "inputs/xboxcontroller.h"
#include "inputs/frsky.h"

#include "commgen.h"

typedef QSharedPointer<AbstractController> ControllerPointer;


class ControllerHandler : public QObject
{
    Q_OBJECT
public:
    explicit ControllerHandler(QObject *parent = 0);
    ~ControllerHandler();
    int controllerCount();
public slots:
    void start();
    void stop();
private:
    void eventLoop();
    void resetControllers();
    void connectControllers();
    void connectDriveController(ControllerPointer controller);

    QList<ControllerPointer> *m_controllers;
    bool m_stop;
    int m_controllerCount;
    int m_maxUsableControllers = 1;

private slots:
    void sendDriveMotorPower(double left, double right);
    // value is actually not used
    // TODO: maybe better way to do this?
    // TODO: connect other controllers

};

#endif // CONTROLLERHANDLER_H
