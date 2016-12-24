#include "xboxcontroller.h"


XboxController::XboxController(int id, QObject *parent) : AbstractController(id, parent)
{

}

XboxController::~XboxController()
{

}

void XboxController::emitAxisChanges(int axisIndex, double value)
{
    AbstractController::emitAxisChanges(axisIndex, value);
}

void XboxController::emitButtonChanges(int buttonIndex, bool value)
{
    AbstractController::emitButtonChanges(buttonIndex, value);
}

bool XboxController::buttonA()
{
    //return currentState->axes[static_cast<int>(buttonMappings::A)];
}

bool XboxController::buttonB()
{
    //return currentState->axes[static_cast<int>(buttonMappings::B)];
}

bool XboxController::buttonX()
{
    //return currentState->axes[static_cast<int>(buttonMappings::X)];
}

bool XboxController::buttonY()
{
//    return currentState->axes[static_cast<int>(buttonMappings::Y)];
}
