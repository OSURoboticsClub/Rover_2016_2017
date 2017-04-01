#include "xboxcontroller.h"


XboxController::XboxController(QFile *file, QObject *parent)
    : AbstractController(file, parent)
{
}

XboxController::~XboxController()
{

}

void XboxController::emitAxisChanges(quint8 axisIndex)
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

void XboxController::emitButtonChanges(quint8 buttonIndex)
{
    //TODO:: decide actual button
    //if button is pushed toggle pan mode
    if (buttonIndex == 3 && m_currentState->buttons[3]){
        panMode = !panMode;
        //emit panModeChange(panMode);
    }
}

