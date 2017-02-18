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
    if(axisIndex == 1 || axisIndex == 2) {
        //sendDriveMotorPower();
        // send a packet
    }
}

void XboxController::emitButtonChanges(int buttonIndex)
{

}

