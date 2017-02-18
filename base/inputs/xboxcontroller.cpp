#include "xboxcontroller.h"


XboxController::XboxController(int id, QObject *parent)
    : AbstractController(id, parent)
{
}

XboxController::~XboxController()
{

}

void XboxController::emitAxisChanges(int axisIndex)
{
    if(axisIndex == 1) {
        // send a packet
    }
}

void XboxController::emitButtonChanges(int buttonIndex)
{

}

