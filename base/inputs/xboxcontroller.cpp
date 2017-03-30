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
    if(axisIndex == 0 || axisIndex == 1) {
        qint16 pan = m_currentState->axes[0];
        qint16 tilt = m_currentState->axes[1];

        sendPanPrimary(pan, tilt);
    }
}

void XboxController::emitButtonChanges(quint8 buttonIndex)
{
    //TODO:: decide actual button
    if (buttonIndex == 5){
        panMode = m_currentState->buttons[5];
    }
}

