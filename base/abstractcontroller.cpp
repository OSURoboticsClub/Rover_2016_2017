#include "abstractcontroller.h"

<<<<<<< HEAD
AbstractController::AbstractController(int id, QObject *parent) : QObject(parent)
=======
AbstractController::AbstractController(QString port, QObject *parent) : QObject(parent)
{

}

AbstractController::~AbstractController()
>>>>>>> 91ddf59f49015932795badb5f5e9d4fc164fcab2
{
    this->id = id;
    currentState = new JoystickState();
}

AbstractController::~AbstractController()
{
    delete currentState;
}
