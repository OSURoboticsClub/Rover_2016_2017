#include "frsky.h"


FrSky::FrSky(QFile *file, QObject *parent)
    : AbstractController(file, parent)
{
}

FrSky::~FrSky()
{

}


void FrSky::emitAxisChanges(int axisIndex){
    if(m_mode == 0){
        if(axisIndex == 0 || axisIndex == 1) { //left and right y - drive motor power
            //sendDriveMotorPower(js::getAxisPosition(m_id, static_cast<js::Axis>(0)), js::getAxisPosition(m_id, static_cast<js::Axis>(1)));
        }
    }
     else if (m_mode == 1) {
        if(axisIndex == 0 || axisIndex == 1 || axisIndex == 2 || axisIndex == 3 || axisIndex == 5){
            double m1 = m_currentState->axes[0];
            double m2 = m_currentState->axes[1];
            double m3 = m_currentState->axes[2];
            double m4 = m_currentState->axes[3];
            double m5 = m_currentState->axes[4];
            sendArmMotorPower(m1, m2, m3, m4, m5);
        }
        else if(axisIndex == 4){
            //armGripper();
        }
    }
}


void FrSky::emitButtonChanges(int buttonIndex){
    if(buttonIndex == 0){ //SF - Pause
        //sendPause();
    }
    else if(buttonIndex == 1){ //SE - change mode
        m_mode = m_currentState->buttons[1];
    }

    if(m_mode == 0){
        if(buttonIndex == 2){ //SG - zero radius mode
            //set zero radius mode
        }
    }
}
