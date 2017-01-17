#ifndef CONTROLLERHANDLER_H
#define CONTROLLERHANDLER_H


#include <QObject>
#include <QList>
#include <QSharedPointer>
#include <QThread>
#include <QDebug>

#include "inputs/abstractcontroller.h"
#include "inputs/xboxcontroller.h"
#include "inputs/frsky.h"


/*
 * Pointer to any subclass of AbstractController.
 */
typedef QSharedPointer<AbstractController> ControllerPointer;

/*
 * Manages the various controllers connected to the computer. Individual
 * controllers are identified by their product id, and the class corresponding
 * to the controller is instantiated. Following, controllers can be assigned to
 * specific roles in relation to the rover. For example, one controller can be
 * responsible for providing the driving direction to the rover. Assignment of
 * controller roles is automatic, with the most important capabilities assigned
 * to the controllers with the highest priority.
 *
 * Once controllers are connected and assigned, an event loop can query for
 * changes in controller state. If the change is mapped to an action, the
 * class will trigger a packet transmission to the rover. For example, moving
 * the driving axis on the controller responsible for rover driving will cause
 * a packet transmission to the rover containing the motor power requested by
 * the user.
 *
 * Since this class is a subclass of QThread, the event loop as well as any
 * necessary setup can be invoked by calling the run() method. Likewise, calling
 * the quit() slot will exit the event loop.
 */

class ControllerHandler : public QThread
{
    Q_OBJECT
public:
    explicit ControllerHandler(QObject *parent = 0);
    ~ControllerHandler();
    /*
     * Counts the total number of controllers connected to the system.
     */
    int controllerCount();
    /*
     * Rebuilds the list of usable controllers. Each connected controller is
     * identified by an id, and the corrsponding controller class, if
     * available, is instantiated. If a controller's product id does not
     * correspond to any controller class, then the controller is not added to
     * the list.
     */
    void resetControllers();
    /*
     * Identifies controllers and preforms any required setup before entering
     * the event loop. This is the only required method to use when starting
     * controller input handling.
     */
    void run() Q_DECL_OVERRIDE;

public slots:
    void stop();

private:
    void run();
    void eventLoop();
    void connectControllers();
    void connectDriveController(ControllerPointer controller);

    /*
     * The list containing the controller class instances that this class uses.
     */
    QList<ControllerPointer> *m_controllers;
    bool m_stop;
    /* The raw number of controllers that have been identified on the system. */
    int m_controllerCount;
    /* The number of controllers that the class can recognize (i.e. have a
     * product id that corresponds to a controller class).
     */
    int m_usableControllerCount;
    /*
     * The maximum amount of controllers that the class can use. This value
     * corresponds to the number of roles that can be assigned to a controller.
     */
    int m_maxUsableControllers = 1;

private slots:
    void sendDriveMotorPower(double left, double right);
    // value is actually not used
    // TODO: maybe better way to do this?
    // TODO: connect other controllers

};

#endif // CONTROLLERHANDLER_H
