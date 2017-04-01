
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

    //if (axisEnable) {
    //axisEnable = false;

    if(m_mode == 0 && !m_swerveControlInto->lock && !m_swerveControlOutof->lock){
        if(axisIndex == 0 || axisIndex == 1) { //left and right y - drive motor power
            qint16 l = m_currentState->axes[0];
            qint16 r = m_currentState->axes[1];
            sendDriveMotorPower(l, r);
        }
    }
    else if (m_mode == 1) {
        if(axisIndex == 0 || axisIndex == 1 || axisIndex == 2 || axisIndex == 3 || axisIndex == 6){
            qint16 m1 = m_currentState->axes[0];
            qint16 m2 = m_currentState->axes[1];
            qint16 m3 = m_currentState->axes[2];
            qint16 m4 = m_currentState->axes[3];
            qint16 m5 = m_currentState->axes[6];
            sendArmMotorPower(m1, m2, m3, m4, m5);
        }

        else if(axisIndex == 5){//left pot
            //armGripper();
        }

    }
    if (axisIndex == 3){

    }


}


void FrSky::emitButtonChanges(quint8 buttonIndex){



    if(buttonIndex == 5){ //SF - Pause
        sendPauseState(m_currentState->buttons[5]);
    }
    else if(buttonIndex == 4){ //SE - change mode
        m_mode = m_currentState->buttons[4];
        //emit frSkyModeChange(m_mode);


    }

    if(m_mode == 0){
        if(buttonIndex == 6){ //SG - zero radius mode straight/turn
            sendSwerveDriveState(m_currentState->buttons[6]);
        }
        if(buttonIndex == 3){//SD - zero radius mode on/off
            //out of swerve
            if(m_currentState->buttons[3]){
                sendSwerveDriveState(m_currentState->buttons[3]);
                sendSwerveMotorPower(1);
            }
            //into swerve
            else if (!m_currentState->buttons[3]){
                sendSwerveDriveState(m_currentState->buttons[3]);
                sendSwerveMotorPower(0);
            }
        }
    }
}
