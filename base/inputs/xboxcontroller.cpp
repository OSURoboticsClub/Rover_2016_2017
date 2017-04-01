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
<<<<<<< Updated upstream
    if(axisIndex == 1 || axisIndex == 2) {
        //sendDriveMotorPower();
        // send a packet
    }
=======

>>>>>>> Stashed changes
}

void XboxController::emitButtonChanges(int buttonIndex)
{
    // TODO: what are these buttons?
    if(buttonIndex == 1) {
        if(m_currentState->axes[buttonIndex]) {
            sendSelectCamera(1);
        }
    } else if (buttonIndex == 2) {
        if(m_currentState->axes[buttonIndex]) {
            sendSelectCamera(-1);
        }
    }
}

