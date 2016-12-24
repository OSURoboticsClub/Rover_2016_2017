#include "abstractcontroller.h"


AbstractController::AbstractController(int id, QObject *parent) : QObject(parent)
{
    this->id = id;
    currentState = new JoystickState();
}

AbstractController::~AbstractController()
{
    delete currentState;
}
