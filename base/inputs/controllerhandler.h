#ifndef CONTROLLERHANDLER_H
#define CONTROLLERHANDLER_H


#include <QObject>
#include <QList>
#include <QSharedPointer>
#include <QThread>


#include "inputs/abstractcontroller.h"
#include "inputs/xboxcontroller.h"
#include "inputs/frsky.h"


typedef QSharedPointer<AbstractController> ControllerPointer;


class ControllerHandler : public QThread
{
    Q_OBJECT
public:
    explicit ControllerHandler(QObject *parent = 0);
    ~ControllerHandler();
    int controllerCount();
    void resetControllers();
    void run() Q_DECL_OVERRIDE;

public slots:
    void quit();
private:

    void eventLoop();
    void connectControllers();
    void connectDriveController(ControllerPointer controller);

    QList<ControllerPointer> *m_controllers;
    bool m_stop;
    int m_controllerCount;
    int m_usableControllerCount;
    int m_maxUsableControllers = 1;

private slots:
    void sendDriveMotorPower(double left, double right);
    // value is actually not used
    // TODO: maybe better way to do this?
    // TODO: connect other controllers

};

#endif // CONTROLLERHANDLER_H
