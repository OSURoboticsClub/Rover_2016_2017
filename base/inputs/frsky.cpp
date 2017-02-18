#include "frsky.h"


FrSky::FrSky(int id, QObject *parent)
    : AbstractController(id, parent)
{
}

FrSky::~FrSky()
{

}


FrSky::emitAxisChanges(int axisIndex){
    if(mode == 0){
        if(axisIndex == 0 || axisIndex == 1) { //left and right y - drive motor power
            //sendDriveMotorPower(js::getAxisPosition(m_id, static_cast<js::Axis>(0)), js::getAxisPosition(m_id, static_cast<js::Axis>(1)));
        }
    }
     else if (mode == 1) {
        if(axisIndex == 0 || axisIndex == 1 || axisIndex == 2 || axisIndex == 3 || axisIndex == 5){
            //sendArmMotorPower();
        }
        else if(axisIndex == 4){
            //armGripper();
        }
    }
}


FrSky::emitButtonChanges(int buttonIndex){
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
