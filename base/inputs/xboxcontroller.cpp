#include "xboxcontroller.h"


XboxController::XboxController(int id, QObject *parent)
    : AbstractController(id, parent)
{
    // TODO: set priority
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
    return m_currentState->buttons[BUTTON_A];
}

bool XboxController::buttonB()
{
    return m_currentState->buttons[BUTTON_B];
}

bool XboxController::buttonX()
{
    return m_currentState->buttons[BUTTON_X];
}

bool XboxController::buttonY()
{
    return m_currentState->buttons[BUTTON_Y];
}
