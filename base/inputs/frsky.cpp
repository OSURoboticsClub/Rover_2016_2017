#include "frsky.h"


FrSky::FrSky(QFile *file, QObject *parent)
    : AbstractController(file, parent)
{
}

FrSky::~FrSky()
{

}


void FrSky::emitAxisChanges(int axisIndex){
    if(mode == 0){
        if(axisIndex == 0 || axisIndex == 1) { //left and right y - drive motor power
            //sendDriveMotorPower(js::getAxisPosition(m_id, static_cast<js::Axis>(0)), js::getAxisPosition(m_id, static_cast<js::Axis>(1)));
        }
    }
     else if (mode == 1) {
        if(axisIndex == 0 || axisIndex == 1 || axisIndex == 2 || axisIndex == 3 || axisIndex == 5){
            double m1 = js::getAxisPosition(m_id, static_cast<js::Axis>(0));
            double m2 = js::getAxisPosition(m_id, static_cast<js::Axis>(1));
            double m3 = js::getAxisPosition(m_id, static_cast<js::Axis>(2));
            double m4 = js::getAxisPosition(m_id, static_cast<js::Axis>(3));
            double m5 = js::getAxisPosition(m_id, static_cast<js::Axis>(5));
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

        mode = static_cast<int>(js::isButtonPressed(m_id, 1));
    }

    if(mode == 0){
        if(buttonIndex == 2){ //SG - zero radius mode
            //set zero radius mode
        }
    }
}
