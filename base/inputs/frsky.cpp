#include "frsky.h"
#include <QDebug>

FrSky::FrSky(QFile *file, QObject *parent)
    : AbstractController(file, parent)
{
}

FrSky::~FrSky()
{

}


void FrSky::emitAxisChanges(quint8 axisIndex){



    if(m_mode == 0){
        if(axisIndex == 0 || axisIndex == 1) { //left and right y - drive motor power
            qint16 l = m_currentState->axes[0];
            qint16 r = m_currentState->axes[1];
            sendDriveMotorPower(l, r);
        }
    }
     else if (m_mode == 1) {
        if(axisIndex == 0 || axisIndex == 1 || axisIndex == 2 || axisIndex == 3 || axisIndex == 5){
            qint16 m1 = m_currentState->axes[0];
            qint16 m2 = m_currentState->axes[1];
            qint16 m3 = m_currentState->axes[2];
            qint16 m4 = m_currentState->axes[3];
            qint16 m5 = m_currentState->axes[4];
            sendArmMotorPower(m1, m2, m3, m4, m5);
        }

        else if(axisIndex == 4){
            //armGripper();
        }
    }
}


void FrSky::emitButtonChanges(quint8 buttonIndex){
    if(buttonIndex == 0){ //SF - Pause
        //sendPause();
    }
    else if(buttonIndex == 4){ //SE - change mode
        m_mode = m_currentState->buttons[4];

    }

    if(m_mode == 0){
        if(buttonIndex == 2){ //SG - zero radius mode
            //set zero radius mode
        }
    }
}
