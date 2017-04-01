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
    if((axisIndex == 0 || axisIndex == 1) && panMode) {
        quint16 pan = m_currentState->axes[0];
        quint16 tilt = m_currentState->axes[1];

        sendPanPrimary(pan, tilt);
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

