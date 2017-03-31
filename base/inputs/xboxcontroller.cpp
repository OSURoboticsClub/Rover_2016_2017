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
    if(axisIndex == 1) {
        if(m_currentState->axes[axisIndex] > 32768) {
            sendSelectCamera(1);
        }
    } else if (axisIndex == 2) {
        if(m_currentState->axes[axisIndex] > 32768) {
            sendSelectCamera(-1);
        }
    }

}

void XboxController::emitButtonChanges(int buttonIndex)
{

}

