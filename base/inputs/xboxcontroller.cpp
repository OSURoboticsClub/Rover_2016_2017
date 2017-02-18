#include "xboxcontroller.h"


XboxController::XboxController(int id, QObject *parent)
    : AbstractController(id, parent)
{
    // TODO: set priority
}

XboxController::~XboxController()
{

}

void XboxController::emitAxisChanges(int axisIndex)
{
}

void XboxController::emitButtonChanges(int buttonIndex)
{

}

