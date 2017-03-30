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
    if(axisIndex == 1 || axisIndex == 2 || axisIndex == 3 || axisIndex == 4) {
        qint8 pan = m_currentState->axes[1];
        qint8 tilt = m_currentState->axes[2];
        qint8 pan2 = m_currentState->axes[3];
        qint8 tilt2 = m_currentState->axes[4];
        sendPanTilt(pan, tilt, pan2, tilt2);
        qDebug("XBOX WORKING!");
    }
}

void XboxController::emitButtonChanges(quint8 buttonIndex)
{

}

