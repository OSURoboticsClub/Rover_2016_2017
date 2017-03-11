#include "xboxcontroller.h"


XboxController::XboxController(QFile *file, QObject *parent)
    : AbstractController(file, parent)
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

